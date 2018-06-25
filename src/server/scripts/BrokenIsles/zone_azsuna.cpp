/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "MotionMaster.h"
#include "MovementTypedefs.h"
#include "ObjectAccessor.h"
#include "PhasingHandler.h"
#include "Position.h"
#include "QuestDef.h"

class scene_azsuna_runes : public SceneScript
{
public:
    scene_azsuna_runes() : SceneScript("scene_azsuna_runes") { }

    // Called when a player receive trigger from scene
    void OnSceneTriggerEvent(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* sceneTemplate, std::string const& triggerName) override
    {
        if (triggerName == "Credit")
        {
            uint32 killCreditEntry = 0;

            switch (sceneTemplate->ScenePackageId)
            {
                case 1378: //Azsuna - Academy - Runes A(Arcane, Quest) - PRK
                case 1695: //Azsuna - Academy - Runes D(Arcane: Sophomore) - PRK
                case 1696: //Azsuna - Academy - Runes E(Arcane: Junior) - PRK
                case 1697: //Azsuna - Academy - Runes F(Arcane: Senior) - PRK
                    killCreditEntry = 89655;
                    break;
                case 1379: //Azsuna - Academy - Runes B(Fire, Quest) - PRK
                case 1698: //Azsuna - Academy - Runes G(Fire: Freshman) - PRK
                case 1699: //Azsuna - Academy - Runes H(Fire: Junior) - PRK
                case 1700: //Azsuna - Academy - Runes I(Fire: Senior) - PRK
                    killCreditEntry = 89656;
                    break;
                case 1380: //Azsuna - Academy - Runes C(Frost, Quest) - PRK
                case 1701: //Azsuna - Academy - Runes J(Frost: Freshman) - PRK
                case 1702: //Azsuna - Academy - Runes K(Frost: Junior) - PRK
                case 1703: //Azsuna - Academy - Runes L(Frost: Senior) - PRK
                    killCreditEntry = 89657;
                    break;
            }

            if (killCreditEntry)
                player->KilledMonsterCredit(killCreditEntry);
        }
    }

    void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* sceneTemplate) override
    {
        uint32 nextSceneSpellId = 0;

        switch (sceneTemplate->ScenePackageId)
        {
            case 1378: nextSceneSpellId = 223283; break; //Azsuna - Academy - Runes A(Arcane, Quest) - PRK, next : fire quest
            case 1379: nextSceneSpellId = 223287; break; //Azsuna - Academy - Runes B(Fire, Quest) - PRK, next : frost quest
            default:
            case 1695: //Azsuna - Academy - Runes D(Arcane: Sophomore) - PRK
            case 1696: //Azsuna - Academy - Runes E(Arcane: Junior) - PRK
            case 1697: //Azsuna - Academy - Runes F(Arcane: Senior) - PRK
            case 1698: //Azsuna - Academy - Runes G(Fire: Freshman) - PRK
            case 1699: //Azsuna - Academy - Runes H(Fire: Junior) - PRK
            case 1700: //Azsuna - Academy - Runes I(Fire: Senior) - PRK
            case 1380: //Azsuna - Academy - Runes C(Frost, Quest) - PRK
            case 1701: //Azsuna - Academy - Runes J(Frost: Freshman) - PRK
            case 1702: //Azsuna - Academy - Runes K(Frost: Junior) - PRK
            case 1703: //Azsuna - Academy - Runes L(Frost: Senior) - PRK
                break;
        }

        if (nextSceneSpellId)
            player->CastSpell(player, nextSceneSpellId, true);
    }
};


enum Phases
{
    PHASE_RUNAS = 51
};

enum Quests
{
    QUEST_HUNGERS_END = 42756,
    QUEST_DEATHOFTHE_ELDEST = 37853
};

enum Area
{
    AREA_RUNAS = 7340
};

enum Killcredit
{
    KILL_CREDIT = 179915
};

enum Itemquests
{
    ITEM_FOR_QUEST = 122095
};
//quest 42756
class area_runes_tickt: public PlayerScript
{
public:
    area_runes_tickt() : PlayerScript("area_runes_tickt") {}
    
void OnUpdateArea(Player* player, uint32 newAreaId, uint32 /*oldAreaID*/) override
{
    if (newAreaId == AREA_RUNAS)
    {
                       
        if (player->GetQuestStatus(QUEST_HUNGERS_END) == QUEST_STATUS_INCOMPLETE)
        {
            PhasingHandler::AddPhase(player, PHASE_RUNAS, true);
        }
        else
        {
          player->GetQuestStatus(QUEST_HUNGERS_END) == QUEST_STATUS_COMPLETE;    
          PhasingHandler::RemovePhase(player, PHASE_RUNAS, true);
        }          

    }
}

};

// quest 37853
class npc_killcredit : public CreatureScript
{
public:
    npc_killcredit() : CreatureScript("npc_killcredit") { }

    struct npc_killcreditAI : public ScriptedAI
    {
        npc_killcreditAI(Creature* creature) : ScriptedAI(creature) { }

        void MoveInLineOfSight(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(QUEST_DEATHOFTHE_ELDEST) == QUEST_STATUS_INCOMPLETE)
                {    
                   if (player->HasItemCount(ITEM_FOR_QUEST, 6) && player->CastSpell(player, KILL_CREDIT, true))
                   {
                     player->IsInDist(me, 4.0f);
                     player->KilledMonsterCredit(me->GetEntry());
                   }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_killcreditAI(creature);
    }
};

void AddSC_azsuna()
{
    new scene_azsuna_runes();
    new area_runes_tickt();
    new npc_killcredit();
}
