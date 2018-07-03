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

#include "Battleground.h"
#include "BattlegroundTK.h"
#include "Creature.h"
#include "GameObject.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldStatePackets.h"
#include "SpellInfo.h"

BattlegroundTK::BattlegroundTK()
{
    BgObjects.resize(BG_TK_OBJECT_MAX);
    BgCreatures.resize(BG_CREATURES_MAX_TK);

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_TK_START_TWO_MINUTES; 
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_TK_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_TK_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_TK_HAS_BEGUN;

    pointsTimer = 4 * IN_MILLISECONDS;

    for (uint8 i = 0; i < BG_TK_MAX_ORBS; ++i)
        m_orbOwners[i] = ObjectGuid::Empty;

    _orbState[0] = BG_TK_ORB_STATE_ON_BASE;
    _orbState[1] = BG_TK_ORB_STATE_ON_BASE;
    _orbState[2] = BG_TK_ORB_STATE_ON_BASE;
    _orbState[3] = BG_TK_ORB_STATE_ON_BASE;

    m_Team_Scores[TEAM_ALLIANCE] = 0;
    m_Team_Scores[TEAM_HORDE] = 0;

    if (sBattlegroundMgr->IsBGWeekend(GetTypeID()))
    {
        m_HonorWinKills = 3;
        m_HonorEndKills = 4;
    }
    else
    {
        m_HonorWinKills = 1;
        m_HonorEndKills = 2;
    }

    m_IsInformedNearVictory = false;
    bgEnd = false;
}

BattlegroundTK::~BattlegroundTK()
{
}

void BattlegroundTK::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        BattlegroundTK::CalculatePoints(diff);

        if (GetElapsedTime() >= 27*MINUTE*IN_MILLISECONDS)
        {
            if (m_Team_Scores[TEAM_ALLIANCE] == 0)
            {
                if (m_Team_Scores[TEAM_HORDE] == 0)        // No one scored - result is tie
                    EndBattleground(WINNER_NONE);
                else                                 // Horde has more points and thus wins
                    EndBattleground(WINNER_HORDE);
            }
            else if (m_Team_Scores[TEAM_HORDE] == 0)
                EndBattleground(ALLIANCE);           // Alliance has > 0, Horde has 0, alliance wins
            else if (m_Team_Scores[TEAM_HORDE] == m_Team_Scores[TEAM_ALLIANCE]) // Team score equal, winner is alliance
                EndBattleground(WINNER_HORDE);
            else if (m_Team_Scores[TEAM_HORDE] > m_Team_Scores[TEAM_ALLIANCE])  // Last but not least, check who has the higher score
                EndBattleground(WINNER_HORDE);
            else
                EndBattleground(WINNER_ALLIANCE);
        }

        else if (GetElapsedTime() > uint32(_minutesElapsed * MINUTE * IN_MILLISECONDS) +  3 * MINUTE * IN_MILLISECONDS)
            ++_minutesElapsed;

        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (player->GetGUID() == m_orbOwners[0])
                {
                    if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_BLUE))
                        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);
                }
                else if (player->GetGUID() == m_orbOwners[1])
                {
                    if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_PURPLE))
                        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);
                }
                else if (player->GetGUID() == m_orbOwners[2])
                {
                    if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_GREEN))
                        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);
                }
                else if (player->GetGUID() == m_orbOwners[3])
                {
                    if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_ORANGE))
                        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);
                }
            }
    }
}

void BattlegroundTK::CalculatePoints(uint32 diff)
{
    if (pointsTimer <= 0)
    {
        pointsTimer = 4 * IN_MILLISECONDS;

        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                for (uint8 i = 0; i < BG_TK_MAX_ORBS; ++i)
                {
                    if (m_orbOwners[i] == player->GetGUID())
                    {
                        if (player->GetExactDist2d(1783.319336f, 1333.339722f) <= 50.0f)
                        {
                            UpdateScore(player->GetTeam() == ALLIANCE ? ALLIANCE : HORDE, BG_TK_CENTER_POINTS);
                            UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_CENTER_POINTS);
                        }
                        else if (player->GetExactDist2d(1783.319336f, 1333.339722f) > 50.0f && player->GetExactDist2d(1783.319336f, 1333.339722f) <= 100.0f)
                        {
                            UpdateScore(player->GetTeam() == ALLIANCE ? ALLIANCE : HORDE, BG_TK_INDOOR_POINTS);
                            UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_INDOOR_POINTS);
                        }
                        else if (player->GetExactDist2d(1783.319336f, 1333.339722f) > 100.0f)
                        {
                            UpdateScore(player->GetTeam() == ALLIANCE ? ALLIANCE : HORDE, BG_TK_OUTDOOR_POINTS);
                            UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_OUTDOOR_POINTS);
                        }
                    }
                }
            }
        }
    } else pointsTimer -= diff;
}

void BattlegroundTK::StartingEventCloseDoors()
{
    DoorClose(BG_TK_OBJECT_DOOR_A);
    DoorClose(BG_TK_OBJECT_DOOR_H);
    SpawnBGObject(BG_TK_OBJECT_DOOR_A, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_TK_OBJECT_DOOR_H, RESPAWN_IMMEDIATELY);
    for (uint32 i = BG_TK_OBJECT_ORB_BLUE; i <= BG_TK_OBJECT_ORB_ORANGE; ++i)
        SpawnBGObject(i, RESPAWN_ONE_DAY);
}

void BattlegroundTK::StartingEventOpenDoors()
{
    DoorOpen(BG_TK_OBJECT_DOOR_A);
    DoorOpen(BG_TK_OBJECT_DOOR_H);

    for (uint32 i = BG_TK_OBJECT_ORB_BLUE; i <= BG_TK_OBJECT_ORB_ORANGE; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundTK::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundTKScore(player->GetGUID(), player->GetBGTeam());
}

void BattlegroundTK::RespawnOrbAfterDrop(uint32 orb)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (orb)
    {
        case BG_TK_OBJECT_ORB_BLUE:
            SpawnBGObject(BG_TK_OBJECT_ORB_BLUE, RESPAWN_IMMEDIATELY);
            SendMessageToAll(LANG_BG_TK_ORB_BLUE_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            _orbState[BG_TK_OBJECT_ORB_BLUE] = BG_TK_ORB_STATE_ON_BASE;
            break;

        case BG_TK_OBJECT_ORB_PURPLE:
            SpawnBGObject(BG_TK_OBJECT_ORB_PURPLE, RESPAWN_IMMEDIATELY);
            SendMessageToAll(LANG_BG_TK_ORB_PURPLE_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            _orbState[BG_TK_OBJECT_ORB_PURPLE] = BG_TK_ORB_STATE_ON_BASE;
            break;

        case BG_TK_OBJECT_ORB_GREEN:
            SpawnBGObject(BG_TK_OBJECT_ORB_GREEN, RESPAWN_IMMEDIATELY);
            SendMessageToAll(LANG_BG_TK_ORB_GREEN_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            _orbState[BG_TK_OBJECT_ORB_GREEN] = BG_TK_ORB_STATE_ON_BASE;
            break;

        case BG_TK_OBJECT_ORB_ORANGE:
            SpawnBGObject(BG_TK_OBJECT_ORB_ORANGE, RESPAWN_IMMEDIATELY);
            SendMessageToAll(LANG_BG_TK_ORB_ORANGE_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            _orbState[BG_TK_OBJECT_ORB_ORANGE] = BG_TK_ORB_STATE_ON_BASE;
            break;
    }

    PlaySoundToAll(BG_TK_SOUND_ORB_RETURNED);

    SetOrbPicker(ObjectGuid::Empty, orb);
}

void BattlegroundTK::EventPlayerClickedOnFlag(Player* player, GameObject* target_obj)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if(HasAnOrb(player))
       return;

    int32 message_id = 0;
    ChatMsg type = CHAT_MSG_BG_SYSTEM_NEUTRAL;

    // Udapte orb blue picked up
    if (GetOrbState(BG_TK_OBJECT_ORB_BLUE) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_BLUE] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_BLUE;

        if (player->GetTeam() == ALLIANCE)
        {
            type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
            PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
        }
        else if (player->GetTeam() ==  HORDE)
        {
            type = CHAT_MSG_BG_SYSTEM_HORDE;
            PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
        }

        SpawnBGObject(BG_TK_OBJECT_ORB_BLUE, RESPAWN_ONE_DAY);
        player->CastSpell(player, BG_TK_AURA_ORB_BLUE, true);
        SetOrbPicker(player->GetGUID(), BG_TK_OBJECT_ORB_BLUE);
        _orbState[BG_TK_OBJECT_ORB_BLUE] = BG_TK_ORB_STATE_ON_PLAYER;
    }

    // Udapte orb purple picked up
    if (GetOrbState(BG_TK_OBJECT_ORB_PURPLE) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_PURPLE] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_PURPLE;

        if (player->GetTeam() == ALLIANCE)
        {
            type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
            PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
        }
        else if (player->GetTeam() == HORDE)
        {
            type = CHAT_MSG_BG_SYSTEM_HORDE;
            PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
        }

        SpawnBGObject(BG_TK_OBJECT_ORB_PURPLE, RESPAWN_ONE_DAY);
        player->CastSpell(player, BG_TK_AURA_ORB_PURPLE, true);
        SetOrbPicker(player->GetGUID(), BG_TK_OBJECT_ORB_PURPLE);
        _orbState[BG_TK_OBJECT_ORB_PURPLE] = BG_TK_ORB_STATE_ON_PLAYER;
    }

    // Udapte orb green picked up
    if (GetOrbState(BG_TK_OBJECT_ORB_GREEN) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_GREEN] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_GREEN;
        if (player->GetTeam() == ALLIANCE)
        {
            type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
            PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
        }
        else if (player->GetTeam() ==  HORDE)
        {
            type = CHAT_MSG_BG_SYSTEM_HORDE;
            PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
        }

        SpawnBGObject(BG_TK_OBJECT_ORB_GREEN, RESPAWN_ONE_DAY);
        player->CastSpell(player, BG_TK_AURA_ORB_GREEN, true);
        SetOrbPicker(player->GetGUID(), BG_TK_OBJECT_ORB_GREEN);
        _orbState[BG_TK_OBJECT_ORB_GREEN] = BG_TK_ORB_STATE_ON_PLAYER;
    }

    // Udapte orb orange picked up
    if (GetOrbState(BG_TK_OBJECT_ORB_ORANGE) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_ORANGE] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_ORANGE;

        if (player->GetTeam() == ALLIANCE)
        {
            type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
            PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
        }
        else if (player->GetTeam() ==  HORDE)
        {
            type = CHAT_MSG_BG_SYSTEM_HORDE;
            PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
        }

        SpawnBGObject(BG_TK_OBJECT_ORB_ORANGE, RESPAWN_ONE_DAY);
        player->CastSpell(player, BG_TK_AURA_ORB_ORANGE, true);
        SetOrbPicker(player->GetGUID(), BG_TK_OBJECT_ORB_ORANGE);
        _orbState[BG_TK_OBJECT_ORB_ORANGE] = BG_TK_ORB_STATE_ON_PLAYER;
    }

    if (!message_id)
        return;

    UpdatePlayerScore(player, SCORE_ORB_POSSESIONS, 1);
    Battleground::PSendMessageToAll(message_id, type, NULL, player->GetName().c_str());
    player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundTK::RemovePlayer(Player* player, ObjectGuid guid, uint32 /*team*/)
{
    if (m_orbOwners[BG_TK_OBJECT_ORB_BLUE] == guid)
    {
        if (!player)
        {
            TC_LOG_DEBUG("network", "BattlegroundTK: Removing offline player who has the ORB!");
            player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_BLUE);
            SetOrbPicker(ObjectGuid::Empty, BG_TK_OBJECT_ORB_BLUE);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);
    }
    else if (m_orbOwners[BG_TK_OBJECT_ORB_PURPLE] == guid)
    {
        if (!player)
        {
            TC_LOG_DEBUG("network", "BattlegroundTK: Removing offline player who has the ORB!");
            player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_PURPLE);
            SetOrbPicker(ObjectGuid::Empty, BG_TK_OBJECT_ORB_PURPLE);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);
    }
    else if (m_orbOwners[BG_TK_OBJECT_ORB_GREEN] == guid)
    {
        if (!player)
        {
            TC_LOG_DEBUG("network", "BattlegroundTK: Removing offline player who has the ORB!");
            player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_GREEN);
            SetOrbPicker(ObjectGuid::Empty, BG_TK_OBJECT_ORB_GREEN);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);
    }
    else if (m_orbOwners[BG_TK_OBJECT_ORB_ORANGE] == guid)
    {
        if (!player)
        {
            TC_LOG_DEBUG("network", "BattlegroundTK: Removing offline player who has the ORB!");
            player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_ORANGE);
            SetOrbPicker(ObjectGuid::Empty, BG_TK_OBJECT_ORB_ORANGE);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);
    }
}

void BattlegroundTK::UpdateScore(uint16 team, int16 points)
{
    ASSERT(team == ALLIANCE || team == HORDE);
    uint8 teamindex = GetTeamIndexByTeamId(team); // 0 = Alliance 1 = Horde

    if (points > 0)
    {
        if (points + m_Team_Scores[teamindex] >= BG_TK_MAX_TEAM_SCORE)
        {
            if (!bgEnd)
            {
                m_Team_Scores[teamindex] = BG_TK_MAX_TEAM_SCORE;
                UpdateWorldState(((teamindex == TEAM_ALLIANCE)?BG_TK_RESOURCES_ALLIANCE:BG_TK_RESOURCES_HORDE), m_Team_Scores[teamindex]);
                EndBattleground((teamindex == TEAM_ALLIANCE) ? WINNER_ALLIANCE : WINNER_HORDE);
                bgEnd = true; // Prevent from UpdateScore repeat EndBattleground
            }
        }
        else
        {
            m_Team_Scores[teamindex] += points;
            UpdateWorldState(((teamindex == TEAM_ALLIANCE)? BG_TK_RESOURCES_ALLIANCE : BG_TK_RESOURCES_HORDE), m_Team_Scores[teamindex]);
        }

        if (!m_IsInformedNearVictory && m_Team_Scores[teamindex] > BG_TK_WARNING_NEAR_VICTORY_SCORE)
        {
            SendMessageToAll(team == ALLIANCE ? LANG_BG_TK_A_NEAR_VICTORY : LANG_BG_TK_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_TK_SOUND_NEAR_VICTORY);
            m_IsInformedNearVictory = true;
        }
    }
}

WorldSafeLocsEntry const* BattlegroundTK::GetClosestGraveYard(Player* player)
{
    return sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[player->GetTeam() == ALLIANCE ? 0 : 1]);
}

bool BattlegroundTK::SetupBattleground()
{
    // Orbs
    if (!AddObject(BG_TK_OBJECT_ORB_BLUE, BG_OBJECT_TK_ORB_BLUE, 1716.808105f, 1249.910645f, 10.675041f, 0.0f, 0, 0, 0.9996573f, 0.02617699f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TK_OBJECT_ORB_PURPLE, BG_OBJECT_TK_ORB_PURPLE, 1850.244995f, 1416.825195f, 10.662804f, 0.0f, 0, 0, 0.008726535f, 0.9999619f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TK_OBJECT_ORB_GREEN, BG_OBJECT_TK_ORB_GREEN, 1716.775635f, 1416.752686f, 10.678053f, 0.0f, 0, 0, 0.008726535f, 0.9999619f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TK_OBJECT_ORB_ORANGE, BG_OBJECT_TK_ORB_ORANGE, 1850.234375f, 1250.272827f, 10.689357f, 0.0f, 0, 0, 0.008726535f, 0.9999619f, RESPAWN_IMMEDIATELY)
        // Horde gate
        || !AddObject(BG_TK_OBJECT_DOOR_H, BG_OBJECT_DOOR_TK_H, 1780.151733f, 1565.395874f, 22.469021f, 4.70132f, 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        // Alliance gate
        || !AddObject(BG_TK_OBJECT_DOOR_A, BG_OBJECT_DOOR_TK_A, 1783.735107f, 1104.960205f, 19.374258f, 1.62f, 0.0f, 0.0f, 0.71615f, 0.697947f, RESPAWN_IMMEDIATELY)
        )
    {
        TC_LOG_DEBUG("network", "BattleGroundTK: Failed to spawn some object Battleground not created!");
        return false;
    }

    WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[0]); // Alliance
    if (!sg || !AddSpiritGuide(TK_SPIRIT_MAIN_ALLIANCE, sg->Loc.X, sg->Loc.Y, sg->Loc.Z, 2.498876f, TEAM_ALLIANCE))
    {
        TC_LOG_DEBUG("network", "BattleGroundTK: Failed to spawn Alliance spirit guide! Battleground not created!");
        return false;
    }

    sg = sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[1]); // Horde
    if (!sg || !AddSpiritGuide(TK_SPIRIT_MAIN_HORDE, sg->Loc.X, sg->Loc.Y, sg->Loc.Z, 5.936957f, TEAM_HORDE))
    {
        TC_LOG_DEBUG("network", "BatlteGroundTK: Failed to spawn Horde spirit guide! Battleground not created!");
        return false;
    }

    TC_LOG_DEBUG("network", "BattleGroundTK: BG objects and spirit guides spawned");

    return true;
}

void BattlegroundTK::EndBattleground(uint32 winner)
{
    RewardHonorToTeam(GetBonusHonorFromKill(1), winner == WINNER_ALLIANCE ? ALLIANCE : HORDE);

    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    Battleground::EndBattleground(winner);
}

void BattlegroundTK::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
    
    UpdateScore(killer->GetTeam(), BG_TK_PLAYER_KILL_POINTS);
    UpdatePlayerScore(killer, SCORE_POINTS_SCORED, BG_TK_PLAYER_KILL_POINTS);

    if (m_orbOwners[0] == player->GetGUID())
        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);
    else if (m_orbOwners[1] == player->GetGUID())
        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);
    else if (m_orbOwners[2] == player->GetGUID())
        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);
    else if (m_orbOwners[3] == player->GetGUID())
        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);

    Battleground::HandleKillPlayer(player, killer);
}

bool BattlegroundTK::UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(player->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return false;

    switch (type)
    {
        case SCORE_ORB_POSSESIONS : // Orbs owned
            ((BattlegroundTKScore*)itr->second)->OrbPossesions += value;
            break;

        case SCORE_POINTS_SCORED: // Points earned
            ((BattlegroundTKScore*)itr->second)->PointsScored += value;
            break;

        default:
            Battleground::UpdatePlayerScore(player, type, value, doAddHonor);
            break;
    }
    return true;
}

bool BattlegroundTK::HasAnOrb(Player* player)
{
    for (uint8 i = 0; i < BG_TK_MAX_ORBS; ++i)
        if (m_orbOwners[i] == player->GetGUID())
            return true;
    return false;
}

void BattlegroundTK::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(uint32(BG_TK_RESOURCES_ALLIANCE), uint32(m_Team_Scores[TEAM_ALLIANCE]));
    packet.Worldstates.emplace_back(uint32(BG_TK_RESOURCES_HORDE), uint32(m_Team_Scores[TEAM_HORDE]));
}

uint32 BattlegroundTK::GetPrematureWinner()
{
    if (m_Team_Scores[TEAM_ALLIANCE] > m_Team_Scores[TEAM_HORDE])
        return ALLIANCE;
    else if (m_Team_Scores[TEAM_HORDE] > m_Team_Scores[TEAM_ALLIANCE])
        return HORDE;

    return Battleground::GetPrematureWinner();
}