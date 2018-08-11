DELETE FROM areatrigger_template WHERE Id = 1713;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES 
('1713','0','0','8','8','0','0','0','0','','26124');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dk_defile');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(152280, 'spell_dk_defile');