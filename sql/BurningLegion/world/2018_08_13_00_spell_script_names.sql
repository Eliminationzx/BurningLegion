DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dh_demonic_infusion', 'spell_dh_blade_turning');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(247254, 'spell_dh_blade_turning'),
(236189, 'spell_dh_demonic_infusion');