DELETE FROM spell_script_names WHERE spell_id = 197344;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (197344, 'spell_item_titanstrike');

UPDATE creature_template SET ScriptName = 'npc_hati' WHERE entry=100324;