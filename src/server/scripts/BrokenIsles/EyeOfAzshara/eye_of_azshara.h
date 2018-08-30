/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
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

#ifndef DEF_EYE_OF_AZSHARA_H
#define DEF_EYE_OF_AZSHARA_H

#define EoAScriptName "instance_eye_of_azshara"
#define DataHeader    "EOA"

uint32 const EncounterCount = 5;

enum EOADataTypes
{
    DATA_WARLORD_PARJESH  = 0,
    DATA_LADY_HATECOIL    = 1,
    DATA_SERPENTRIX       = 2,
    DATA_KING_DEEPBEARD   = 3,
    DATA_WRATH_OF_AZSHARA = 4,

    DATA_ARCANIST_DIED    = 5,
    DATA_RESPAWN_DUNES    = 6,
    DATA_BOSS_DIED        = 7,
    DATA_NAGA_DIED        = 8,
    DATA_CRY_OF_WRATH     = 9
};

enum EOACreatureIds
{
    NPC_SERPENTRIX          = 91808,
    NPC_WARLORD_PARJESH     = 91784,
    NPC_LADY_HATECOIL       = 91789,
    NPC_KING_DEEPBEARD      = 91797,
    NPC_WRATH_OF_AZSHARA    = 96028,
    NPC_HATECOIL_ARCANIST   = 97171,

    NPC_SAND_DUNE           = 97853,

    NPC_RITUALIST_LESHA     = 100248,
    NPC_CHANNELER_VARISZ    = 100249,
    NPC_BINDER_ASHIOI       = 100250,
    NPC_MYSTIC_SSAVEH       = 98173,

    NPC_WATER_SPOUT         = 103674, // Not sure

    NPC_WEATHERMAN          = 97063
};

enum EOAGameObjectIds
{
    GO_BUBBLE               = 240788,
    GOB_SAND_DUNE           = 244690
};

enum EOASpellIds
{
    SPELL_TEMPEST_ATTUNEMENT    = 193491,
    SPELL_AQUA_SPOUT_MISSILE    = 195212,
    SPELL_FEEDBACK              = 196035,
    SPELL_WATER_SPOUT_AT        = 195539,
    SPELL_WATER_SPOUT_DAMAGE    = 195541,
    SPELL_MASSIVE_QUAKE_AT      = 195828,
    SPELL_MASSIVE_QUAKE_DAMAGE  = 195832,
    SPELL_ARMORSHELL_SHRAPNEL   = 196183,

    SPELL_VIOLENT_WINDS_90S     = 191792,
    SPELL_VIOLENT_WINDS_30S     = 191805,
    SPELL_VIOLENT_WINDS_10S     = 192649,
    SPELL_VIOLENT_WINDS_DUMMY   = 191797,

    SPELL_LIGHTNING_STRIKES         = 192796, // Cast on a player
    SPELL_LIGHTNING_STRIKES_5S      = 192737, // Triggers 192796 every 5 sec
    SPELL_LIGHTNING_STRIKES_AMBIENT = 192728, // Visual only, launched every 250ms (on what? seems to be random coordinates)

    SPELL_SKYBOX_RAIN           = 191815,
    SPELL_SKYBOX_WIND           = 212614,
    SPELL_SKYBOX_LIGHTNING      = 191816,
    SPELL_SKYBOX_HURRICANE      = 212615,
};

enum eData
{
    DATA_WORLDBREAKER       = 0,
    DATA_FELHOUNDS,
    DATA_ANTORAN,
    
    DATA_HASABEL,
    DATA_EONAR,
    DATA_IMONAR,
    
    DATA_KINGAROTH,
    DATA_VARIMATHRAS,
    DATA_COVEN,
    
    DATA_AGGRAMAR,
    DATA_ARGUS,

    MAX_ENCOUNTER,
};

enum eCreatures
{
    NPC_DREADWING                   = 125487,
    NPC_LIGHTFORGED_WIREFRAME       = 125476,
    NPC_LIGHTFORGED_WIREFRAME_1     = 127256,
    NPC_LIGHT_CENTURION             = 125478,
    NPC_LIGHT_COMANDIR              = 125480,
    NPC_LIGHT_PRIEST                = 124778,

    NPC_EXARH_TURALION              = 125512, 
    NPC_TELEPORT_OF_LIGHTFORGED_1   = 130137,

    NPC_BATTLE_SHIP                 = 125771,

    NPC_RUN_NPC_1                   = 127221,
    NPC_RUN_NPC_2                   = 126444,

    NPC_FEL_PORTAL_1                = 127050,
    NPC_FEL_PORTAL_2                = 126527,

    NPC_WORLDBREAKER                = 122450, // boss
    NPC_ANNIHILATOR_OF_KINGAROTH    = 123398,
    NPC_WORLDBREAKER_DECIMATOR      = 122773,
    NPC_WORLDBREAKER_ANNIHILATOR    = 122778,
    NPC_WB_ANNIHILATION_TRIGGER     = 122818,
    NPC_WB_ANNIHILATION_TRIGGER_2   = 124330,
    NPC_SURGING_FEL_TRIGGER         = 124167, //Heroic+
    NPC_SURGING_FEL_TRIGGER_MOVER   = 128429, //Heroic+

    NPC_SHATUG                      = 122135, //Boss
    NPC_FHARG                       = 122477,
    NPC_MOLTEN_TOUCH                = 122507,

    NPC_HASABEL                     = 122104, // boss

    NPC_IMAGE_OF_EONAR              = 128352,
    NPC_EONAR_EVENT                 = 122500,

    NPC_IMONAR_INTRO                = 125692,
    NPC_IMONAR                      = 124158, // boss

    NPC_IMPLOSIONS                  = 125603,
    NPC_PRIESTESS_OF_DELIRIUM       = 128060,

    NPC_AZARA                       = 122467, // boss

    NPC_WIND_STALKER                = 123719,

    NPC_KINGAROTH                   = 122578, // Boss
    NPC_DETONATION_CHARGE           = 122585,
    NPC_RUINER                      = 124230,
    NPC_INCINERATOR_STALKER         = 124879,
    NPC_INFERNAL_TOWER_1            = 122634,
    NPC_INFERNAL_TOWER_2            = 122740,
    NPC_INFERNAL_TOWER_3            = 122885,
    NPC_GAROTHI_ANNIHILATOR         = 123906,
    NPC_GAROTHI_DECIMATOR           = 123921,
    NPC_GAROTHI_DEMOLISHER          = 123929,
    NPC_APOCALYPSE_BLAST_STALKER    = 125462,
    NPC_ANNIHILATION_TRIGGER        = 124160,
    NPC_EMPOWERED_RUINER_STALKER    = 125646,
};

enum eGameObjects
{
    GO_STONE                        = 278488,
    GO_INVISIBLE_WALL               = 277365,
    GO_ELEVATOR                     = 278815,
    GO_PORTAL_TO_ELUNARIES          = 277474,

    GO_ARCANE_BARRIER               = 249386,
    GO_ARCANE_BARRIER_VISUAL        = 273911,

    GO_KINGAROTH_DOOR               = 277531,
    GO_KINGAROTH_TRAP_LEFT_1        = 276288,
    GO_KINGAROTH_TRAP_LEFT_2        = 276294,
    GO_KINGAROTH_TRAP_RIGHT_1       = 276292,
    GO_KINGAROTH_TRAP_RIGHT_2       = 276289,
    GO_KINGAROTH_TRAP_BACK_1        = 276290,
    GO_KINGAROTH_TRAP_BACK_2        = 276293,
};  

enum eSpells
{
    SPELL_INTRO_FIELD_OF_FIRE       = 249100,
    SPELL_SPAWN                     = 247963,
    SPELL_LIGHT_SHIELD              = 248515,

    SPELL_SURGE_OF_LIFE_OVERRIDE    = 254506, // TO-DO FIND REAL SPELL OF FIX  254506
};

#endif // EYE_OF_AZSHARA_H_
