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

#include "BattlegroundSM.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldStatePackets.h"
#include "BattlegroundMgr.h"
#include "Creature.h"
#include "CreatureAIImpl.h"
#include "Language.h"
#include "Object.h"
#include "Player.h"
#include "Util.h"
#include "GossipDef.h"
#include "MotionMaster.h"

// these variables aren't used outside of this file, so declare them only here
uint32 BG_SM_HonorScoreTicks[BG_HONOR_MODE_NUM] =
{
    260, // normal honor
    160  // holiday
};

BattlegroundSM::BattlegroundSM()
{
    m_BuffChange = true;
    BgObjects.resize(BG_SM_OBJECT_MAX);
    BgCreatures.resize(BG_SM_CREATURES_MAX);

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_SM_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SM_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_SM_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SM_HAS_BEGUN;

    m_TeamScores[TEAM_ALLIANCE] = 0;
    m_TeamScores[TEAM_HORDE] = 0;
    m_HonorScoreTics[TEAM_ALLIANCE] = 0;
    m_HonorScoreTics[TEAM_HORDE] = 0;
    m_MineCartCheckTimer = 1000;
    m_FirstMineCartSummonTimer = 110 * IN_MILLISECONDS; // 10 sec before opening doors
    bool isBGWeekend = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTics = (isBGWeekend) ? BG_SM_SMWeekendHonorTicks : BG_SM_NotSMWeekendHonorTicks;
    m_IsInformedNearVictory = false;
    m_MineCartSpawnTimer = 30 * IN_MILLISECONDS;
    m_MineCartAddPointsTimer = 2000;
    m_FirstMineCartSpawned = false;
    m_WaterfallPathDone = false;
    m_TrackSwitch[SM_EAST_TRACK_SWITCH] = true;
    m_TrackSwitch[SM_NORTH_TRACK_SWITCH] = false;
    m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] = 3000;
    m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] = 3000;
    m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = true;
    m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = true;

    for (uint8 i = 0; i < SM_MINE_CART_MAX; ++i)
    {
        m_MineCartsProgressBar[i] = BG_SM_PROGRESS_BAR_NEUTRAL;
        m_MineCartReachedDepot[i] = false;
        m_MineCartNearDepot[i] = false;
        m_MineCartSpawned[i] = false;
    }

    for (uint8 i = 0; i < 4; ++i)
    {
        m_Depot[i] = false;
        m_DepotCloseTimer[i] = 3 * IN_MILLISECONDS;
    }

    for (uint8 i = 0; i < SM_MAX_PATHS; ++i)
        m_PathDone[i][i] = false;
}

BattlegroundSM::~BattlegroundSM() { }

void BattlegroundSM::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        BattlegroundSM::SummonMineCart(diff);
        BattlegroundSM::CheckPlayerNearMineCart(diff);
        BattlegroundSM::CheckMineCartNearDepot(diff);
        BattlegroundSM::EventReopenDepot(diff);
        BattlegroundSM::MineCartAddPoints(diff);
        BattlegroundSM::CheckTrackSwitch(diff);
    }

    if (!m_FirstMineCartSpawned)
        BattlegroundSM::FirstMineCartSummon(diff);

    if (Creature* trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
    {
        if (trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
            m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] = true;
        else
        {
            m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] = false;
            m_PathDone[SM_EAST_PATH][0] = false;
            m_PathDone[SM_EAST_PATH][1] = false;
        }

        if (trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f, true))
            m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] = true;
        else
        {
            m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] = false;
            m_WaterfallPathDone = false;
        }

        if (trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
            m_MineCartSpawned[BG_SM_MINE_CART_3 - 1] = true;
        else
        {
            m_MineCartSpawned[BG_SM_MINE_CART_3 - 1] = false;
            m_PathDone[SM_NORTH_PATH][0] = false;
            m_PathDone[SM_NORTH_PATH][1] = false;
        }
    }

    if (!m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH])
    {
        if (m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] <= 0)
        {
            if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_EAST]))
            {
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetExactDist2d(track->GetPositionX(), track->GetPositionY()) <= 10.0f)
                            player->PlayerTalkClass->SendCloseGossip(); // Prevent from using multiple times track switches

                track->RemoveAurasDueToSpell(BG_SM_PREVENTION_AURA);
                m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = true;
            }
        }
        else m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] -= diff;
    }

    if (!m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH])
    {
        if (m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] <= 0)
        {
            if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_NORTH]))
            {
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetExactDist2d(track->GetPositionX(), track->GetPositionY()) <= 10.0f)
                            player->PlayerTalkClass->SendCloseGossip(); // Prevent from using multiple times track switches

                track->RemoveAurasDueToSpell(BG_SM_PREVENTION_AURA);
                m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = true;
            }
        }
        else m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] -= diff;
    }

    BattlegroundSM::MineCartsMoves(diff);
}

void BattlegroundSM::StartingEventCloseDoors()
{
    SpawnBGObject(BG_SM_OBJECT_DOOR_A_1, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_SM_OBJECT_DOOR_H_1, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_SM_OBJECT_DOOR_A_2, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_SM_OBJECT_DOOR_H_2, RESPAWN_IMMEDIATELY);

    for (uint8 i = BG_SM_OBJECT_WATERFALL_DEPOT; i < BG_SM_OBJECT_MAX; ++i)
        SpawnBGObject(i, RESPAWN_ONE_DAY);
}

void BattlegroundSM::CheckTrackSwitch(uint32 diff)
{
    Creature* trigger = NULL;

    if (m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH])
    {
        if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
        {
            if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_EAST]))
            {
                if (track->HasAura(BG_SM_TRACK_SWITCH_OPENED) && !m_TrackSwitch[SM_EAST_TRACK_SWITCH])
                {
                    SendMessageToAll(LANG_BG_SM_EAST_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] = 3000;
                    m_TrackSwitch[SM_EAST_TRACK_SWITCH] = true;
                    m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = false;
                }

                if (track->HasAura(BG_SM_TRACK_SWITCH_CLOSED) && m_TrackSwitch[SM_EAST_TRACK_SWITCH])
                {
                    SendMessageToAll(LANG_BG_SM_EAST_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] = 3000;
                    m_TrackSwitch[SM_EAST_TRACK_SWITCH] = false;
                    m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = false;
                }
            }
        }
    }

    if (m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH])
    {
        if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
        {
            if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_NORTH]))
            {
                if (track->HasAura(BG_SM_TRACK_SWITCH_CLOSED) && m_TrackSwitch[SM_NORTH_TRACK_SWITCH])
                {
                    SendMessageToAll(LANG_BG_SM_NORTH_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] = 3000;
                    m_TrackSwitch[SM_NORTH_TRACK_SWITCH] = false;
                    m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = false;
                }

                if (track->HasAura(BG_SM_TRACK_SWITCH_OPENED) && !m_TrackSwitch[SM_NORTH_TRACK_SWITCH])
                {
                    SendMessageToAll(LANG_BG_SM_NORTH_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] = 3000;
                    m_TrackSwitch[SM_NORTH_TRACK_SWITCH] = true;
                    m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = false;
                }
            }
        }
    }
}

void BattlegroundSM::FirstMineCartSummon(uint32 diff)
{
    if (m_FirstMineCartSummonTimer <= 0)
    {
        Creature* trigger = NULL;
        m_FirstMineCartSpawned = true;

        if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
        {
            if (uint8 mineCart = urand(BG_SM_MINE_CART_1, BG_SM_MINE_CART_3))
            {
                switch (mineCart)
                {
                    case BG_SM_MINE_CART_1:
                    {
                        if (trigger)
                        {
                            trigger->SummonCreature(NPC_MINE_CART_1, 744.542053f, 183.545883f, 319.658203f, 4.356342f);

                            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                                cart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                                cart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                cart->SetSpeed(MOVE_WALK, 0.8f);
                                m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
                            }
                        }
                        break;
                    }

                    case BG_SM_MINE_CART_2:
                    {
                        if (trigger)
                        {
                            trigger->SummonCreature(NPC_MINE_CART_2, 739.400330f, 203.598511f, 319.603333f, 2.308198f);

                            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                                cart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                                cart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                cart->SetSpeed(MOVE_WALK, 0.8f);
                                m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
                            }
                        }
                        break;
                    }

                    case BG_SM_MINE_CART_3:
                    {
                        if (trigger)
                        {
                            trigger->SummonCreature(NPC_MINE_CART_3, 760.184509f, 198.844742f, 319.446655f, 0.351249f);

                            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                                cart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                                cart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                cart->SetSpeed(MOVE_WALK, 0.8f);
                                m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
                            }
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
        }
    } else m_FirstMineCartSummonTimer -= diff;
}

void BattlegroundSM::StartingEventOpenDoors()
{
    for (uint8 i = BG_SM_OBJECT_DOOR_A_1; i < (BG_SM_OBJECT_DOOR_H_2 + 1); ++i)
        SpawnBGObject(i, RESPAWN_ONE_DAY);
    
    for (uint8 i = BG_SM_OBJECT_WATERFALL_DEPOT; i < BG_SM_OBJECT_MAX; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundSM::SummonMineCart(uint32 diff)
{
    if (m_MineCartSpawnTimer <= 0)
    {
        m_MineCartSpawnTimer = 30 * IN_MILLISECONDS;

        Creature* trigger = NULL;
        uint8 mineCart = 0;

        if (m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
            mineCart = 0;

        else if (!m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = RAND(BG_SM_MINE_CART_1, BG_SM_MINE_CART_2, BG_SM_MINE_CART_3);
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        else if (m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = RAND(BG_SM_MINE_CART_2, BG_SM_MINE_CART_3);
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        else if (!m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = RAND(BG_SM_MINE_CART_1, BG_SM_MINE_CART_3);
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        else if (!m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = RAND(BG_SM_MINE_CART_1, BG_SM_MINE_CART_2);
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        else if (m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = BG_SM_MINE_CART_3;
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        else if (m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            !m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = BG_SM_MINE_CART_2;
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        else if (!m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] &&
            m_MineCartSpawned[BG_SM_MINE_CART_3 - 1])
        {
            mineCart = BG_SM_MINE_CART_1;
            SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
        }

        if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
        {
            switch (mineCart)
            {
                case BG_SM_MINE_CART_1:
                {
                    if (trigger)
                    {
                        trigger->SummonCreature(NPC_MINE_CART_1, 744.542053f, 183.545883f, 319.658203f, 4.356342f);

                        if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f))
                        {
                            cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                            cart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                            cart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            cart->SetSpeed(MOVE_WALK, 0.8f);
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
                        }
                    }
                    break;
                }

                case BG_SM_MINE_CART_2:
                {
                    if (trigger)
                    {
                        trigger->SummonCreature(NPC_MINE_CART_2, 739.400330f, 203.598511f, 319.603333f, 2.308198f);

                        if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f))
                        {
                            cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                            cart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                            cart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            cart->SetSpeed(MOVE_WALK, 0.8f);
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
                        }
                    }
                    break;
                }

                case BG_SM_MINE_CART_3:
                {
                    if (trigger)
                    {
                        trigger->SummonCreature(NPC_MINE_CART_3, 760.184509f, 198.844742f, 319.446655f, 0.351249f);

                        if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f))
                        {
                            cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                            cart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                            cart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            cart->SetSpeed(MOVE_WALK, 0.8f);
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
                        }
                    }
                    break;
                }

                default:
                    break;
            }
        }
    } else m_MineCartSpawnTimer -= diff;
}

void BattlegroundSM::CheckPlayerNearMineCart(uint32 diff)
{
    if (m_MineCartCheckTimer <= 0)
    {
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (player->GetBGTeam() == ALLIANCE)
                {
                    if (Creature* cart = player->FindNearestCreature(NPC_MINE_CART_1, 22.0f, true))
                    {
                        player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] >= 100)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] = 100;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]++;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]);
                        }
                        
                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_ALLIANCE, true);
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else if (Creature* cart = player->FindNearestCreature(NPC_MINE_CART_2, 22.0f, true))
                    {
                        player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] >= 100)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] = 100;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]++;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_ALLIANCE, true);
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else if (Creature* cart = player->FindNearestCreature(NPC_MINE_CART_3, 22.0f, true))
                    {
                        player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] >= 100)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] = 100;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]++;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_ALLIANCE, true);
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_DONT_SHOW);
                }
                else // for GetBGTeam() == HORDE
                {
                    if (Creature* cart = player->FindNearestCreature(NPC_MINE_CART_1, 22.0f, true))
                    {
                        player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] <= 0)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] = 0;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]--;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_HORDE, true);
                                SendMessageToAll(LANG_BG_SM_HORDE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else if (Creature* cart = player->FindNearestCreature(NPC_MINE_CART_2, 22.0f, true))
                    {
                        player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] <= 0)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] = 0;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]--;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_HORDE, true);
                                SendMessageToAll(LANG_BG_SM_HORDE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else if (Creature* cart = player->FindNearestCreature(NPC_MINE_CART_3, 22.0f, true))
                    {
                        player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] <= 0)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] = 0;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]--;
                            player->SendUpdateWorldState(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]);
                        }
                        
                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                            {
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_HORDE, true);
                                SendMessageToAll(LANG_BG_SM_HORDE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, cart->GetGUID());

                            if (cart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                cart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, cart->GetGUID());

                            if (!cart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                cart->CastSpell(cart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else player->SendUpdateWorldState(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_DONT_SHOW);
                }

                    m_MineCartCheckTimer = 1000;
            }
        }
    } else m_MineCartCheckTimer -= diff;
}

void BattlegroundSM::CheckMineCartNearDepot(uint32 diff)
{
    Creature* trigger = NULL;
    if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
    {
        if (!m_MineCartNearDepot[BG_SM_MINE_CART_1 - 1])
        {
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
            {
                if (cart->GetExactDist2d(BG_SM_DepotPos[SM_LAVA_DEPOT][0], BG_SM_DepotPos[SM_LAVA_DEPOT][1]) <= 6.0f)
                {
                    m_Depot[SM_LAVA_DEPOT] = true;
                    BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), BG_SM_MINE_CART_1);
                    m_MineCartNearDepot[BG_SM_MINE_CART_1 - 1] = true;
                }

                if (cart->GetExactDist2d(BG_SM_DepotPos[SM_DIAMOND_DEPOT][0], BG_SM_DepotPos[SM_DIAMOND_DEPOT][1]) <= 6.0f)
                {
                    m_Depot[SM_DIAMOND_DEPOT] = true;
                    BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), BG_SM_MINE_CART_1);
                    m_MineCartNearDepot[BG_SM_MINE_CART_1 - 1] = true;
                }
            }
        }

        if (!m_MineCartNearDepot[BG_SM_MINE_CART_2 - 1])
        {
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f, true))
            {
                if (cart->GetExactDist2d(BG_SM_DepotPos[SM_WATERFALL_DEPOT][0], BG_SM_DepotPos[SM_WATERFALL_DEPOT][1]) <= 6.0f)
                {
                    m_Depot[SM_WATERFALL_DEPOT] = true;
                    BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_2), BG_SM_MINE_CART_2);
                    m_MineCartNearDepot[BG_SM_MINE_CART_2 - 1] = true;
                }
            }
        }

        if (!m_MineCartNearDepot[BG_SM_MINE_CART_3 - 1])
        {
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
            {
                if (cart->GetExactDist2d(BG_SM_DepotPos[SM_DIAMOND_DEPOT][0], BG_SM_DepotPos[SM_DIAMOND_DEPOT][1]) <= 6.0f)
                {
                    m_Depot[SM_DIAMOND_DEPOT] = true;
                    BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), BG_SM_MINE_CART_3);
                    m_MineCartNearDepot[BG_SM_MINE_CART_3 - 1] = true;
                }

                if (cart->GetExactDist2d(BG_SM_DepotPos[SM_TROLL_DEPOT][0], BG_SM_DepotPos[SM_TROLL_DEPOT][1]) <= 6.0f)
                {
                    m_Depot[SM_TROLL_DEPOT] = true;
                    BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), BG_SM_MINE_CART_3);
                    m_MineCartNearDepot[BG_SM_MINE_CART_3 - 1] = true;
                }
            }
        }
    }
}

void BattlegroundSM::EventTeamCapturedMineCart(uint32 team, uint8 mineCart)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
        {
            if (player->GetBGTeam() == team)
                if (player->FindNearestCreature(NPC_MINE_CART_1, 22.0f, true) ||
                    player->FindNearestCreature(NPC_MINE_CART_2, 22.0f, true) ||
                    player->FindNearestCreature(NPC_MINE_CART_3, 22.0f, true))
                {
                    UpdatePlayerScore(player, SCORE_CART_CONTROLLED, 1);
                    player->RewardHonor(player, 1, irand(10, 12));
                }
        }
    }

    Creature* trigger = NULL;
    if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
    {
        switch (mineCart)
        {
            case BG_SM_MINE_CART_1:
            {
                if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
                {
                    if (m_Depot[SM_LAVA_DEPOT])
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_LAVA_DEPOT]))
                        {
                            cart->StopMoving();
                            depot->UseDoorOrButton();
                            m_MineCartReachedDepot[BG_SM_MINE_CART_1 - 1] = true;
                        }
                    }

                    if (m_Depot[SM_DIAMOND_DEPOT])
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                        {
                            cart->StopMoving();
                            depot->UseDoorOrButton();
                            m_MineCartReachedDepot[BG_SM_MINE_CART_1 - 1] = true;
                        }
                    }
                }
                break;
            }

            case BG_SM_MINE_CART_2:
            {
                if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f, true))
                {
                    if (m_Depot[SM_WATERFALL_DEPOT])
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_WATERFALL_DEPOT]))
                        {
                            cart->StopMoving();
                            depot->UseDoorOrButton();
                            m_MineCartReachedDepot[BG_SM_MINE_CART_2 - 1] = true;
                        }
                    }
                }
                break;
            }

            case BG_SM_MINE_CART_3:
            {
                if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
                {
                    if (m_Depot[SM_DIAMOND_DEPOT])
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                        {
                            cart->StopMoving();
                            depot->UseDoorOrButton();
                            m_MineCartReachedDepot[BG_SM_MINE_CART_3 - 1] = true;
                        }
                    }

                    if (m_Depot[SM_TROLL_DEPOT])
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_TROLL_DEPOT]))
                        {
                            cart->StopMoving();
                            depot->UseDoorOrButton();
                            m_MineCartReachedDepot[BG_SM_MINE_CART_3 - 1] = true;
                        }
                    }
                }
                break;
            }
        }
    }
}

void BattlegroundSM::EventReopenDepot(uint32 diff)
{
    Creature* trigger = NULL;
    if (m_MineCartReachedDepot[BG_SM_MINE_CART_1 - 1])
    {
        if (m_Depot[SM_LAVA_DEPOT])
        {
            if (m_DepotCloseTimer[SM_LAVA_DEPOT] <= 0)
            {
                if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
                {
                    if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_LAVA_DEPOT]))
                        {
                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE)
                            {
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                            }

                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE)
                            {
                                SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                            }

                            if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                                BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), POINTS_PER_MINE_CART);

                            BattlegroundSM::ResetDepotsAndMineCarts(SM_LAVA_DEPOT, BG_SM_MINE_CART_1);
                            depot->ResetDoorOrButton();
                            cart->DespawnOrUnsummon();
                        }
                    }
                }
            } else m_DepotCloseTimer[SM_LAVA_DEPOT] -= diff;
        }

        if (m_Depot[SM_DIAMOND_DEPOT])
        {
            if (m_DepotCloseTimer[SM_DIAMOND_DEPOT] <= 0)
            {
                if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
                {
                    if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                        {
                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE)
                            {
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                            }

                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE)
                            {
                                SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                            }

                            if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                                BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), POINTS_PER_MINE_CART);
                            
                            BattlegroundSM::ResetDepotsAndMineCarts(SM_DIAMOND_DEPOT, BG_SM_MINE_CART_1);
                            depot->ResetDoorOrButton();
                            cart->DespawnOrUnsummon();
                        }
                    }
                }
            } else m_DepotCloseTimer[SM_DIAMOND_DEPOT] -= diff;
        }
    }

    if (m_MineCartReachedDepot[BG_SM_MINE_CART_2 - 1])
    {
        if (m_Depot[SM_WATERFALL_DEPOT])
        {
            if (m_DepotCloseTimer[SM_WATERFALL_DEPOT] <= 0)
            {
                if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
                {
                    if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f, true))
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_WATERFALL_DEPOT]))
                        {
                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == ALLIANCE)
                            {
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                            }

                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == HORDE)
                            {
                                SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                            }

                            if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                                BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_2), POINTS_PER_MINE_CART);
                            
                            BattlegroundSM::ResetDepotsAndMineCarts(SM_WATERFALL_DEPOT, BG_SM_MINE_CART_2);
                            depot->ResetDoorOrButton();
                            cart->DespawnOrUnsummon();
                        }
                    }
                }
            } else m_DepotCloseTimer[SM_WATERFALL_DEPOT] -= diff;
        }
    }

    if (m_MineCartReachedDepot[BG_SM_MINE_CART_3 - 1])
    {
        if (m_Depot[SM_DIAMOND_DEPOT])
        {
            if (m_DepotCloseTimer[SM_DIAMOND_DEPOT] <= 0)
            {
                if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
                {
                    if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                        {
                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
                            {
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                            }

                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
                            {
                                SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                            }

                            if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                                BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), POINTS_PER_MINE_CART);
                            
                            BattlegroundSM::ResetDepotsAndMineCarts(SM_DIAMOND_DEPOT, BG_SM_MINE_CART_3);
                            depot->ResetDoorOrButton();
                            cart->DespawnOrUnsummon();
                        }
                    }
                }
            } else m_DepotCloseTimer[SM_DIAMOND_DEPOT] -= diff;
        }

        if (m_Depot[SM_TROLL_DEPOT])
        {
            if (m_DepotCloseTimer[SM_TROLL_DEPOT] <= 0)
            {
                if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
                {
                    if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
                    {
                        if (GameObject* depot = HashMapHolder<GameObject>::Find(BgObjects[BG_SM_OBJECT_TROLL_DEPOT]))
                        {
                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
                            {
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                            }

                            if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
                            {
                                SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                                PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                            }

                            if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                                BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), POINTS_PER_MINE_CART);
                            
                            BattlegroundSM::ResetDepotsAndMineCarts(SM_TROLL_DEPOT, BG_SM_MINE_CART_3);
                            depot->ResetDoorOrButton();
                            cart->DespawnOrUnsummon();
                        }
                    }
                }
            } else m_DepotCloseTimer[SM_TROLL_DEPOT] -= diff;
        }
    }
}

uint32 BattlegroundSM::GetMineCartTeamKeeper(uint8 mineCart)
{
    if (m_MineCartsProgressBar[mineCart - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
        return ALLIANCE;

    if (m_MineCartsProgressBar[mineCart - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
        return HORDE;

    return 0;
}

void BattlegroundSM::MineCartAddPoints(uint32 diff)
{
    if (m_MineCartAddPointsTimer <= 0)
    {
        m_MineCartAddPointsTimer = 2 * IN_MILLISECONDS;

           /* ==================  ALLIANCE  ================== */
        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 5);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) != ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 3);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) != ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 3);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) != ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 3);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) != ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) != ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 2);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) != ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) != ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 2);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) != ALLIANCE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) != ALLIANCE)
            BattlegroundSM::AddPoints(ALLIANCE, 2);

            /* ==================  HORDE  ================== */
        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
            BattlegroundSM::AddPoints(HORDE, 5);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) != HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
            BattlegroundSM::AddPoints(HORDE, 3);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) != HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
            BattlegroundSM::AddPoints(HORDE, 3);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) != HORDE)
            BattlegroundSM::AddPoints(HORDE, 3);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) != HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) != HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
            BattlegroundSM::AddPoints(HORDE, 2);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) != HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) != HORDE)
            BattlegroundSM::AddPoints(HORDE, 2);

        else if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_2) != HORDE &&
            GetMineCartTeamKeeper(BG_SM_MINE_CART_3) != HORDE)
            BattlegroundSM::AddPoints(HORDE, 2);
    } else m_MineCartAddPointsTimer -= diff;
}

void BattlegroundSM::ResetDepotsAndMineCarts(uint8 depot, uint8 mineCart)
{
    m_DepotCloseTimer[depot] = 3000;
    m_Depot[depot] = false;
    m_MineCartsProgressBar[mineCart - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
    m_MineCartNearDepot[mineCart - 1] = false;
    m_MineCartReachedDepot[mineCart - 1] = false;
    m_MineCartSpawned[mineCart - 1] = false;
}

void BattlegroundSM::MineCartsMoves(uint32 diff)
{
    Creature* trigger = NULL;
    if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
    {
        if (!m_PathDone[SM_EAST_PATH][0])
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
            {
                cart->GetMotionMaster()->MovePath(NPC_MINE_CART_1, false);
                m_PathDone[SM_EAST_PATH][0] = true;
            }

        if (m_PathDone[SM_EAST_PATH][0] && !m_PathDone[SM_EAST_PATH][1])
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_1, 99999.0f, true))
                if (cart->GetExactDist2d(717.169312f, 114.258339f) < 0.5f) // East pos
                    if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_EAST]))
                        if (track->HasAura(BG_SM_TRACK_SWITCH_OPENED))
                        {
                            cart->GetMotionMaster()->Clear(true);
                            cart->GetMotionMaster()->MovePath(NPC_MINE_CART_1 * 10, false);
                            m_PathDone[SM_EAST_PATH][1] = true;
                        }
                        else
                        {
                            cart->GetMotionMaster()->Clear(true);
                            cart->GetMotionMaster()->MovePath(NPC_MINE_CART_1 * 100, false);
                            m_PathDone[SM_EAST_PATH][1] = true;
                        }

        if (!m_WaterfallPathDone)
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_2, 99999.0f, true))
            {
                cart->GetMotionMaster()->MovePath(NPC_MINE_CART_2, false);
                m_WaterfallPathDone = true;
            }

        if (!m_PathDone[SM_NORTH_PATH][0])
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
            {
                cart->GetMotionMaster()->MovePath(NPC_MINE_CART_3, false);
                m_PathDone[SM_NORTH_PATH][0] = true;
            }

        if (m_PathDone[SM_NORTH_PATH][0] && !m_PathDone[SM_NORTH_PATH][1])
            if (Creature* cart = trigger->FindNearestCreature(NPC_MINE_CART_3, 99999.0f, true))
                if (cart->GetExactDist2d(834.727234f, 299.809753f) < 0.5f) // North pos
                    if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_NORTH]))
                        if (track->HasAura(BG_SM_TRACK_SWITCH_CLOSED))
                        {
                            cart->GetMotionMaster()->Clear(true);
                            cart->GetMotionMaster()->MovePath(NPC_MINE_CART_3 * 10, false);
                            m_PathDone[SM_NORTH_PATH][1] = true;
                        }
                        else
                        {
                            cart->GetMotionMaster()->Clear(true);
                            cart->GetMotionMaster()->MovePath(NPC_MINE_CART_3 * 100, false);
                            m_PathDone[SM_NORTH_PATH][1] = true;
                        }
    }
}

void BattlegroundSM::AddPoints(uint32 Team, uint32 Points)
{
    TeamId team_index = GetTeamIndexByTeamId(Team);
    m_TeamScores[team_index] += Points;
    m_HonorScoreTics[team_index] += Points;
    if (m_HonorScoreTics[team_index] >= m_HonorTics)
    {
        RewardHonorToTeam(GetBonusHonorFromKill(1), Team);
        m_HonorScoreTics[team_index] -= m_HonorTics;
    }
    UpdateTeamScore(team_index);
}

void BattlegroundSM::UpdateTeamScore(uint32 Team)
{
    uint32 score = GetTeamScore(Team);

    if (!m_IsInformedNearVictory && score >= BG_SM_WARNING_NEAR_VICTORY_SCORE)
    {
        SendMessageToAll(Team == TEAM_ALLIANCE ? LANG_BG_SM_A_NEAR_VICTORY : LANG_BG_SM_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
        PlaySoundToAll(BG_SM_SOUND_NEAR_VICTORY);
        m_IsInformedNearVictory = true;
    }

    if (score >= BG_SM_MAX_TEAM_SCORE)
    {
        score = BG_SM_MAX_TEAM_SCORE;
        EndBattleground(Team == TEAM_ALLIANCE ? ALLIANCE : HORDE);
    }

    UpdateWorldState(Team == TEAM_ALLIANCE ? SM_ALLIANCE_RESOURCES : SM_HORDE_RESOURCES, score);
}

void BattlegroundSM::EndBattleground(uint32 winner)
{
    // Win reward
    RewardHonorToTeam(GetBonusHonorFromKill(1), winner == ALLIANCE ? ALLIANCE : HORDE);

    // Complete map reward
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);

    Battleground::EndBattleground(winner);

    Creature* trigger = NULL;
    if (trigger = HashMapHolder<Creature>::Find(BgCreatures[SM_MINE_CART_TRIGGER]))
    {
        for (uint32 cartEntry = NPC_MINE_CART_1; cartEntry <= NPC_MINE_CART_3; ++cartEntry)
            if (Creature* cart = trigger->FindNearestCreature(cartEntry, 99999.0f, true))
                cart->DespawnOrUnsummon();
    }
}

void BattlegroundSM::UpdatePointsCount(uint32 Team)
{
    UpdateWorldState(SM_ALLIANCE_RESOURCES, m_TeamScores[Team == ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE]);
}

void BattlegroundSM::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundSMScore(player->GetGUID(), player->GetBGTeam());
}

bool BattlegroundSM::SetupBattleground()
{
    // doors
    if (!AddObject(BG_SM_OBJECT_WATERFALL_DEPOT, BG_SM_MINE_DEPOT, BG_SM_DepotPos[SM_WATERFALL_DEPOT][0], BG_SM_DepotPos[SM_WATERFALL_DEPOT][1], BG_SM_DepotPos[SM_WATERFALL_DEPOT][2], BG_SM_DepotPos[SM_WATERFALL_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)	// Waterfall
        || !AddObject(BG_SM_OBJECT_LAVA_DEPOT, BG_SM_MINE_DEPOT, BG_SM_DepotPos[SM_LAVA_DEPOT][0], BG_SM_DepotPos[SM_LAVA_DEPOT][1], BG_SM_DepotPos[SM_LAVA_DEPOT][2], BG_SM_DepotPos[SM_LAVA_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)							// Lava
        || !AddObject(BG_SM_OBJECT_DIAMOND_DEPOT, BG_SM_MINE_DEPOT, BG_SM_DepotPos[SM_DIAMOND_DEPOT][0], BG_SM_DepotPos[SM_DIAMOND_DEPOT][1], BG_SM_DepotPos[SM_DIAMOND_DEPOT][2], BG_SM_DepotPos[SM_DIAMOND_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)			// Diamond
        || !AddObject(BG_SM_OBJECT_TROLL_DEPOT, BG_SM_MINE_DEPOT, BG_SM_DepotPos[SM_TROLL_DEPOT][0], BG_SM_DepotPos[SM_TROLL_DEPOT][1], BG_SM_DepotPos[SM_TROLL_DEPOT][2], BG_SM_DepotPos[SM_TROLL_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)						// Troll
        || !AddObject(BG_SM_OBJECT_DOOR_A_1, BG_SM_DOOR, BG_SM_DoorPos[0][0], BG_SM_DoorPos[0][1], BG_SM_DoorPos[0][2], BG_SM_DoorPos[0][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_DOOR_A_2, BG_SM_DOOR, BG_SM_DoorPos[1][0], BG_SM_DoorPos[1][1], BG_SM_DoorPos[1][2], BG_SM_DoorPos[1][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_DOOR_H_1, BG_SM_DOOR, BG_SM_DoorPos[2][0], BG_SM_DoorPos[2][1], BG_SM_DoorPos[2][2], BG_SM_DoorPos[2][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_DOOR_H_2, BG_SM_DOOR, BG_SM_DoorPos[3][0], BG_SM_DoorPos[3][1], BG_SM_DoorPos[3][2], BG_SM_DoorPos[3][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_BERSERKING_BUFF_EAST, BG_OBJECTID_BERSERKERBUFF_ENTRY, BG_SM_BuffPos[0][0], BG_SM_BuffPos[0][1], BG_SM_BuffPos[0][2], BG_SM_BuffPos[0][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_BERSERKING_BUFF_WEST, BG_OBJECTID_BERSERKERBUFF_ENTRY, BG_SM_BuffPos[1][0], BG_SM_BuffPos[1][1], BG_SM_BuffPos[1][2], BG_SM_BuffPos[1][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_RESTORATION_BUFF_WATERFALL, BG_OBJECTID_REGENBUFF_ENTRY, BG_SM_BuffPos[2][0], BG_SM_BuffPos[2][1], BG_SM_BuffPos[2][2], BG_SM_BuffPos[2][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_RESTORATION_BUFF_LAVA, BG_OBJECTID_REGENBUFF_ENTRY, BG_SM_BuffPos[3][0], BG_SM_BuffPos[3][1], BG_SM_BuffPos[3][2], BG_SM_BuffPos[3][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY))
    {
        TC_LOG_ERROR("sql", "BatteGroundSM: Failed to spawn some object Battleground not created!");
        return false;
    }

    // Npcs
    if (!AddCreature(NPC_MINE_CART_TRIGGER, SM_MINE_CART_TRIGGER, 748.360779f, 195.203018f, 331.861938f, 2.428625f)
        || !AddCreature(NPC_TRACK_SWITCH_EAST, SM_TRACK_SWITCH_EAST, BG_SM_TrackPos[SM_EAST_PATH][0], BG_SM_TrackPos[SM_EAST_PATH][1], BG_SM_TrackPos[SM_EAST_PATH][2], BG_SM_TrackPos[SM_EAST_PATH][3])
        || !AddCreature(NPC_TRACK_SWITCH_NORTH, SM_TRACK_SWITCH_NORTH, BG_SM_TrackPos[SM_NORTH_PATH][0], BG_SM_TrackPos[SM_NORTH_PATH][1], BG_SM_TrackPos[SM_NORTH_PATH][2], BG_SM_TrackPos[SM_NORTH_PATH][3]))
    {
        TC_LOG_ERROR("sql", "BatteGroundSM: Failed to spawn some creatures Battleground not created!");
        return false;
    }

    if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_EAST]))
    {
        track->CastSpell(track, BG_SM_FEIGN_DEATH_STUN, true);
        track->CastSpell(track, BG_SM_TRACK_SWITCH_OPENED, true);
    }

    if (Creature* track = HashMapHolder<Creature>::Find(BgCreatures[SM_TRACK_SWITCH_NORTH]))
    {
        track->CastSpell(track, BG_SM_FEIGN_DEATH_STUN, true);
        track->CastSpell(track, BG_SM_TRACK_SWITCH_CLOSED, true);
    }

    WorldSafeLocsEntry const* sg = NULL;
    sg = sWorldSafeLocsStore.LookupEntry(SM_GRAVEYARD_MAIN_ALLIANCE);
    if (!sg || !AddSpiritGuide(SM_SPIRIT_ALLIANCE, sg->Loc.X, sg->Loc.Y, sg->Loc.Z, 2.138462f, TEAM_ALLIANCE))
    {
        TC_LOG_ERROR("sql", "BatteGroundSM: Failed to spawn spirit guide! Battleground not created!");
        return false;
    }

    sg = sWorldSafeLocsStore.LookupEntry(SM_GRAVEYARD_MAIN_HORDE);
    if (!sg || !AddSpiritGuide(SM_SPIRIT_HORDE, sg->Loc.X, sg->Loc.Y, sg->Loc.Z, 5.570653f, TEAM_HORDE))
    {
        TC_LOG_ERROR("sql", "BatteGroundSM: Failed to spawn spirit guide! Battleground not created!");
        return false;
    }
    return true;
}

void BattlegroundSM::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    Battleground::HandleKillPlayer(player, killer);
    EventPlayerDroppedFlag(player);
}

bool BattlegroundSM::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found
        return false;

    switch (type)
    {
        case SCORE_CART_CONTROLLED:                           // Mine Carts captures
            ((BattlegroundSMScore*)itr->second)->MineCartCaptures += value;
            break;

        default:
            Battleground::UpdatePlayerScore(Source, type, value, doAddHonor);
            break;
    }

    return true;
}

void BattlegroundSM::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(uint32(SM_MINE_CARTS_DISPLAY), uint32(DISPLAY_WORLDSTATE));
    packet.Worldstates.emplace_back(uint32(SM_ALLIANCE_RESOURCES), uint32(m_TeamScores[TEAM_HORDE]));
    packet.Worldstates.emplace_back(uint32(SM_HORDE_RESOURCES), uint32(m_TeamScores[TEAM_ALLIANCE]));
    packet.Worldstates.emplace_back(uint32(SM_MINE_CART_1), uint32(DISPLAY_WORLDSTATE));
    packet.Worldstates.emplace_back(uint32(SM_MINE_CART_2), uint32(DISPLAY_WORLDSTATE));
    packet.Worldstates.emplace_back(uint32(SM_MINE_CART_3), uint32(DISPLAY_WORLDSTATE));
    packet.Worldstates.emplace_back(uint32(SM_DISPLAY_ALLIANCE_RESSOURCES), uint32(DISPLAY_WORLDSTATE));
    packet.Worldstates.emplace_back(uint32(SM_DISPLAY_HORDE_RESSOURCES), uint32(DISPLAY_WORLDSTATE));
    packet.Worldstates.emplace_back(uint32(SM_DISPLAY_PROGRESS_BAR), uint32(BG_SM_PROGRESS_BAR_DONT_SHOW)); // This shows the mine cart control bar
    packet.Worldstates.emplace_back(uint32(SM_PROGRESS_BAR_STATUS), uint32(BG_SM_PROGRESS_BAR_NEUTRAL)); // Neutral
    packet.Worldstates.emplace_back(uint32(SM_UNK), uint32(0));
}

uint32 BattlegroundSM::GetPrematureWinner()
{
    if (GetTeamScore(TEAM_ALLIANCE) > GetTeamScore(TEAM_HORDE))
        return ALLIANCE;

    else if (GetTeamScore(TEAM_HORDE) > GetTeamScore(TEAM_ALLIANCE))
        return HORDE;

    return Battleground::GetPrematureWinner();
}