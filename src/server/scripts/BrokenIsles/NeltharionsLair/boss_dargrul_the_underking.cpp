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
#include "neltharions_lair.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "GameObject.h"
#include "Unit.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_EMOTE           = 1,
    SAY_DEATH           = 2,
};

enum Spells
{
    SPELL_GAIN_ENERGY               = 201444,
    SPELL_CONVERSATION_01           = 201665, //SPELL_MAGMA_SCULPTOR
    SPELL_CONVERSATION_02           = 201666, //SPELL_MAGMA_SCULPTOR
    SPELL_MAGMA_SCULPTOR            = 200637, 
    SPELL_LANDSLIDE                 = 200700,
    SPELL_LANDSLIDE_MISSILE         = 200722,
    SPELL_CRYSTAL_WALL              = 200551,
    SPELL_MAGMA_WAVE                = 200404,
    SPELL_MAGMA_WAVE_AT             = 200418,
    SPELL_MAGMA_WAVE_DMG_AOE        = 217090,
    SPELL_MOLTEN_CRASH              = 200732,

    //Heroic
    SPELL_MAGMA_BREAKER_DARGRUL     = 216368,
    SPELL_LAVA_GEYSER_SUM           = 216373,
    SPELL_LAVA_GEYSER_AT            = 216385,
    SPELL_FLAME_GOUT                = 216374,

    SPELL_CONVERSATION_03           = 201663,
    SPELL_CONVERSATION_04           = 201659,
    SPELL_CONVERSATION_05           = 201661, //SPELL_MAGMA_WAVE

    SPELL_SUM_PILLAR_CREATION       = 213589,

    //Trash
    SPELL_FIXATE_PLR                = 200154,
    SPELL_COLLISION_AT              = 188197,
    SPELL_MAGMA_BREAKER             = 209926,
    SPELL_MAGMA_BREAKER_AURA        = 209920,
    SPELL_RISING_INFERNO            = 216369,

    SPELL_CRYSTAL_BLOCKER_AT        = 200367,
    SPELL_CRYSTAL_WALL_SUM_GO       = 209964, //GO DOOR
};

enum eEvents
{
    EVENT_MAGMA_SCULPTOR            = 1,
    EVENT_LANDSLIDE                 = 2,
    EVENT_CRYSTAL_WALL              = 3,
    EVENT_MOLTEN_CRASH              = 4,
    //Heroic
    EVENT_LAVA_GEYSER               = 5,
};

//91007
class boss_dargrul_the_underking : public CreatureScript
{
public:
    boss_dargrul_the_underking() : CreatureScript("boss_dargrul_the_underking") {}

    struct boss_dargrul_the_underkingAI : public BossAI
    {
        boss_dargrul_the_underkingAI(Creature* creature) : BossAI(creature, DATA_DARGRUL) 
        {
            me->SetMaxPower(POWER_MANA, 64);
            introDone = false;
        }

        std::list<ObjectGuid> listGuid;
        bool introDone;
        uint8 rand;
        uint16 magmaTimer;

        void Reset() override
        {
            _Reset();
            me->RemoveAurasDueToSpell(SPELL_GAIN_ENERGY);
            me->SetPower(POWER_MANA, 0);
            magmaTimer = 2000;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO); //Yes. TRY to take the hammer from me!
            _EnterCombat();
            DoCast(me, SPELL_GAIN_ENERGY, true);

            if (GetDifficulty() != DIFFICULTY_NORMAL)
                DoCast(me, SPELL_MAGMA_BREAKER_DARGRUL, true);

            events.ScheduleEvent(EVENT_MAGMA_SCULPTOR, 10000);
            events.ScheduleEvent(EVENT_LANDSLIDE, 16000);
            events.ScheduleEvent(EVENT_CRYSTAL_WALL, 6000);
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);
            _JustDied();
        }
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (!introDone && me->IsWithinDistInMap(who, 115.0f))
            {
                who->CastSpell(who, 209698, true);
                introDone = true;
            }
        }

        void SpellHitTarget(Unit* /*target*/, const SpellInfo* spell) override
        {
            if (spell->Id == SPELL_LANDSLIDE)
            {
                Position pos;
                float angle;
                for (uint8 i = 0; i < 100; ++i)
                {
                    angle = frand(-0.17f, 0.17f);
                    pos = me->GetNearPosition(i, angle);
                    me->CastSpell(pos, SPELL_LANDSLIDE_MISSILE, true);
                }
            }
        }

        void DoAction(int32 const /*action*/) override
        {
            if (GetDifficulty() != DIFFICULTY_NORMAL)
                events.ScheduleEvent(EVENT_LAVA_GEYSER, 1000);
        }

        bool GetObjectData(ObjectGuid const& guid)
        {
            bool find = false;

            for (auto targetGuid : listGuid)
                if (targetGuid == guid)
                    find = true;

            if (!find)
                listGuid.push_back(guid);

            return find;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                if (magmaTimer <= diff)
                {
                    magmaTimer = 2000;

                    if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                        if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_MAGMA_WAVE)
                            DoCast(me, SPELL_MAGMA_WAVE_DMG_AOE, true);
                }
                else
                    magmaTimer -= diff;
                return;
            }

            if (me->GetPositionZ() > -205.0f)
            {
                EnterEvadeMode();
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MAGMA_SCULPTOR:
                        rand = urand(0, 1);
                        DoCast(me, rand ? SPELL_CONVERSATION_01 : SPELL_CONVERSATION_02, true);
                        DoCast(SPELL_MAGMA_SCULPTOR);
                        events.ScheduleEvent(EVENT_MAGMA_SCULPTOR, 70000);
                        break;
                    case EVENT_LANDSLIDE:
                        listGuid.clear();
                        DoCast(SPELL_LANDSLIDE);
                        events.ScheduleEvent(EVENT_LANDSLIDE, 16000);
                        events.ScheduleEvent(EVENT_MOLTEN_CRASH, 3000);
                        break;
                    case EVENT_CRYSTAL_WALL:
                        DoCast(SPELL_CRYSTAL_WALL);
                        events.ScheduleEvent(EVENT_CRYSTAL_WALL, 22000);
                        break;
                    case EVENT_MOLTEN_CRASH:
                        DoCastVictim(SPELL_MOLTEN_CRASH);
                        break;
                    case EVENT_LAVA_GEYSER:
                        DoCast(SPELL_LAVA_GEYSER_SUM);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_dargrul_the_underkingAI (creature);
    }
};

//101476
class npc_dargrul_molten_charskin : public CreatureScript
{
public:
    npc_dargrul_molten_charskin() : CreatureScript("npc_dargrul_molten_charskin") {}

    struct npc_dargrul_molten_charskinAI : public ScriptedAI
    {
        npc_dargrul_molten_charskinAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        ObjectGuid playerGuid;

        void Reset() override {}

        void IsSummonedBy(Unit* summoner) override
        {
            DoZoneInCombat(me, 100.0f);

            if (me->GetMap()->GetDifficultyID() != DIFFICULTY_NORMAL)
                DoCast(me, SPELL_RISING_INFERNO, true);
            else
                DoCast(me, SPELL_MAGMA_BREAKER_AURA, true);

            events.ScheduleEvent(1, 2000);
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->RemoveAurasDueToSpell(SPELL_MAGMA_BREAKER_AURA);
        }

        void SpellHitTarget(Unit* /*target*/, const SpellInfo* spell) override
        {
            if (spell->Id == SPELL_FIXATE_PLR)
            {
                playerGuid = target->GetGUID();
                AttackStart(target);
                me->ClearUnitState(UNIT_STATE_CASTING);
            }
        }

        bool checkPlayers()
        {
            std::list<HostileReference*> threatList = me->getThreatManager().getThreatList();
            if (threatList.size() > 1)
                return true;

            return false;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case 1:
                    {
                        Unit* player = ObjectAccessor::GetUnit(*me, playerGuid);
                        if (!player || !player->IsAlive() || !player->HasAura(SPELL_FIXATE_PLR))
                        {
                            if (me->GetOwner())
                                if (Creature* summoner = me->GetOwner()->ToCreature())
                                    if (Unit* pTarget = summoner->AI()->SelectTarget(SELECT_TARGET_RANDOM, checkPlayers(), 80.0f, true))
                                    {
                                        playerGuid.Clear();
                                        me->AttackStop();
                                        me->CastSpell(pTarget, SPELL_FIXATE_PLR, TriggerCastFlags(TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_IGNORE_AURA_INTERRUPT_FLAGS));
                                    }
                        }
                        else
                            AttackStart(player);
                        events.ScheduleEvent(1, 2000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dargrul_molten_charskinAI(creature);
    }
};

//101593
class npc_dargrul_crystal_wall : public CreatureScript
{
public:
    npc_dargrul_crystal_wall() : CreatureScript("npc_dargrul_crystal_wall") {}

    struct npc_dargrul_crystal_wallAI : public ScriptedAI
    {
        npc_dargrul_crystal_wallAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        bool despawn = false;

        void Reset() override {}

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            if (!summoner->IsInCombat())
            {
                me->DespawnOrUnsummon();
                return;
            }

            DoCast(me, SPELL_CRYSTAL_BLOCKER_AT, true);
            DoCast(me, SPELL_CRYSTAL_WALL_SUM_GO, true);
        }

        void OnAreaTriggerCast(Unit* /*caster*/, Unit* /*target*/, uint32 spellId, uint32 /*createATSpellId*/)
        {
            if (spellId == 200672) //Stun
                Despawn();
        }

        void SpellHit(Unit* caster, const SpellInfo* spell) override
        {
            if (spell->Id == 200721 || spell->Id == 209947)
                Despawn();
        }

        void Despawn()
        {
            if (despawn)
                return;

            despawn = true;

            me->AddTimedDelayedOperation(200, [this] () -> void
            {
                if (me)
                    me->DespawnOrUnsummon();
            });
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            damage = 0;
        }

        void UpdateAI(uint32 diff) override {}
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dargrul_crystal_wallAI(creature);
    }
};

//108926
class npc_dargrul_lava_geyser : public CreatureScript
{
public:
    npc_dargrul_lava_geyser() : CreatureScript("npc_dargrul_lava_geyser") {}

    struct npc_dargrul_lava_geyserAI : public ScriptedAI
    {
        npc_dargrul_lava_geyserAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() override {}
        
        void IsSummonedBy(Unit* summoner) override
        {
            DoCast(me, SPELL_LAVA_GEYSER_AT, true);
            DoCast(me, SPELL_FLAME_GOUT, true);
        }

        void UpdateAI(uint32 diff) override {}
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dargrul_lava_geyserAI(creature);
    }
};

//201444
class spell_dargrul_gain_energy : public SpellScriptLoader
{
public:
    spell_dargrul_gain_energy() : SpellScriptLoader("spell_dargrul_gain_energy") { }

    class spell_dargrul_gain_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dargrul_gain_energy_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Creature* caster = GetCaster()->ToCreature();
            if (!caster)
                return;

            if (caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (caster->GetPower(POWER_MANA) >= 64)
            {
                caster->CastSpell(caster, SPELL_CONVERSATION_05, true);
                caster->CastSpell(caster, SPELL_MAGMA_WAVE_AT);
                //caster->m_Events.AddEvent(new DelayCastEvent(*caster, caster->GetGUID(), SPELL_MAGMA_WAVE, false), caster->m_Events.CalculateTime(500));
                caster->AI()->DoAction(true);
                if (Creature* target = caster->FindNearestCreature(91007, 50, true))
                    target->AI()->Talk(SAY_EMOTE);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dargrul_gain_energy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dargrul_gain_energy_AuraScript();
    }
};

//209920, 216368
class spell_dargrul_magma_breaker : public SpellScriptLoader
{
public:
    spell_dargrul_magma_breaker() : SpellScriptLoader("spell_dargrul_magma_breaker") { }

    class spell_dargrul_magma_breaker_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dargrul_magma_breaker_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->isMoving())
                caster->CastSpell(caster, SPELL_MAGMA_BREAKER, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dargrul_magma_breaker_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dargrul_magma_breaker_AuraScript();
    }
};

//200404, 217090
class spell_dargrul_magma_wave_filter : public SpellScriptLoader
{
public:
    spell_dargrul_magma_wave_filter() : SpellScriptLoader("spell_dargrul_magma_wave_filter") { }

    class spell_dargrul_magma_wave_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dargrul_magma_wave_filter_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            if (targets.empty())
                return;

            std::list<GameObject*> goList;
            GetCaster()->GetGameObjectListWithEntryInGrid(goList, GO_CRYSTAL_WALL_COLLISION, 100.0f);
            if (!goList.empty())
                for (std::list<GameObject*>::const_iterator itr = goList.begin(); itr != goList.end(); ++itr)
                    targets.remove_if(WallCheck(GetCaster(), (*itr)));
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dargrul_magma_wave_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }

    private:
        class WallCheck
        {
            public:
                WallCheck(Unit* searcher, GameObject* go) : _searcher(searcher), _go(go) {}
        
                bool operator()(WorldObject* unit)
                {
                    return (_go->IsInBetween(_searcher, unit, 4.0f));
                }

            private:
                Unit* _searcher;
                GameObject* _go;
        };
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dargrul_magma_wave_filter_SpellScript();
    }
};

//200732
class spell_dargrul_molten_crash : public SpellScriptLoader
{
public:
    spell_dargrul_molten_crash() : SpellScriptLoader("spell_dargrul_molten_crash") { }

    class spell_dargrul_molten_crash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dargrul_molten_crash_SpellScript);

        void HandleTriggerEffect(SpellEffIndex /*effIndex*/)
        {
            //if (GetHitUnit() && GetHitUnit()->IsActiveMitigation())
                PreventHitDefaultEffect(EFFECT_1);
        }

        void Register() override
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_dargrul_molten_crash_SpellScript::HandleTriggerEffect, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dargrul_molten_crash_SpellScript();
    }
};

//200721
class spell_dargrul_landslide_filter : public SpellScriptLoader
{
    public:
        spell_dargrul_landslide_filter() : SpellScriptLoader("spell_dargrul_landslide_filter") { }

        class spell_dargrul_landslide_filter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dargrul_landslide_filter_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (auto caster = GetCaster()->ToCreature())
                {
                    for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        //if (caster->AI()->GetObjectData((*itr)->GetGUID()))
                            targets.remove(*itr++);
                }
            }

            void FilterTargetsEntry(std::list<WorldObject*>& targets)
            {
                targets.remove_if([this](WorldObject* object) -> bool
                {
                    if (object == nullptr || object->GetEntry() != NPC_CRYSTAL_WALL_STALKER || object->GetDistance(*GetExplTargetDest()) >= 3.0f)
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dargrul_landslide_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dargrul_landslide_filter_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dargrul_landslide_filter_SpellScript::FilterTargetsEntry, EFFECT_2, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dargrul_landslide_filter_SpellScript();
        }
};

// 102295 trash
class npc_emberhusk_dominator : public CreatureScript
{
public:
    npc_emberhusk_dominator() : CreatureScript("npc_emberhusk_dominator") {}

    struct npc_emberhusk_dominatorAI : public ScriptedAI
    {
        npc_emberhusk_dominatorAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 SpellTimer;
        uint32 SayTimer;

        void Reset() override
        {
            SpellTimer = urand (3000, 4000);
            SayTimer = urand(30000, 45000);
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (SpellTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    me->CastSpell(target, 201959, TriggerCastFlags(TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE));

                SpellTimer = urand (3000, 4000);
            } else SpellTimer -= diff;

            if (SayTimer <= diff)
            {
                SayTimer = urand(30000, 45000);
                Talk(0);
            } else SayTimer -= diff;
        }
        
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_emberhusk_dominatorAI(creature);
    }
};

void AddSC_boss_dargrul_the_underking()
{
    new boss_dargrul_the_underking();
    new npc_dargrul_molten_charskin();
    new npc_dargrul_crystal_wall();
    new npc_dargrul_lava_geyser();
    new spell_dargrul_gain_energy();
    new spell_dargrul_magma_breaker();
    new spell_dargrul_magma_wave_filter();
    new spell_dargrul_molten_crash();
    new spell_dargrul_landslide_filter();
    //trash
    new npc_emberhusk_dominator();
}