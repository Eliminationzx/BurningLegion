---Fix paladin order hall guard
UPDATE `creature_template` SET `ScriptName`='npc_sanctum_of_light_guard' WHERE `entry` IN (92148, 92151);
