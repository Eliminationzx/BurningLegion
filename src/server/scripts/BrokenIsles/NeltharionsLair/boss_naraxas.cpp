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
#include "neltharions_lair.h"
#include "Vehicle.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"

enum Says
{
    SAY_SUM             = 0,
    SAY_EMOTE           = 1,
};

enum Spells
{
    SPELL_INTRO_MYSTIC          = 209625, //Boss 03 Intro Mystic Cast - визуально прячется?
    SPELL_INTRO_EMERGE          = 209641, //Boss 03 Intro Emerge - hit npc 105766
    SPELL_GAIN_ENERGY           = 200086,
    SPELL_PUTRID_SKIES          = 198963,
    SPELL_FRENZY                = 199775,
    SPELL_RANCID_MAW            = 205549,
    SPELL_TOXIC_WRETCH          = 210150,
    SPELL_TOXIC_WRETCH_AT       = 210159,
    SPELL_SPIKED_TONGUE_CHANNEL = 199178,
    SPELL_SPIKED_TONGUE_CHECK_R = 199335, //Check Radius
    SPELL_SPIKED_TONGUE_RIDE    = 205417,
    SPELL_SPIKED_TONGUE         = 199176,
    SPELL_SPIKED_TONGUE_AT      = 199187,
    SPELL_SPIKED_TONGUE_DMG     = 199705,
    SPELL_SPIKED_TONGUE_JUMP    = 204136,
    SPELL_RAVENOUS              = 199246,
    SPELL_DEVOUR_FANATIC        = 216797,

    //Heroic
    SPELL_CALL_ANGRY_CROWD      = 217028,

    //Tresh
    SPELL_JUMP_VISUAL           = 184483,
    SPELL_FANATICS_SACRIFICE    = 209906,
    SPELL_RANCID_MAW_ROOT       = 205609, //Movement speed reduced by 80%.
    SPELL_FANATIC_SACRIFICE     = 209902,
    
    SPELL_HURLING_ROCKS         = 199245,
};

enum eEvents
{
    EVENT_RANCID_MAW            = 1,
    EVENT_TOXIC_WRETCH          = 2,
    EVENT_SUM_WORMSPEAKER       = 3,
    EVENT_PLR_OUT               = 4,
    //Heroic
    EVENT_CALL_ANGRY_CROWD      = 5,
};

enum Misc
{
    DATA_ACHIEVEMENT,  
};

Position const speakerPos[2] =
{
    {3045.07f, 1807.39f, -44.13f, 3.54f},
    {3048.89f, 1799.60f, -45.43f, 3.35f}
};

//91005
class boss_naraxas : public CreatureScript
{
public:
    boss_naraxas() : CreatureScript("boss_naraxas") {}

    struct boss_naraxasAI : public BossAI
    {
        boss_naraxasAI(Creature* creature) : BossAI(creature, DATA_NARAXAS), summons(me)
        {
            SetCombatMovement(false);
            me->SetMaxPower(POWER_MANA, 100);
            introDone = false;
            introDone1 = false;
            DoCast(me, SPELL_INTRO_MYSTIC, true);
            stacksdone = false;
        }

        void Initialize()
        {
            CheckTimer = 1000;
        }

        SummonList summons;
        bool berserkActive;
        bool introDone;
        bool introDone1;
        bool stacksdone;
        uint16 checkMeleeTimer;
        uint32 CheckTimer;
        ObjectGuid targetGUID;

        void Reset() override
        {
            _Reset();
            summons.DespawnAll();
            me->RemoveAurasDueToSpell(SPELL_GAIN_ENERGY);
            me->RemoveAurasDueToSpell(SPELL_FRENZY);
            me->RemoveAurasDueToSpell(SPELL_RAVENOUS);
            me->SetPower(POWER_MANA, 0);
            checkMeleeTimer = 2000;
            berserkActive = false;
            stacksdone = false;

            if (Creature* sum = me->SummonCreature(NPC_ANGRY_CROWD, 3034.80f, 1815.17f, -32.28f))
                sum->SetReactState(REACT_PASSIVE);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            DoCast(me, SPELL_GAIN_ENERGY, true);

            events.ScheduleEvent(EVENT_RANCID_MAW, 8000);
            events.ScheduleEvent(EVENT_TOXIC_WRETCH, 12000);
            events.ScheduleEvent(EVENT_SUM_WORMSPEAKER, 7000);

            //Heroic
            events.ScheduleEvent(EVENT_CALL_ANGRY_CROWD, 4000); //Не повторяется
        }

        void EnterEvadeMode(EvadeReason /*why*/) override
        {
            BossAI::EnterEvadeMode();

            /*if (me->GetVehicleKit())
            me->GetVehicleKit()->RemoveAllPassengers();*/
            
            if (Vehicle* naraxas = me->GetVehicleKit())
                    naraxas->RemoveAllPassengers();
        }

        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            summons.DespawnAll();

            if (Creature* target = me->FindNearestCreature(100700, 50, true))
                target->CastSpell(target, 208691); //conversation
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
            case DATA_ACHIEVEMENT:
                return stacksdone ? 1 : 0;
            }

            return 0;
        }
        
        void MoveInLineOfSight(Unit* who) override
        {  
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (!introDone && me->IsWithinDistInMap(who, 115.0f))
            {
                who->CastSpell(who, 209582, true);
                introDone = true;
            }

            if (!introDone1 && me->IsWithinDistInMap(who, 80.0f))
            {
                me->CastSpell(me, 209629, true);
                me->RemoveAurasDueToSpell(SPELL_INTRO_MYSTIC);
                if (Creature* target = me->FindNearestCreature(105766, 30, true))
                {
                    me->CastSpell(target, SPELL_INTRO_EMERGE);
                    target->SetVisible(false);
                }
                introDone1 = true;
            }
        }        

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*//*DamageEffectType dmgType*/) override
        {
            if (me->HealthBelowPct(21) && !berserkActive)
            {
                berserkActive = true;
                DoCast(me, SPELL_FRENZY, true);
            }
        }

        void SpellHit(Unit* caster, const SpellInfo* spell) override
        {
            switch (spell->Id)
            {
                case SPELL_TOXIC_WRETCH:
                {
                    Position pos;
                    float dist;
                    for (uint8 i = 0; i < 12; i++)
                    {
                        dist = frand(10, 20);
                        pos = me->GetNearPosition(dist, frand(-2.0f, 2.0f));
                        me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_TOXIC_WRETCH_AT, true);
                    }
                    break;
                }
                case SPELL_FANATIC_SACRIFICE: //Trash Serach Boss
                    DoCast(caster, SPELL_DEVOUR_FANATIC);
                    break;
                case SPELL_SPIKED_TONGUE:
                    DoCastVictim(SPELL_SPIKED_TONGUE_CHANNEL, true);
                    DoCast(me, SPELL_SPIKED_TONGUE_AT, true);
                    break;
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell) override
        {
            if (spell->Id == SPELL_CALL_ANGRY_CROWD)
                target->CastSpell(target, SPELL_HURLING_ROCKS, true);

            if (spell->Id == SPELL_SPIKED_TONGUE_CHECK_R) //Check Radius
            {
                if (me->IsInCombat() && me->GetDistance(target) < 5.0f)
                {
                    me->RemoveAurasDueToSpell(SPELL_SPIKED_TONGUE_CHANNEL);
                    //me->RemoveAreaObject(SPELL_SPIKED_TONGUE_AT); TIMUR
                    DoCast(target, SPELL_SPIKED_TONGUE_RIDE, true);
                }
            }
        }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
        {
            if (!me->IsInCombat() || !who->IsAlive())
                return;

            if (apply)
                DoCast(who, SPELL_SPIKED_TONGUE_DMG, true);
            else
            {
                if (who->GetTypeId() != TYPEID_PLAYER)
                    who->Kill(who);
                else
                {
                    targetGUID = who->GetGUID();
                    events.ScheduleEvent(EVENT_PLR_OUT, 200);
                }
                DoCast(me, SPELL_RAVENOUS, true);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(SPELL_SPIKED_TONGUE_CHANNEL))
                return;

            if (CheckTimer <= diff)
            {
                if (Aura const* aura = me->GetAura(199246))
                {
                    if (me->GetMap()->IsMythic())
                    {
                    if (aura->GetStackAmount() >= 6)
                    {
                        stacksdone = true;
                    }
                    }
                }
                CheckTimer = 1000;
            }else CheckTimer -= diff;

            if (checkMeleeTimer <= diff)
            {
                if (me->GetVictim())
                    if (!me->IsWithinMeleeRange(me->GetVictim()))
                        DoCast(SPELL_PUTRID_SKIES);

                checkMeleeTimer = 2000;
            }
            else
                checkMeleeTimer -= diff;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RANCID_MAW:
                        DoCast(SPELL_RANCID_MAW);
                        events.ScheduleEvent(EVENT_RANCID_MAW, 18000);
                        break;
                    case EVENT_TOXIC_WRETCH:
                        DoCast(SPELL_TOXIC_WRETCH);
                        events.ScheduleEvent(EVENT_TOXIC_WRETCH, 14000);
                        break;
                    case EVENT_SUM_WORMSPEAKER:
                        Talk(SAY_SUM);
                        for (uint8 i = 0; i < 2; i++)
                            me->SummonCreature(NPC_WORMSPEAKER_DEVOUT, speakerPos[i]);
                        events.ScheduleEvent(EVENT_SUM_WORMSPEAKER, 64000);
                        break;
                    case EVENT_PLR_OUT:
                    {
                        Position pos;
                        pos = me->GetNearPosition(20.0f, 0.0f);
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
                            target->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 10.0f, SPELL_SPIKED_TONGUE_JUMP, true);
                        break;
                    }
                    case EVENT_CALL_ANGRY_CROWD:
                        DoCast(SPELL_CALL_ANGRY_CROWD);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_naraxasAI (creature);
    }
};

//101075
class npc_naraxas_wormspeaker_devout : public CreatureScript
{
public:
    npc_naraxas_wormspeaker_devout() : CreatureScript("npc_naraxas_wormspeaker_devout") {}

    struct npc_naraxas_wormspeaker_devoutAI : public ScriptedAI
    {
        npc_naraxas_wormspeaker_devoutAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() override
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            events.ScheduleEvent(EFFECT_1, 1000);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == EFFECT_MOTION_TYPE)
            {
                if (id == 0)
                {
                    me->RemoveAurasDueToSpell(SPELL_JUMP_VISUAL);
                    events.ScheduleEvent(EFFECT_2, 1000);
                }
            }
            if (type == POINT_MOTION_TYPE)
            {
                if (id == 1)
                    DoCast(me, SPELL_FANATICS_SACRIFICE, true);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() && me->IsInCombat())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EFFECT_1:
                    {
                        DoCast(me, SPELL_JUMP_VISUAL, true);
                        Position pos;
                        pos = me->GetNearPosition(15.0f, 0.0f);
                        me->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() - 13, 15.0f, 15.0f, 0);
                        break;
                    }
                    case EFFECT_2:
                        if (Unit* owner = me->GetOwner())
                        {
                            Position pos;
                            pos = owner->GetNearPosition(18, frand(-1.0f, 1.0f));
                            me->GetMotionMaster()->MovePoint(1, pos);
                        }
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_naraxas_wormspeaker_devoutAI(creature);
    }
};

//200086
class spell_naraxas_gain_energy : public SpellScriptLoader
{
public:
    spell_naraxas_gain_energy() : SpellScriptLoader("spell_naraxas_gain_energy") { }

    class spell_naraxas_gain_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_naraxas_gain_energy_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (caster->GetPower(POWER_MANA) >= 100)
            {
                caster->CastSpell(caster, SPELL_SPIKED_TONGUE);

                if (Creature* target = caster->FindNearestCreature(91005, 50, true))
                    target->AI()->Talk(SAY_EMOTE);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_naraxas_gain_energy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_naraxas_gain_energy_AuraScript();
    }
};

class achievement_cant_eat_just_one : public AchievementCriteriaScript
{
public:
    achievement_cant_eat_just_one() : AchievementCriteriaScript("achievement_cant_eat_just_one") { }

    bool OnCheck(Player* /*player*/, Unit* target) override
    {
        if (!target)
            return false;

        if (Creature* boss = target->ToCreature())
            if (boss->AI()->GetData(DATA_ACHIEVEMENT))
                return true;

        return false;
    }
};

void AddSC_boss_naraxas()
{
    new boss_naraxas();
    new npc_naraxas_wormspeaker_devout();
    new spell_naraxas_gain_energy();
    new achievement_cant_eat_just_one();
}