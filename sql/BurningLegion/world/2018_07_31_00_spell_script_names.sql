DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_power_of_goldrinn', 'spell_dru_moon_and_stars');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(202940, 'spell_dru_moon_and_stars'),
(202996, 'spell_dru_power_of_goldrinn');