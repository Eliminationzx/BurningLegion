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

#ifndef __BATTLEGROUNDTK_H
#define __BATTLEGROUNDTK_H

#include "Battleground.h"

enum BG_TK_Sound
{
    BG_TK_SOUND_HORDE_ORB_PICKED_UP		= 8174,
    BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP	= 8212,
    BG_TK_SOUND_ORB_RETURNED			= 8192,
    BG_TK_SOUND_NEAR_VICTORY			= 8456
};

enum BG_TK_OrbState
{
    BG_TK_ORB_STATE_ON_BASE			= 0,
    BG_TK_ORB_STATE_ON_PLAYER		= 1
};

enum BG_TK_Score
{
    BG_TK_PLAYER_KILL_POINTS			= 10,
    BG_TK_CENTER_POINTS					= 6,
    BG_TK_INDOOR_POINTS					= 4,
    BG_TK_OUTDOOR_POINTS				= 2,
    BG_TK_WARNING_NEAR_VICTORY_SCORE    = 1400,
    BG_TK_MAX_TEAM_SCORE                = 1600
};

enum BG_TK_WorldStates
{
    BG_TK_RESOURCES_ALLIANCE		= 6303,
    BG_TK_RESOURCES_HORDE			= 6304,
};

enum BG_TK_ObjectTypes
{
    BG_TK_OBJECT_ORB_BLUE		= 0,
    BG_TK_OBJECT_ORB_PURPLE		= 1,
    BG_TK_OBJECT_ORB_GREEN		= 2,
    BG_TK_OBJECT_ORB_ORANGE		= 3,
    BG_TK_OBJECT_DOOR_A			= 4,
    BG_TK_OBJECT_DOOR_H			= 5,
    BG_TK_OBJECT_MAX			= 6
};

enum BG_TK_CreatureTypes
{
    TK_SPIRIT_MAIN_ALLIANCE	= 0,
    TK_SPIRIT_MAIN_HORDE	= 1,

    BG_CREATURES_MAX_TK		= 2
};

enum BG_TK_SpellId
{
    BG_TK_AURA_ORB_BLUE		= 121164,
    BG_TK_AURA_ORB_PURPLE	= 121175,
    BG_TK_AURA_ORB_GREEN	= 121176,
    BG_TK_AURA_ORB_ORANGE	= 121177
};

enum BG_TK_GameObjectId
{
    BG_OBJECT_DOOR_TK_A			= 400424,
    BG_OBJECT_DOOR_TK_H			= 400425,
    BG_OBJECT_TK_ORB_BLUE		= 400420,
    BG_OBJECT_TK_ORB_PURPLE		= 400421,
    BG_OBJECT_TK_ORB_GREEN		= 400422,
    BG_OBJECT_TK_ORB_ORANGE		= 400423
};

#define BG_TK_MAX_ORBS  4

const uint32 BG_TK_GraveyardIds[2] = {3552, 3553};

class BattlegroundTKScore final : public BattlegroundScore
{
    public:
        BattlegroundTKScore(ObjectGuid playerGuid, uint32 team): BattlegroundScore(playerGuid, team), OrbPossesions(0), PointsScored(0) {};

        void BuildPvPLogPlayerDataPacket(WorldPackets::Battleground::PVPLogData::PlayerData& playerData) const override
        {
            BattlegroundScore::BuildPvPLogPlayerDataPacket(playerData);

            playerData.Stats.push_back(OrbPossesions);
            playerData.Stats.push_back(PointsScored);
        }

        uint32 GetAttr1() const final override { return OrbPossesions; }
        uint32 GetAttr2() const final override { return PointsScored; }

        uint32 OrbPossesions;
        uint32 PointsScored;
};

class BattlegroundTK : public Battleground
{
    public:
        /*Construction*/
        BattlegroundTK();
        ~BattlegroundTK();

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* player) override;
        void StartingEventCloseDoors() override;
        void StartingEventOpenDoors() override;

         /* Battleground Events */
        void EventPlayerClickedOnFlag(Player* player, GameObject* target_obj) override;

        void RemovePlayer(Player* player, ObjectGuid guid, uint32 team) override;
        void HandleKillPlayer(Player* player, Player* killer) override;
        bool SetupBattleground() override;
        void Reset() override;
        void EndBattleground(uint32 winner) override;
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player) override;

        /* BG Orbs*/
        ObjectGuid GetOrbPickerGUID(int32 team) const
        {
            if (team == TEAM_ALLIANCE || team == TEAM_HORDE)
                return m_orbOwners[team];
            return ObjectGuid::Empty;
        }

        void SetOrbPicker(ObjectGuid guid, uint32 orb)     { m_orbOwners[orb] = guid; }
        bool IsAllianceOrbPickedup() const         { return m_orbOwners[TEAM_ALLIANCE] != ObjectGuid::Empty; }
        bool IsHordeOrbPickedup() const            { return m_orbOwners[TEAM_HORDE] != ObjectGuid::Empty; }
        bool bgEnd;
        uint8 GetOrbState(uint32 orb)             { return _orbState[orb]; }
        void RespawnOrbAfterDrop(uint32 orb);
        bool HasAnOrb(Player* player);

        uint32 GetPrematureWinner() override;
        void UpdateScore(uint16 team, int16 points);
        bool UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor = true) override;
        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
        /* Scorekeeping */
        void AddPoint(uint32 TeamID, uint32 Points)     { m_Team_Scores[GetTeamIndexByTeamId(TeamID)] += Points; }

    private:
        ObjectGuid m_orbOwners[4];                        // 0 = orb 1, 1 = orb 2, 2 = orb 3, 3 = orb 4 (for player's guids)
        uint8 _orbState[4];                               // for checking orb state (on player, on base)

        int32 m_Team_Scores[2];
        bool m_IsInformedNearVictory;

        int32 pointsTimer;
        uint32 m_HonorWinKills;
        uint32 m_HonorEndKills;
        uint32 _minutesElapsed;

        void PostUpdateImpl(uint32 diff) override;
        void CalculatePoints(uint32 diff);
};

#endif
