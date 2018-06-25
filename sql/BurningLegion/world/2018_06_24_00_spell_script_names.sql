DELETE FROM spell_script_names WHERE spell_id = 48504;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (48504, 'spell_dru_living_seed_heal');

DELETE FROM spell_script_names WHERE spell_id IN (214870, 209577);
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES 
(214870, 'spell_item_warbreaker'),
(209577, 'spell_item_warbreaker_damage');