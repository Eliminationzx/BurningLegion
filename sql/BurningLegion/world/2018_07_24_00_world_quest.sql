---fix bug quest 37656
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('90487', '0', '3', '0', '64', '0', '100', '0', '0', '0', '0', '0', '', '33', '8888883', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'quest');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('90487', '0', '4', '0', '64', '0', '100', '0', '0', '0', '0', '0', '', '41', '1', '35', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'despawn stop spawn');

---fix bug quest 36811
UPDATE `gameobject_template` SET `Data3`=35000 WHERE (`entry`=239676);