DELETE FROM spell_script_names WHERE spell_id = 206967;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (206967, 'spell_dk_will_of_the_necropolis');

DELETE FROM spell_script_names WHERE spell_id = 248580;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (248580, 'spell_war_shattered_defenses');

DELETE FROM spell_linked_spell WHERE spell_effect = -248625;
INSERT INTO spell_linked_spell (spell_trigger, spell_effect, type, comment) VALUES (12294, -248625, 0, 'Remove shattered defenses buff by mortal strike');
INSERT INTO spell_linked_spell (spell_trigger, spell_effect, type, comment) VALUES (163201, -248625, 0, 'Remove shattered defenses buff by execute');

DELETE FROM spell_script_names WHERE spell_id = 251879;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (251879, 'spell_war_t21_arms4p_bonus');

DELETE FROM spell_linked_spell WHERE spell_effect = -253383;
INSERT INTO spell_linked_spell (spell_trigger, spell_effect, type, comment) VALUES (1680, -253383, 0, 'Remove weighted blade buff by whirlwind');
INSERT INTO spell_linked_spell (spell_trigger, spell_effect, type, comment) VALUES (1464, -253383, 0, 'Remove weighted blade buff by slam');

DELETE FROM spell_script_names WHERE spell_id = 238147;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (238147, 'spell_war_executioners_precision');

DELETE FROM spell_linked_spell WHERE spell_effect = -242188;
INSERT INTO spell_linked_spell (spell_trigger, spell_effect, type, comment) VALUES (12294, -242188, 0, 'Remove executioners precision buff by mortal strike');

DELETE FROM spell_script_names WHERE ScriptName = 'spell_warr_second_wind_proc';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_warr_second_wind_damaged';
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES 
(29838, 'spell_warr_second_wind_proc'),
(202149, 'spell_warr_second_wind_damaged');