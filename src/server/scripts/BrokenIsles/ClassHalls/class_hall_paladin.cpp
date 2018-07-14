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
#include "ScriptedGossip.h"
#include "Player.h"

enum SanctumOfLightGuard
{
    SANCTUM_OF_LIGHT_AREA_ID         = 7638,
    SPELL_HAMMER_OF_SUPREME_JUSTICE  = 195558,
    SPELL_HOLY_CHANNELING            = 118247,

    EVENT_CATCH_INTRUDER             = 1,
    EVENT_TELE_OUT                   = 2
};

const Position teleOutPos = { 2283.882080f, -5322.789551f, 89.235878f, 2.362668f };

class npc_sanctum_of_light_guard : public CreatureScript
{
    public:
        npc_sanctum_of_light_guard() : CreatureScript("npc_sanctum_of_light_guard") {}

        struct npc_sanctum_of_light_guardAI : public ScriptedAI
        {
            npc_sanctum_of_light_guardAI(Creature* creature) : ScriptedAI(creature) 
            { 
                _target = nullptr;

                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
                creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
            }

            void Reset() override { }

            void EnterCombat(Unit* /*who*/) override { }

            void AttackStart(Unit* /*who*/) override { }

            void MoveInLineOfSight(Unit* who) override
            {
                if (!who || !who->IsInWorld() || who->GetAreaId() != SANCTUM_OF_LIGHT_AREA_ID)
                    return;

                if ((who->getClass() == CLASS_PALADIN && who->getLevel() > 100) || who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (!me->IsWithinDist(who, 65.0f, false))
                    return;

                DoCast(who, SPELL_HAMMER_OF_SUPREME_JUSTICE);
                _target = who;

                events.ScheduleEvent(EVENT_CATCH_INTRUDER, 1s);

                return;
            }

            void UpdateAI(uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CATCH_INTRUDER:
                        {
                            if (!_target)
                                break;

                            // Send message
                            Talk(0);

                            // Add visual channeling
                            DoCastSelf(SPELL_HOLY_CHANNELING);

                            events.ScheduleEvent(EVENT_TELE_OUT, 4s);

                            break;
                        }
                        case EVENT_TELE_OUT:
                        {
                            // Tele out
                            if (_target)
                                _target->NearTeleportTo(teleOutPos);

                            // Remove visual channeling
                            if (me->HasAura(SPELL_HOLY_CHANNELING))
                                me->RemoveAura(SPELL_HOLY_CHANNELING);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            Unit* _target;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sanctum_of_light_guardAI(creature);
    }
};

void AddSC_class_hall_paladin()
{
    new npc_sanctum_of_light_guard();
}
