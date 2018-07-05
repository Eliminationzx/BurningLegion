/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
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

#ifndef __BattleGroundTK_H
#define __BattleGroundTK_H

#include "Battleground.h"
#include "ObjectAccessor.h"

enum BG_TK_NPC
{
    BG_SM_NPC_POWERBALL         = 29265
};

#define BG_TK_MAX_TEAM_SCORE        1500
#define BG_TK_ORB_POINTS_MAX        1500
#define BG_TK_POINTS_UPDATE_TIME    (5*IN_MILLISECONDS)
#define BG_TK_EVENT_START_BATTLE    8563

enum BG_TK_Objects
{
    BG_TK_OBJECT_A_DOOR         = 0,
    BG_TK_OBJECT_H_DOOR         = 1,
    BG_TK_OBJECT_ORB_1          = 2,
    BG_TK_OBJECT_ORB_2          = 3,
    BG_TK_OBJECT_ORB_3          = 4,
    BG_TK_OBJECT_ORB_4          = 5,
    BG_TK_OBJECT_MAX            = 6
};

enum BG_TK_Creatures
{
    BG_TK_CREATURE_ORB_AURA_1   = 0,
    BG_TK_CREATURE_ORB_AURA_2   = 1,
    BG_TK_CREATURE_ORB_AURA_3   = 2,
    BG_TK_CREATURE_ORB_AURA_4   = 3,

    BG_TK_CREATURE_SPIRIT_1     = 4,
    BG_TK_CREATURE_SPIRIT_2     = 5,

    BG_TK_CREATURE_MAX          = 6
};

enum BG_TK_Objets_Entry
{
    BG_TK_OBJECT_DOOR_ENTRY     = 213172,

    BG_TK_OBJECT_ORB_1_ENTRY    = 212091,
    BG_TK_OBJECT_ORB_2_ENTRY    = 212092,
    BG_TK_OBJECT_ORB_3_ENTRY    = 212093,
    BG_TK_OBJECT_ORB_4_ENTRY    = 212094
};

enum BG_TK_Sound
{
    BG_TK_SOUND_ORB_PLACED      = 8232,
    BG_TK_SOUND_A_ORB_PICKED_UP = 8174,
    BG_TK_SOUND_H_ORB_PICKED_UP = 8174,
    BG_TK_SOUND_ORB_RESPAWNED   = 8232
};

enum BG_TK_SpellId
{
    BG_TK_SPELL_ORB_PICKED_UP_1 = 121175,   // PURPLE
    BG_TK_SPELL_ORB_PICKED_UP_2 = 121177,   // ORANGE
    BG_TK_SPELL_ORB_PICKED_UP_3 = 121176,   // GREEN
    BG_TK_SPELL_ORB_PICKED_UP_4 = 121164,   // YELLOW

    BG_TK_SPELL_ORB_AURA_1      = 121219,   // PURPLE
    BG_TK_SPELL_ORB_AURA_2      = 121221,   // ORANGE
    BG_TK_SPELL_ORB_AURA_3      = 121220,   // GREEN
    BG_TK_SPELL_ORB_AURA_4      = 121217,   // YELLOW

    BG_TK_ALLIANCE_INSIGNIA     = 131527,
    BG_TK_HORDE_INSIGNIA        = 131528
};

enum BG_TK_WorldStates
{
    BG_TK_ICON_A                = 6308,
    BG_TK_ICON_H                = 6307,
    BG_TK_ORB_POINTS_A          = 6303,
    BG_TK_ORB_POINTS_H          = 6304,
    BG_TK_ORB_STATE             = 6309
};

enum BG_TK_Graveyards
{
    TK_GRAVEYARD_RECTANGLEA1    = 3552,
    TK_GRAVEYARD_RECTANGLEA2    = 4058,
    TK_GRAVEYARD_RECTANGLEH1    = 3553,
    TK_GRAVEYARD_RECTANGLEH2    = 4057
};

enum BG_TK_ZONE
{
    TK_ZONE_OUT                 = 0,
    TK_ZONE_IN                  = 1,
    TK_ZONE_MIDDLE              = 2,
    TK_ZONE_MAX                 = 3
};


class BattleGroundTKScore final : public BattlegroundScore
{
    friend class BattlegroundTK;
    
    protected:
        BattleGroundTKScore(ObjectGuid playerGuid, uint32 team) : BattlegroundScore(playerGuid, team), OrbHandles(0), Score(0) {}
        
    void BuildPvPLogPlayerDataPacket(WorldPackets::Battleground::PVPLogData::PlayerData& playerData) const override
    {
        BattlegroundScore::BuildPvPLogPlayerDataPacket(playerData);

        playerData.Stats.push_back(OrbHandles);
        playerData.Stats.push_back(Score);
    }

    uint32 GetAttr1() const final override { return OrbHandles; }
    uint32 GetAttr2() const final override { return Score; }
 
    uint32 OrbHandles;
    uint32 Score;
};

enum BG_TK_Events
{
    TK_EVENT_ORB                  = 0,
    // spiritguides will spawn (same moment, like TP_EVENT_DOOR_OPEN)
    TK_EVENT_SPIRITGUIDES_SPAWN   = 2
};

#define MAX_ORBS                    4

const float BG_TK_DoorPositions[2][4] =
{
    {1783.84f, 1100.66f, 20.60f, 1.625020f},
    {1780.15f, 1570.22f, 24.59f, 4.711630f}
};

const float BG_TK_OrbPositions[MAX_ORBS][4] =
{
    {1850.26f, 1416.77f, 13.5709f, 1.56061f},   ///< PURPLE
    {1850.29f, 1250.31f, 13.5708f, 4.70848f},   ///< ORANGE
    {1716.78f, 1416.64f, 13.5709f, 1.57239f},   ///< GREEN
    {1716.83f, 1249.93f, 13.5706f, 4.71397f}    ///< YELLOW
};

const float BG_TK_SpiritPositions[MAX_ORBS][4] =
{
    {1892.61f, 1151.69f, 14.7160f, 2.523528f},
    {1672.40f, 1524.10f, 16.7387f, 6.032206f},
};

const static char* s_OrbColor[MAX_ORBS] =
{
    "00660099",   ///< PURPLE
    "00ED7F10",   ///< ORANGE
    "003A9D23",   ///< GREEN
    "00318CE7"    ///< BLEU
};

const static uint32 BG_TK_ORBS_SPELLS[MAX_ORBS] =
{
    BG_TK_SPELL_ORB_PICKED_UP_1,
    BG_TK_SPELL_ORB_PICKED_UP_2,
    BG_TK_SPELL_ORB_PICKED_UP_3,
    BG_TK_SPELL_ORB_PICKED_UP_4
};

const static uint32 BG_TK_ORBS_AURA[MAX_ORBS] =
{
    BG_TK_SPELL_ORB_AURA_1,
    BG_TK_SPELL_ORB_AURA_2,
    BG_TK_SPELL_ORB_AURA_3,
    BG_TK_SPELL_ORB_AURA_4
};

const static uint32 s_OrbsWorldStates[MAX_ORBS] =
{
    6715,
    6717,
    6716,
    6714
};

//tick point according to which zone
const static uint32 BG_TK_TickPoints[3] = { 3, 4, 5 };

class BattlegroundTK : public Battleground
{
    friend class BattleGroundMgr;

    public:
        /* Construction */
        BattlegroundTK();
        ~BattlegroundTK();
        void PostUpdateImpl(uint32 diff) override;

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* plr) override;
        void StartingEventCloseDoors() override;
        void StartingEventOpenDoors() override;

        /* Battleground Events */
        void EventPlayerDroppedOrb(Player* source);

        void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj) override;

        void RemovePlayer(Player* plr, ObjectGuid guid, uint32 team) override;
        void HandleAreaTrigger(Player* source, uint32 trigger, bool entered) override;
        void HandleKillPlayer(Player* player, Player* killer) override;
        bool SetupBattleground() override;
        void Reset() override;
        void EndBattleground(uint32 winner) override;
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player) override;

        void UpdateOrbState(Team team, uint32 value);
        void UpdateTeamScore(Team team);
        bool UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor = true) override;
        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;

        /* Scorekeeping */
        uint32 GetTeamScore(uint32 p_TeamID) const { return m_TeamScores[GetTeamIndexByTeamId(p_TeamID)]; }
        uint32 GetMaxScore() const { return BG_TK_MAX_TEAM_SCORE; }
        bool IsScoreIncremental() const { return true; }

        void AddPoint(Team team, uint32 Points = 1)     { m_TeamScores[GetTeamIndexByTeamId(team)] += Points; }
        void SetTeamPoint(Team team, uint32 Points = 0) { m_TeamScores[GetTeamIndexByTeamId(team)] = Points; }
        void RemovePoint(Team team, uint32 Points = 1)  { m_TeamScores[GetTeamIndexByTeamId(team)] -= Points; }

        void AccumulateScore(uint32 team, BG_TK_ZONE zone);
        ObjectGuid GetOrbKeeper(uint8 p_OrbIndex) const
        {
            if (p_OrbIndex < MAX_ORBS)
                return m_OrbKeepers[p_OrbIndex];
            return ObjectGuid::Empty;
        }

    private:

        std::set<ObjectGuid> const GetFlagPickersGUID(int32 p_Team) const
        {
            if (p_Team != TEAM_ALLIANCE && p_Team != TEAM_HORDE)
                return std::set<ObjectGuid>();

            std::set<ObjectGuid> l_FlagPickers;
            for (int l_I = 0; l_I < MAX_ORBS; l_I++)
            {
                if (m_OrbKeepers[l_I] != ObjectGuid::Empty)
                {
                    Player* l_Picker = ObjectAccessor::FindPlayer(m_OrbKeepers[l_I]);
                    if (l_Picker != nullptr && l_Picker->GetTeamId() == p_Team)
                        l_FlagPickers.insert(m_OrbKeepers[l_I]);
                }
            }

            return l_FlagPickers;
        }

        ObjectGuid m_OrbKeepers[MAX_ORBS];
        std::map<ObjectGuid, BG_TK_ZONE> m_playersZone;

        uint32 m_ReputationCapture;
        uint32 m_HonorWinKills;
        uint32 m_HonorEndKills;
        uint32 m_UpdatePointsTimer;
        Team   m_LastCapturedOrbTeam;
        int32 m_CheatersChecTKimer;
};

#endif
