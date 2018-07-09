DELETE FROM spell_script_names WHERE spell_id = 192657;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (192657, 'spell_rog_bag_of_tricks');

DELETE FROM areatrigger_scripts WHERE entry = 9645;
INSERT INTO areatrigger_scripts (entry, ScriptName) VALUES 
(9645, 'at_rog_bag_of_tricks_areatrigger');