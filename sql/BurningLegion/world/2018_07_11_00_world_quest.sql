---fix quest 40956
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=283885);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=281600);
UPDATE `quest_objectives` SET ``Flags`=4 WHERE (`ID`=281565);

---fix quest 39987
UPDATE `creature_template` SET `KillCredit1`=3333333, `KillCredit2`=114070 `AIName`='SmartAI' WHERE (`entry`=114071);
UPDATE `quest_objectives` SET `ObjectID`=3333333 WHERE (`ID`=280298);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=280299);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `TitleAlt`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `HealthScalingExpansion`, `RequiredExpansion`, `VignetteID`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES ('3333333', '0', '0', '0', '0', '0', '70917', '0', '0', '0', 'Killcredit', NULL, NULL, NULL, NULL, '0', '1', '1', '0', '0', '0', '35', '0', '1', '1.14286', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '0', '128', '', '0');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('114071', '0', '0', '0', '10', '0', '100', '0', '1', '10', '3000', '3000', '', '33', '3333333', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'quest');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('114071', '0', '1', '0', '10', '0', '100', '0', '1', '10', '0', '0', '', '33', '114070', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'quest');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('114071', '0', '2', '0', '10', '0', '100', '0', '1', '10', '0', '0', '', '33', '100169', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'quest ');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000116', '100169', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1480.52', '4453.72', '123.153', '3.2455', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000119', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1476.64', '4451.86', '122.82', '3.0523', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000120', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1470.74', '4452.83', '122.214', '3.16226', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000121', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1467.02', '4451.7', '122.193', '1.4489', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000122', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1483.98', '4453.73', '123.447', '0.9423', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000125', '3333333', '1220', '0', '0', '1', '0', '0', '0', '-1', '11686', '0', '1471.31', '4447.12', '121.999', '0.284932', '300', '0', '0', '87', '0', '0', '0', '33554432', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000126', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1466.37', '4446.86', '121.935', '0.601446', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000127', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1453.85', '4445.84', '119.913', '0.110573', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000128', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1455.49', '4445.79', '120.18', '0.189094', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000129', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1459.35', '4446.72', '121.102', '0.26763', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000130', '114070', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1458.27', '4446.47', '120.888', '0.354009', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000131', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1454.91', '4446.61', '120.314', '6.23349', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000132', '99213', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '1', '1462.89', '4446.1', '121.455', '0.214196', '300', '0', '0', '3117801', '0', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000133', '100169', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1461.97', '4444.01', '120.795', '1.47318', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000134', '114071', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '1443.53', '4447.13', '120.056', '3.36835', '300', '0', '0', '831414', '1', '0', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000135', '98312', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '1', '1477.33', '4732.56', '143.152', '5.78345', '300', '0', '0', '831414', '1', '0', '16777219', '0', '0', '0', '0', '', '0');

---fix quest 44009
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286220);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286179);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286138);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286103);

---fix quest 40326
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE (`entry`=245327);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('245327', '1', '0', '0', '70', '0', '100', '0', '2', '0', '0', '0', '', '41', '35', '5', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'despawn stop spam');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('245327', '1', '1', '0', '70', '0', '100', '0', '2', '0', '0', '0', '', '11', '196091', '0', '0', '0', '0', '0', '16', '0', '0', '0', '0', '0', '0', '0', 'quest');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120104', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1868.68', '5275.84', '73.8955', '5.36835', '-0', '-0', '-0.441633', '0.897196', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120105', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1970.71', '5218.31', '89.5316', '5.82386', '-0', '-0', '-0.22765', '0.973743', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120106', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1925.08', '5144.47', '83.9449', '3.16685', '-0', '-0', '-0.99992', '0.0126301', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120107', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1860.86', '5139.91', '83.3862', '2.3736', '-0', '-0', '-0.927175', '-0.374628', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120108', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1872.8', '5185.97', '83.2473', '1.48846', '-0', '-0', '-0.677404', '-0.735611', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120109', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1824.91', '5279.83', '71.6898', '3.26424', '-0', '-0', '-0.99812', '0.0612867', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120110', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1747.96', '5210.43', '70.9358', '4.3316', '-0', '-0', '-0.828147', '0.560512', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120111', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1719', '5257.14', '71.7077', '2.5833', '-0', '-0', '-0.961291', '-0.275534', '300', '255', '1', '0', '', '0');
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES ('210120112', '245327', '1220', '0', '0', '1', '0', '0', '0', '-1', '1697.4', '5172.22', '71.4449', '5.21989', '-0', '-0', '-0.506956', '0.861972', '300', '255', '1', '0', '', '0');


---fix quest 44412
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286615);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286614);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286613);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286612);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286611);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286610);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286609);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286608);

---fix quest 38035
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=280241);

---fix quest 38206
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=279841);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=279840);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=279839);

---fix quest 39800
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286405);

---fix quest 37530
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=276733);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277634);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277633);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277631);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277630);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277629);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=276735);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000139', '89090', '1220', '0', '0', '1', '0', '0', '0', '-1', '61099', '0', '-130.733', '6019.18', '47.3834', '0.393065', '300', '0', '0', '87', '0', '0', '0', '0', '0', '0', '0', '', '0');

---fix quest 46499
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES ('121302', '146955', '0', '100', '0', '1', '0', '1', '1', NULL);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES ('119886', '46499');
INSERT INTO `creature_questender` (`id`, `quest`) VALUES ('119886', '46499');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000143', '121302', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '-725.322', '3301.92', '44.0267', '3.06569', '300', '0', '0', '87', '0', '0', '0', '0', '0', '0', '0', '', '0');

---fix quest 37497
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=276977);

---fix quest 47967
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=291248);

---fix quest 37536
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=276360);


--fix quest 29786
DELETE FROM creature WHERE id IN (55786);
UPDATE `creature_template` SET `HealthModifier`=50 WHERE (`entry`=55786);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('20000000000147', '55786', '860', '0', '0', '1', '0', '524', '0', '-1', '0', '0', '714.028', '4164.85', '195.911', '2.65269', '300', '0', '0', '16900', '0', '0', '0', '0', '0', '0', '0', '', '0');
