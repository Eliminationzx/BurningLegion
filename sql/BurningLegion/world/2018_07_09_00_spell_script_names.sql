DELETE FROM spell_script_names WHERE spell_id = 192657;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (192657, 'spell_rog_bag_of_tricks');

DELETE FROM areatrigger_template WHERE Id = 9645;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES 
('9645','0','0','8','8','0','0','0','0','at_rog_bag_of_tricks_areatrigger','26124');

DELETE FROM spell_script_names WHERE spell_id = 203797;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (203797, 'spell_pal_retribution_aura');

DELETE FROM spell_script_names WHERE spell_id = 774;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (774, 'spell_dru_rejuvenation');