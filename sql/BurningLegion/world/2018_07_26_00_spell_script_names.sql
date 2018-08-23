DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_sha_lighthing_rod');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(421, 'spell_sha_lighthing_rod'),
(403, 'spell_sha_lighthing_rod');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_warr_shield_slam_reset');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(231834, 'spell_warr_shield_slam_reset');

DELETE FROM areatrigger_template WHERE Id = 10718;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES 
('10718','0','0','8','8','0','0','0','0','at_dh_sigil_of_chains','26124');