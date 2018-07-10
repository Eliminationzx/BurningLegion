DELETE FROM spell_script_names WHERE spell_id = 209539;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (209539, 'spell_pal_light_of_the_titans');

DELETE FROM spell_script_names WHERE spell_id = 204241;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (204241, 'spell_pal_consecration_heal');

DELETE FROM areatrigger_template WHERE Id = 6013;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) values 
('6013','0','0','8','8','0','0','0','0','at_pal_aegis_of_light','26124');