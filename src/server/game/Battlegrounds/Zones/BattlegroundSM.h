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

#ifndef __BATTLEGROUNDSM_H
#define __BATTLEGROUNDSM_H

#include "Language.h"
#include "Battleground.h"

enum BG_SM_WorldStates
{
    SM_MINE_CARTS_DISPLAY			= 6436,
    SM_ALLIANCE_RESOURCES           = 6437,
    SM_HORDE_RESOURCES              = 6438,
    SM_MINE_CART_1					= 6439,
    SM_MINE_CART_2					= 6440,
    SM_MINE_CART_3					= 6441,
    SM_DISPLAY_ALLIANCE_RESSOURCES	= 6442,
    SM_DISPLAY_HORDE_RESSOURCES		= 6443,
    SM_DISPLAY_PROGRESS_BAR			= 6875, // 0 = false, 1 = true
    SM_PROGRESS_BAR_STATUS			= 6876, // 0 = Horde max, 50 = Neutral, 100 = Alliance max
    SM_UNK							= 6877
};

enum BG_SM_MineCarts
{
    BG_SM_MINE_CART_1		= 1,
    BG_SM_MINE_CART_2		= 2,
    BG_SM_MINE_CART_3		= 3
};

enum BG_SM_ProgressBarConsts
{
    BG_SM_PROGRESS_BAR_DONT_SHOW			= 0,
    BG_SM_PROGRESS_BAR_SHOW					= 1,
    BG_SM_PROGRESS_BAR_NEUTRAL				= 50
};

enum BG_SM_Sounds
{
    BG_SM_SOUND_MINE_CART_CAPTURED_HORDE		= 8213,
    BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE		= 8173,
    BG_SM_SOUND_NEAR_VICTORY					= 8456,
    BG_SM_SOUND_MINE_CART_SPAWNED				= 8174
};

enum BG_SM_Spells
{
    BG_SM_CONTROL_VISUAL_ALLIANCE	= 116086,
    BG_SM_CONTROL_VISUAL_HORDE		= 116085,
    BG_SM_CONTROL_VISUAL_NEUTRAL	= 118001,
    BG_SM_TRACK_SWITCH_OPENED		= 120228,
    BG_SM_TRACK_SWITCH_CLOSED		= 120229,
    BG_SM_FEIGN_DEATH_STUN			= 135781, // Prevent from turn moves
    BG_SM_PREVENTION_AURA			= 135846, // Prevent from using multiple times track switches
};

enum SMBattlegroundObjectEntry
{
    BG_SM_MINE_DEPOT			= 400433,
    BG_SM_DOOR					= 400434
};

enum SMBattlegroundGaveyards
{
    SM_GRAVEYARD_MAIN_ALLIANCE     = 4062,
    SM_GRAVEYARD_MAIN_HORDE        = 4061
};

enum SMBattlegroundCreaturesTypes
{
    SM_SPIRIT_ALLIANCE		= 0,
    SM_SPIRIT_HORDE			= 1,
    SM_MINE_CART_TRIGGER	= 2,
    SM_TRACK_SWITCH_EAST	= 3,
    SM_TRACK_SWITCH_NORTH	= 4,

    BG_SM_CREATURES_MAX		= 5
};

enum SMDepots
{
    SM_WATERFALL_DEPOT,
    SM_LAVA_DEPOT,
    SM_DIAMOND_DEPOT,
    SM_TROLL_DEPOT
};

enum SMTracks
{
    SM_EAST_TRACK_SWITCH,
    SM_NORTH_TRACK_SWITCH,
    SM_MAX_TRACK_SWITCH
};

enum SMBattlegroundObjectTypes
{
    BG_SM_OBJECT_DOOR_A_1					= 0,
    BG_SM_OBJECT_DOOR_H_1					= 1,
    BG_SM_OBJECT_DOOR_A_2					= 2,
    BG_SM_OBJECT_DOOR_H_2					= 3,
    BG_SM_OBJECT_WATERFALL_DEPOT			= 4,
    BG_SM_OBJECT_LAVA_DEPOT					= 5,
    BG_SM_OBJECT_DIAMOND_DEPOT				= 6,
    BG_SM_OBJECT_TROLL_DEPOT				= 7,
    BG_SM_OBJECT_BERSERKING_BUFF_EAST		= 8,
    BG_SM_OBJECT_BERSERKING_BUFF_WEST		= 9,
    BG_SM_OBJECT_RESTORATION_BUFF_WATERFALL	= 10,
    BG_SM_OBJECT_RESTORATION_BUFF_LAVA		= 11,

    BG_SM_OBJECT_MAX						= 12
};

enum BG_SM_Score
{
    BG_SM_WARNING_NEAR_VICTORY_SCORE    = 1400,
    BG_SM_MAX_TEAM_SCORE                = 1600
};

enum SMBattlegroundMineCartState
{
    SM_MINE_CART_CONTROL_NEUTRAL	= 0,
    SM_MINE_CART_CONTROL_ALLIANCE	= 1,
    SM_MINE_CART_CONTROL_HORDE		= 2
};

enum BG_SM_CreatureIds
{
    NPC_TRACK_SWITCH_EAST	= 60283,
    NPC_TRACK_SWITCH_NORTH	= 60309,
    NPC_MINE_CART_1			= 60378,
    NPC_MINE_CART_2			= 60379,
    NPC_MINE_CART_3			= 60380,
    NPC_MINE_CART_TRIGGER	= 400464
};

enum BG_SM_Paths
{
    SM_EAST_PATH,
    SM_NORTH_PATH
};

const float BG_SM_BuffPos[4][4] =
{
    {749.444153f, 64.338188f, 369.535797f, 6.058259f},   // Berserking buff East
    {789.979431f, 281.883575f, 355.389984f, 0.652173f},  // Berserking buff West
    {539.873596f, 396.386749f, 345.722412f, 3.994188f},  // Restoration buff Waterfall
    {614.202698f, 120.924660f, 294.430908f, 4.241807f}   // Restoration buff Lava
};

const float BG_SM_DepotPos[4][4] =
{
    {566.950989f, 337.05801f, 347.295013f, 1.559089f},   // Waterfall
    {619.469971f, 79.719597f, 299.067993f, 1.625564f},   // Lava
    {895.974426f, 27.210802f, 364.390991f, 3.445790f},   // Diamond
    {778.444946f, 500.949707f, 359.738983f, 0.737040f}   // Troll
};

const float BG_SM_DoorPos[4][4] =
{
    {852.0289952f, 158.216003f, 326.761011f, 0.150758f}, // Alliance 1
    {830.092102f, 143.925507f, 326.5f, 3.130245f},       // Alliance 2
    {652.177612f, 228.493423f, 326.917480f, 0.163844f},  // Horde 1
    {635.622925f, 208.220886f, 326.648315f, 3.717332f}   // Horde 2
};

const float BG_SM_TrackPos[2][4] =
{
    {715.585388f, 101.272034f, 319.994690f, 4.647377f}, // East
    {847.481689f, 308.032562f, 346.573242f, 0.587086f}  // North
};

#define MINE_CART_AT_DEPOT_POINTS		200
#define POINTS_PER_MINE_CART			150
#define DISPLAY_WORLDSTATE				1
#define BG_SM_NotSMWeekendHonorTicks    260
#define BG_SM_SMWeekendHonorTicks       160
#define SM_MINE_CART_MAX				3
#define SM_MAX_PATHS					2

struct BattlegroundSMScore final : public BattlegroundScore
{
    BattlegroundSMScore(ObjectGuid playerGuid, uint32 team) : BattlegroundScore(playerGuid, team), MineCartCaptures(0) { }

    void BuildPvPLogPlayerDataPacket(WorldPackets::Battleground::PVPLogData::PlayerData& playerData) const override
    {
        BattlegroundScore::BuildPvPLogPlayerDataPacket(playerData);

        playerData.Stats.push_back(MineCartCaptures);
    }

    uint32 GetAttr1() const final override { return MineCartCaptures; }

    uint32 MineCartCaptures;
};

class BattlegroundSM : public Battleground
{
    public:
        BattlegroundSM();
        ~BattlegroundSM();

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* player) override;
        void StartingEventCloseDoors() override;
        void StartingEventOpenDoors() override;

        void HandleKillPlayer(Player* player, Player* killer) override;
        bool SetupBattleground() override;
        void UpdateTeamScore(uint32 Team);
        void EndBattleground(uint32 winner) override;
        bool UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor = true) override;
        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;

        /* Battleground Events */
        void EventReopenDepot(uint32 diff);

        uint32 GetPrematureWinner() override;
    private:
        void PostUpdateImpl(uint32 diff) override;

        void EventTeamCapturedMineCart(uint32 team, uint8 mineCart);
        void UpdatePointsCount(uint32 Team);
        void SummonMineCart(uint32 diff);
        void FirstMineCartSummon(uint32 diff);
        void MineCartsMoves(uint32 diff);
        void CheckPlayerNearMineCart(uint32 diff);
        void CheckMineCartNearDepot(uint32 diff);
        void MineCartAddPoints(uint32 diff);
        void ResetDepotsAndMineCarts(uint8 depot, uint8 mineCart);
        void CheckTrackSwitch(uint32 diff);

        /* Scorekeeping */
        void AddPoints(uint32 Team, uint32 Points);

        void RemovePoint(uint32 TeamID, uint32 Points = 1) { m_TeamScores[GetTeamIndexByTeamId(TeamID)] -= Points; }
        void SetTeamPoint(uint32 TeamID, uint32 Points = 0) { m_TeamScores[GetTeamIndexByTeamId(TeamID)] = Points; }
        
        uint32 GetMineCartTeamKeeper(uint8 mineCart);
        uint32 m_HonorScoreTics[2];

        uint32 m_MineCartsTrigger[SM_MINE_CART_MAX];
        int32 m_MineCartsProgressBar[SM_MINE_CART_MAX];
        uint32 m_MineCartTeamKeeper[SM_MINE_CART_MAX]; // keepers team
        int32 m_MineCartSpawnTimer;
        int32 m_FirstMineCartSummonTimer;
        int32 m_MineCartCheckTimer;
        int32 m_DepotCloseTimer[4];
        int32 m_MineCartAddPointsTimer;
        int32 m_TrackSwitchClickTimer[SM_MAX_TRACK_SWITCH];
        bool m_Depot[4]; // 0 = Waterfall, 1 = Lava, 2 = Diamond, 3 = Troll
        bool m_MineCartReachedDepot[SM_MINE_CART_MAX];
        bool m_MineCartNearDepot[SM_MINE_CART_MAX];
        bool m_MineCartSpawned[SM_MINE_CART_MAX];
        bool m_FirstMineCartSpawned;
        bool m_PathDone[SM_MINE_CART_MAX - 1][SM_MAX_PATHS]; // Only for first and third mine cart
        bool m_WaterfallPathDone; // Waterfall path
        bool m_TrackSwitch[SM_MAX_TRACK_SWITCH]; // East : true = open, false = close | North : true = close, false = open
        bool m_TrackSwitchCanInterract[SM_MAX_TRACK_SWITCH];

        uint32 m_HonorTics;
        bool m_IsInformedNearVictory;
};
#endif