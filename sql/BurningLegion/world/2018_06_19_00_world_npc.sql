UPDATE `creature_template` SET `npcflag`=8192 WHERE (`entry`=110971);
UPDATE `creature_template` SET `npcflag`=8192 WHERE (`entry`=90639);
INSERT INTO `creature` VALUES (210118241, 110971, 1220, 0, 0, 1, 0, 0, 0, -1, 0, 0, -223.467, 7734.84, 139.19, 3.09529, 300, 0, 0, 87, 0, 0, 8192, 0, 0, 0, 0, '', 0);
INSERT INTO `creature` VALUES (210118994, 100559, 1220, 0, 0, 1, 0, 0, 0, -1, 0, 0, 3896.76, 3496.99, 800.305, 2.85291, 300, 0, 0, 130624, 0, 0, 8192, 0, 0, 0, 0, '', 0);

UPDATE `creature_template` SET `npcflag`=4224 WHERE (`entry`=120898);
INSERT INTO `creature` VALUES (210118247, 68363, 369, 0, 0, 1, 0, 0, 0, -1, 0, 1, -80.9149, 2507.37, -43.1276, 3.28261, 300, 0, 0, 20785340, 0, 0, 0, 0, 0, 0, 0, '', 0);

INSERT INTO `creature` VALUES (2101182475, 102309, 1220, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1559.84, 3033.4, 90.2227, 2.08655, 300, 0, 0, 1039267, 1, 0, 0, 0, 0, 0, 0, '', 0);

UPDATE `creature_template` SET `faction`=14, `minlevel`=110, `maxlevel`=110 WHERE (`entry`=112759);
INSERT INTO `creature` VALUES (2101182476, 112759, 1220, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1094.38, 2155.01, 6.7752, 5.76795, 300, 0, 0, 573924, 799662, 0, 0, 0, 0, 0, 0, '', 0);
INSERT INTO `creature` VALUES (2101182477, 99948, 1220, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1636.42, 2734.57, 22.2798, 6.22132, 300, 0, 0, 1039267, 1, 0, 0, 0, 0, 0, 0, '', 0);

UPDATE `creature_template` SET `entry`='102104', `difficulty_entry_1`='0', `difficulty_entry_2`='0', `difficulty_entry_3`='0', `KillCredit1`='0', `KillCredit2`='0', `modelid1`='25801', `modelid2`='0', `modelid3`='0', `modelid4`='0', `name`='Enslaved Shieldmaiden', `femaleName`='', `subname`='', `TitleAlt`=NULL, `IconName`='', `gossip_menu_id`='0', `minlevel`='110', `maxlevel`='110', `HealthScalingExpansion`='6', `RequiredExpansion`='0', `VignetteID`='0', `faction`='14', `npcflag`='0', `speed_walk`='1', `speed_run`='1.28571', `scale`='1', `rank`='1', `dmgschool`='0', `BaseAttackTime`='2000', `RangeAttackTime`='2000', `BaseVariance`='1', `RangeVariance`='1', `unit_class`='1', `unit_flags`='32832', `unit_flags2`='0', `unit_flags3`='0', `dynamicflags`='0', `family`='0', `trainer_type`='0', `trainer_class`='0', `trainer_race`='0', `type`='7', `type_flags`='2097224', `type_flags2`='0', `lootid`='102104', `pickpocketloot`='0', `skinloot`='0', `resistance1`='0', `resistance2`='0', `resistance3`='0', `resistance4`='0', `resistance5`='0', `resistance6`='0', `spell1`='0', `spell2`='0', `spell3`='0', `spell4`='0', `spell5`='0', `spell6`='0', `spell7`='0', `spell8`='0', `VehicleId`='0', `mingold`='0', `maxgold`='0', `AIName`='SmartAI', `MovementType`='0', `InhabitType`='3', `HoverHeight`='1', `HealthModifier`='5', `HealthModifierExtra`='1', `ManaModifier`='1', `ManaModifierExtra`='1', `ArmorModifier`='1', `DamageModifier`='1', `ExperienceModifier`='1', `RacialLeader`='0', `movementId`='0', `RegenHealth`='1', `mechanic_immune_mask`='0', `flags_extra`='0', `ScriptName`='', `VerifiedBuild`='26124' WHERE (`entry`='102104');
