---fix quest 40005
UPDATE `creature_template` SET `faction`=16, `unit_class`=2, `unit_flags`=0, `unit_flags2`=0, `type_flags`=0, `movementId`=0 WHERE (`entry`=92307);
UPDATE `creature_template` SET `gossip_menu_id`=999918,  `AIName`='SmartAI' WHERE (`entry`=97986);
UPDATE `creature_template` SET `gossip_menu_id`=999919, `AIName`='SmartAI' WHERE (`entry`=98190);
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild`) VALUES ('999918', '0', '0', 'Teleport me to the top', '0', '1', '1', '0');
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild`) VALUES ('999919', '0', '0', 'Let me down Vethir', '0', '1', '1', '0');
INSERT INTO `gossip_menu_option_locale` (`MenuId`, `OptionIndex`, `Locale`, `OptionText`, `BoxText`) VALUES ('999918', '0', 'ruRU', 'Отправь меня на верх Ветир', NULL);
INSERT INTO `gossip_menu_option_locale` (`MenuId`, `OptionIndex`, `Locale`, `OptionText`, `BoxText`) VALUES ('999919', '0', 'ruRU', 'Спусти меня вниз Ветир', NULL);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('97986', '0', '0', '0', '62', '0', '100', '0', '999918', '0', '0', '0', '', '62', '1220', '0', '0', '0', '0', '0', '7', '0', '0', '0', '2484.08', '952.097', '596.141', '3.96996', 'tp quest');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('98190', '0', '0', '0', '62', '0', '100', '0', '999919', '0', '0', '0', '', '62', '1220', '0', '0', '0', '0', '0', '7', '0', '0', '0', '2517.34', '976.213', '233.266', '3.89692', 'tp quest');

---fix quest 39405
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('210119131', '94228', '1220', '0', '0', '1', '0', '0', '0', '-1', '0', '0', '2737.49', '1583.95', '13.2277', '3.75884', '300', '0', '0', '870', '0', '0', '0', '0', '0', '0', '0', '', '0');

---fix quest 40002
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=280170);

---fix quest 37449
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (90255, 37449);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=286100);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277505);

---fix quest 41485
UPDATE ``creature_template` SET `npcflag`=16777217, `AIName`='SmartAI' WHERE (`entry`=103816);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('103816', '0', '1', '0', '64', '0', '100', '0', '0', '0', '0', '0', '', '41', '2550', '30', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'despawn stop spam');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('103816', '0', '0', '0', '64', '0', '100', '0', '0', '0', '0', '0', '', '33', '103816', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'w');
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES ('103816', '208883', '1', '0');


---fix quest 41474
UPDATE `gameobject_template` SET `Data0`=93, `Data3`=25000, `Data10`=0,  `AIName`='SmartGameObjectAI' WHERE (`entry`=248114);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('248114', '1', '0', '0', '70', '0', '100', '0', '2', '0', '0', '0', '', '33', '248114', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'quest');


---fix quest 38331
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=285319);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=285317);

---fix quest 38059
UPDATE `creature_template` SET `npcflag`=1,  `AIName`='SmartAI' WHERE (`entry`=90948);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('90948', '0', '0', '0', '64', '0', '100', '0', '0', '0', '0', '0', '', '33', '90948', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'quest');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('90948', '0', '1', '0', '64', '0', '100', '0', '0', '0', '0', '0', '', '41', '1', '25', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'despawn stop spam');

---fix quest 38060
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277473);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=278188);

---fix quest 38058
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277457);
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=277453);

---fix quest 39756
UPDATE `quest_objectives` SET `Flags`=4 (`ID`=279906);
