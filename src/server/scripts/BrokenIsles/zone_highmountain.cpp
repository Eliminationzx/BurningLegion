/*
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
#include "ScriptedGossip.h"
#include "Player.h"

enum TrueshotLodgeSentinel
{
    TRUESHOT_LODGE_AREA_ID = 7877,
    SPELL_EAGLE_SENTINEL   = 208643,

    TEXT_ID_DETECTION_ANNOUNCEMENT = 0
};

#define SENTINEL_DETECTION_DISTANCE  65.0f

class npc_trueshot_lodge_sentinel : public CreatureScript
{
public:
    npc_trueshot_lodge_sentinel() : CreatureScript("npc_trueshot_lodge_sentinel") { }

    struct npc_trueshot_lodge_sentinelAI : public ScriptedAI
    {
        npc_trueshot_lodge_sentinelAI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
            creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
        }

        void Reset() override { }

        void EnterCombat(Unit* /*who*/) override { }

        void AttackStart(Unit* /*who*/) override { }

        void MoveInLineOfSight(Unit* who) override
        {
            if (!who || !who->IsInWorld() || who->GetAreaId() != TRUESHOT_LODGE_AREA_ID)
                return;

            if (!me->IsWithinDist(who, SENTINEL_DETECTION_DISTANCE, false))
                return;

            Player* player = who->GetCharmerOrOwnerPlayerOrPlayerItself();

            if (!player || player->IsGameMaster() || player->IsBeingTeleported() ||
                player->getClass() == CLASS_HUNTER) // skip only hunters
                return;

            // Detection announcement
            Talk(TEXT_ID_DETECTION_ANNOUNCEMENT, player);
            // Cast eagle sentinel on nearby detected target
            DoCast(player, SPELL_EAGLE_SENTINEL);
            return;
        }

        void UpdateAI(uint32 /*diff*/) override { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_trueshot_lodge_sentinelAI(creature);
    }
};


void AddSC_highmountain()
{
    new npc_trueshot_lodge_sentinel();
}
