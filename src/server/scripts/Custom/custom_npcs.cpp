/*
* Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "DB2Stores.h"
#include "Creature.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellHistory.h"
#include "World.h"
#include "WorldSession.h"
#include "Log.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundQueue.h"
#include "Group.h"
#include "Language.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Chat.h"

enum Challenger
{
    GOSSIP_ACTION_SHOW_MAP_MENU = 100,
    GOSSIP_MENU_ID_CHALLENGER = 44467
};

enum ChallengeTypes
{
    CHALLENGE_TYPE_1v1 = 100,
    CHALLENGE_TYPE_2v2 = 200,
    CHALLENGE_TYPE_3v3 = 300,
    CHALLENGE_TYPE_5v5 = 500
};

class npc_challenger : public CreatureScript
{
public:
    npc_challenger() : CreatureScript("npc_challenger") { }

    static bool JoinQueueArena(Player* player, uint8 type, BattlegroundTypeId bgTypeId)
    {
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
        if (!bg)
        {
            TC_LOG_ERROR("network", "Battleground: template bg (all arenas) not found");
            return false;
        }

        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, type);
        PVPDifficultyEntry const* bracketEntry = DB2Manager::GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
        if (!bracketEntry)
            return false;

        // check if already in queue
        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return false;
        // check if has free queue slots
        if (!player->HasFreeBattlegroundQueueId())
            return false;

        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = nullptr;
        uint32 avgTime = 0;
        uint32 queueSlot = 0;
        WorldPacket data;

        if (Group* grp = player->GetGroup())
        {
            if (grp->GetLeaderGUID() != player->GetGUID())
                return false;

            ObjectGuid errorGuid;
            GroupJoinBattlegroundResult err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, type, type, false, 0, errorGuid);

            if (!err)
            {
                ginfo = bgQueue.AddGroup(player, grp, bgTypeId, bracketEntry, type, false, false, 0, 0);
                avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
            }

            for (GroupReference* itr = grp->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member)
                    continue;   // this should never happen

                if (err)
                {
                    WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
                    sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, 0, err, &errorGuid);
                    member->SendDirectMessage(battlefieldStatus.Write());
                    continue;
                }

                queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

                WorldPackets::Battleground::BattlefieldStatusQueued battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusQueued(&battlefieldStatus, bg, member, queueSlot, ginfo->JoinTime, avgTime, type, true);
                member->SendDirectMessage(battlefieldStatus.Write());
            }
        }
        else
        {
            queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);
            ginfo = bgQueue.AddGroup(player, nullptr, bgTypeId, bracketEntry, type, false, false, 0, 0);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());

            WorldPackets::Battleground::BattlefieldStatusQueued battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusQueued(&battlefieldStatus, bg, player, queueSlot, ginfo->JoinTime, avgTime, type, false);
            player->GetSession()->SendPacket(battlefieldStatus.Write());
        }

        sBattlegroundMgr->ScheduleQueueUpdate(0, type, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_1v1);
        AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_2v2);
        AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_3v3);
        AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_5v5);
        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);

        switch (action)
        {
            case GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_1v1:
            case GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_2v2:
            case GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_3v3:
            case GOSSIP_ACTION_SHOW_MAP_MENU + CHALLENGE_TYPE_5v5:
            {
                uint32 challengeType = action - GOSSIP_ACTION_SHOW_MAP_MENU;
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 4, GOSSIP_SENDER_MAIN, BATTLEGROUND_BE + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 5, GOSSIP_SENDER_MAIN, BATTLEGROUND_DS + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 6, GOSSIP_SENDER_MAIN, BATTLEGROUND_NA + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 7, GOSSIP_SENDER_MAIN, BATTLEGROUND_RL + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 8, GOSSIP_SENDER_MAIN, BATTLEGROUND_RV + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 9, GOSSIP_SENDER_MAIN, BATTLEGROUND_TTP + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 10, GOSSIP_SENDER_MAIN, BATTLEGROUND_TVA + challengeType);
                AddGossipItemFor(player, GOSSIP_MENU_ID_CHALLENGER, 11, GOSSIP_SENDER_MAIN, BATTLEGROUND_AA + challengeType);
                SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
                break;
            }
            // Blade's Edge arena
            case BATTLEGROUND_BE + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_BE + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_BE + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_BE + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_BE) / 100, BATTLEGROUND_BE);
                CloseGossipMenuFor(player);
                break;
            }
            // Dalaran Sewers
            case BATTLEGROUND_DS + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_DS + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_DS + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_DS + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_DS) / 100, BATTLEGROUND_DS);
                CloseGossipMenuFor(player);
                break;
            }
            // Nagrand Arena
            case BATTLEGROUND_NA + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_NA + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_NA + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_NA + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_NA) / 100, BATTLEGROUND_NA);
                CloseGossipMenuFor(player);
                break;
            }
            // Ruins of Lordarenon
            case BATTLEGROUND_RL + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_RL + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_RL + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_RL + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_RL) / 100, BATTLEGROUND_RL);
                CloseGossipMenuFor(player);
                break;
            }
            // Ring of Valor
            case BATTLEGROUND_RV + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_RV + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_RV + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_RV + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_RV) / 100, BATTLEGROUND_RV);
                CloseGossipMenuFor(player);
                break;
            }
            // The Tiger's Peak
            case BATTLEGROUND_TTP + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_TTP + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_TTP + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_TTP + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_TTP) / 100, BATTLEGROUND_TTP);
                CloseGossipMenuFor(player);
                break;
            }
            // Tol'Viron Arena
            case BATTLEGROUND_TVA + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_TVA + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_TVA + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_TVA + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_TVA) / 100, BATTLEGROUND_TVA);
                CloseGossipMenuFor(player);
                break;
            }
            // All Arenas
            case BATTLEGROUND_AA + CHALLENGE_TYPE_1v1:
            case BATTLEGROUND_AA + CHALLENGE_TYPE_2v2:
            case BATTLEGROUND_AA + CHALLENGE_TYPE_3v3:
            case BATTLEGROUND_AA + CHALLENGE_TYPE_5v5:
            {
                JoinQueueArena(player, (action - BATTLEGROUND_AA) / 100, BATTLEGROUND_AA);
                CloseGossipMenuFor(player);
                break;
            }
            default:
                break;
        }
        return true;
    }
};

enum PremiumMinion
{
    GOSSIP_MENU_ID_PREMIUM_MINION       = 44468,

    // - 1 Premium trial
    GOSSIP_ACTION_REPAIR_TOOL           = 100,
    GOSSIP_ACTION_MORPH                 = 200,
    GOSSIP_ACTION_DEMORPH               = 300,
    GOSSIP_ACTION_OPEN_BANK             = 400,

    // - 2 Premium normal
    GOSSIP_ACTION_RESET_TALENTS         = 500,
    GOSSIP_ACTION_CHEAT_TAXI            = 600,
    GOSSIP_ACTION_TELE_HOME             = 700,
    GOSSIP_ACTION_BUFF                  = 800,

    // - 3 Premium elite
    GOSSIP_ACTION_APPEAR_FRIEND         = 900,
    GOSSIP_ACTION_CHANGE_RACE           = 1000,
    GOSSIP_ACTION_CHANGE_FACTION        = 1100,
    GOSSIP_ACTION_CHAR_CUSTOMIZE        = 1200,
    GOSSIP_ACTION_CHOOSE_PERSONAL_RATES = 1300,
    GOSSIP_ACTION_RENAME                = 1400

    // TODO: Buffs
};

#define GOSSIP_TEXT_APPEAR_ERROR "Something wrong. We can't find your friend"
#define GOSSIP_TEXT_SUCCESS_CHANGE_FACTION "Now you can change faction after logout"
#define GOSSIP_TEXT_SUCCESS_CHANGE_RACE "Now you can change your race after logout"
#define GOSSIP_TEXT_SUCCESS_CHAR_CUSTOMIZE "Now you can customize character after logout"
#define GOSSIP_TEXT_SUCCESS_CHAR_RENAME "Now you can rename character after logout"
#define GOSSIP_TEXT_SUCCESS_REPAIR "We successfully repair all your items"
#define GOSSIP_TEXT_SUCCESS_MORPH "You looks better now!"
#define GOSSIP_TEXT_SUCCESS_DEMORPH "You returned your natural form"
#define GOSSIP_TEXT_SUCCESS_BUFF "Now you are stronger!"
#define GOSSIP_TEXT_SUCCESS_TAXI "We successfully open all fly path for you"
#define GOSSIP_TEXT_SUCCESS_RESET_TALENTS "We successfully reset your talents"
#define GOSSIP_TEXT_SUCCESS_TELE_HOME "You successfully returned back to home!"
#define GOSSIP_TEXT_SUCCESS_RATE_CHANGE "You successfully changed personal rates"
#define GOSSIP_TEXT_ERROR_RATE_CHANGE "Something wrong. We can't allow you to change personal rates"

#define MAX_PERSONAL_RATES 100

class npc_premium_minion : public CreatureScript
{
public:
    npc_premium_minion() : CreatureScript("npc_premium_minion") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (player->GetSession()->IsPremium())
        {
            switch (player->GetSession()->GetPremiumType())
            {
                case PREMIUM_TRIAL:
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_REPAIR_TOOL);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_MORPH);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_DEMORPH);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_OPEN_BANK);
                    break;
                case PREMIUM_NORMAL:
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_REPAIR_TOOL);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_MORPH);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_DEMORPH);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_OPEN_BANK);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_RESET_TALENTS);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHEAT_TAXI);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TELE_HOME);
                    // TODO AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_BUFF);
                    break;
                case PREMIUM_ELITE:
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_REPAIR_TOOL);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_MORPH);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_DEMORPH);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_OPEN_BANK);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_RESET_TALENTS);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHEAT_TAXI);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TELE_HOME);
                    // TODO AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_BUFF);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_APPEAR_FRIEND);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHANGE_RACE);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHANGE_FACTION);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHAR_CUSTOMIZE);
                    AddGossipItemFor(player, GOSSIP_MENU_ID_PREMIUM_MINION, 12, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHOOSE_PERSONAL_RATES);
                    break;
                default:
                    break;
            }
        }

        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);

        if (player->IsInCombat() || player->InArena() || player->InBattleground())
            return false;

        switch (action)
        {
            case GOSSIP_ACTION_REPAIR_TOOL:
            {
                player->DurabilityRepairAll(false, 0, false);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_REPAIR, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_MORPH:
            {
                uint32 models[17] = { 25900, 18718, 29348, 22235, 30414, 736, 20582, 28213, 2345, 2344, 25409, 25418, 25422, 25432, 5555, 5554, 5556 };
                player->SetDisplayId(models[urand(0, 16)]);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_MORPH, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_DEMORPH:
            {
                player->DeMorph();
                creature->Whisper(GOSSIP_TEXT_SUCCESS_DEMORPH, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_OPEN_BANK:
            {
                player->GetSession()->SendShowBank(player->GetGUID());
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_RESET_TALENTS:
            {
                player->ResetTalents(true);
                player->ResetTalentSpecialization();
                player->SendTalentsInfoData();
                creature->Whisper(GOSSIP_TEXT_SUCCESS_RESET_TALENTS, LANG_UNIVERSAL, player);

                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_CHEAT_TAXI:
            {
                if (!player->isTaxiCheater())
                {
                    player->SetTaxiCheater(true);
                    creature->Whisper(GOSSIP_TEXT_SUCCESS_TAXI, LANG_UNIVERSAL, player);
                }

                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_TELE_HOME:
            {
                player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_TELE_HOME, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_BUFF:
            {
                /*std::vector<uint32> buffs;
                buffs.resize(3);

                buffs.push_back(1459);
                buffs.push_back(21562);
                buffs.push_back(546);
      
                for (auto buffId: buffs)
                    player->AddAura(buffId, player);

                creature->Whisper(GOSSIP_TEXT_SUCCESS_BUFF, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                    */
                break;
            }          
            case GOSSIP_ACTION_CHANGE_RACE:
            {
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_CHANGE_RACE, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_CHANGE_FACTION:
            {
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_CHANGE_FACTION, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_CHAR_CUSTOMIZE:
            {
                player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_CHAR_CUSTOMIZE, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_RENAME:
            {
                player->SetAtLoginFlag(AT_LOGIN_RENAME);
                creature->Whisper(GOSSIP_TEXT_SUCCESS_CHAR_RENAME, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
            }
            default:
                break;
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code) override
    {
        ClearGossipMenuFor(player);

        if (player->IsInCombat() || player->InArena() || player->InBattleground())
            return false;

        if (sender == GOSSIP_SENDER_MAIN)
        {
            switch (action)
            {
                case GOSSIP_ACTION_CHOOSE_PERSONAL_RATES:
                    ChangePersonalRates(player, creature, code);
                    CloseGossipMenuFor(player);
                    break;
                case GOSSIP_ACTION_APPEAR_FRIEND:
                    AppearFriend(player, creature, code);
                    CloseGossipMenuFor(player);
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    static void AppearFriend(Player* player, Creature* creature, const char* code)
    {
        const char* plrName = code;
        char playerName[50];
        strcpy(playerName, plrName);
        for (int i = 0; i < 13; i++)
        {
            if (playerName[i] == '\0')
                break;
            if (i == 0 && playerName[i] > 96)
                playerName[0] -= 32;
            else if (playerName[i] < 97)
                playerName[i] += 32;
        }

        if (Player* target = ObjectAccessor::FindPlayerByName(playerName))
        {
            if (target->IsInFlight() || target->InBattleground() || target->InArena() || !target->IsFriendlyTo(player))
            {
                creature->Whisper(GOSSIP_TEXT_APPEAR_ERROR, LANG_UNIVERSAL, player);
                return;
            }

            // stop flight if need
            if (player->IsInFlight())
            {
                player->GetMotionMaster()->MovementExpired();
                player->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                player->SaveRecallPosition();

            // to point to see at target with same orientation
            float x, y, z;
            target->GetContactPoint(player, x, y, z);

            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target));
        }
    }

    static void ChangePersonalRates(Player* player, Creature* creature, const char* code)
    {
        int selectedRates = atoi(code);

        if (selectedRates <= 0 || selectedRates > MAX_PERSONAL_RATES || 
            player->GetPersonalRate() == float(selectedRates))
        {
            creature->Whisper(GOSSIP_TEXT_ERROR_RATE_CHANGE, LANG_UNIVERSAL, player);
            return;
        }

        player->SetPersonalRate(float(selectedRates));
        creature->Whisper(GOSSIP_TEXT_SUCCESS_RATE_CHANGE, LANG_UNIVERSAL, player);
    }
};

void AddSC_custom_npcs()
{
    new npc_challenger();
    new npc_premium_minion();
}
