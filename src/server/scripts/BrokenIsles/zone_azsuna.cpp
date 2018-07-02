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
#include "PhasingHandler.h"
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
    QUEST_DEATHOFTHE_ELDEST = 37853,
    QUEST_APONI_TRAIL = 43490,
    QUEST_YOU_SCRATH_MY_BACK = 37860
};

enum Area
{
    AREA_RUNAS = 7340
};

enum Killcredit
{
    KILL_CREDIT = 179915,
    SPELL_TELE_NISCARA = 220767,
    KILL_CREDIT_APONI_TELE = 6666664,
    NPC_PILON_RELEASE_1 = 90263,
    NPC_PILON_RELEASE_2 = 100383,
    NPC_PILON_RELEASE_3 = 100384,
    NPC_PILON_RELEASE_4 = 100385,
    KILL_CREDIT_PILON_1 = 444443,
    KILL_CREDIT_PILON_2 = 444444,
    KILL_CREDIT_PILON_3 = 444445,
    KILL_CREDIT_PILON_4 = 444446
};

enum Itemquests
{
    ITEM_FOR_QUEST = 122095,
    ITEM_FOR_QUEST_APONI = 139540
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
            PhasingHandler::AddPhase(player, PHASE_RUNAS, true);
        else if (player->GetQuestStatus(QUEST_HUNGERS_END) == QUEST_STATUS_COMPLETE)
          PhasingHandler::RemovePhase(player, PHASE_RUNAS, true);
    }
}

};

// quest 37853
class npc_killcredit_37853 : public CreatureScript
{
public:
    npc_killcredit_37853() : CreatureScript("npc_killcredit_37853") { }

    struct npc_killcredit_37853AI : public ScriptedAI
    {
        npc_killcredit_37853AI(Creature* creature) : ScriptedAI(creature) { }

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
        return new npc_killcredit_37853AI(creature);
    }
};

// quest 43490
class npc_tele_niscara : public CreatureScript
{
public:
    npc_tele_niscara() : CreatureScript("npc_tele_niscara") { }

    struct npc_tele_niscaraAI : public ScriptedAI
    {
        npc_tele_niscaraAI(Creature* creature) : ScriptedAI(creature) { }

        void MoveInLineOfSight(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
                if (player->GetQuestStatus(QUEST_APONI_TRAIL) == QUEST_STATUS_INCOMPLETE)
                   if (player->HasItemCount(ITEM_FOR_QUEST_APONI, 8) && player->CastSpell(player, SPELL_TELE_NISCARA, true))
                       if (player->IsInDist(me, 2.0f))
                           player->KilledMonsterCredit(KILL_CREDIT_APONI_TELE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_tele_niscaraAI(creature);
    }
};

// quest 37860
class spell_release_whelplings : public SpellScriptLoader
{
    public:
        spell_release_whelplings() : SpellScriptLoader("spell_release_whelplings") { }

        class spell_release_whelplings_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_release_whelplings_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Creature* unitTarget = GetHitCreature())
                {
                    if (Player* player = GetCaster()->ToPlayer())
                    {
                        if (player->GetQuestStatus(QUEST_YOU_SCRATH_MY_BACK) == QUEST_STATUS_INCOMPLETE)
                        {
                            switch (unitTarget->GetEntry())
                            {
                                 case NPC_PILON_RELEASE_1:
                                     player->KilledMonsterCredit(KILL_CREDIT_PILON_1);
                                     break;
                                 case NPC_PILON_RELEASE_2:
                                     player->KilledMonsterCredit(KILL_CREDIT_PILON_2);
                                     break;
                                 case NPC_PILON_RELEASE_3:
                                     player->KilledMonsterCredit(KILL_CREDIT_PILON_3);
                                     break;
                                 case NPC_PILON_RELEASE_4:
                                     player->KilledMonsterCredit(KILL_CREDIT_PILON_4);
                                     break;
                                 default:
                                     break;
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_release_whelplings_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_release_whelplings_SpellScript();
        }
};


void AddSC_azsuna()
{
    new scene_azsuna_runes();
    new area_runes_tickt();
    new npc_killcredit_37853();
    new npc_tele_niscara();
    new spell_release_whelplings();
}
