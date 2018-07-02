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
#include "World.h"
#include "WorldSession.h"
#include "Log.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundQueue.h"
#include "Group.h"

class npc_rate_xp_modifier : public CreatureScript
{
    public:
        npc_rate_xp_modifier() : CreatureScript("npc_rate_xp_modifier") { }

#define MAX_RATE uint32(10)

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            for (uint32 i = 1; i <= MAX_RATE; ++i)
            {
                if (i == player->GetPersonnalXpRate())
                    continue;

                if (i == sWorld->getRate(RATE_XP_KILL))
                    continue;

                std::ostringstream gossipText;
                gossipText << "Rate x" << i;
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, i);
            }

            if (player->GetPersonnalXpRate())
            {
                std::ostringstream gossipText;
                gossipText << "Default Rate - x" << sWorld->getRate(RATE_XP_KILL);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, 0);
            }

            SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*uiSender*/, uint32 uiAction) override
        {
            CloseGossipMenuFor(player);
            player->SetPersonnalXpRate(float(std::min(MAX_RATE, uiAction)));
            return true;
        }
};

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
            return;

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

void AddSC_custom_npcs()
{
    new npc_rate_xp_modifier();
    new npc_challenger();
}
