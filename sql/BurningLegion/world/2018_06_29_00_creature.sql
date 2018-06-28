---fix oplot paladin
DELETE FROM `creature` WHERE `id`=92148;
UPDATE `creature_template` SET `ScriptName`='npc_paladin_def' WHERE (`entry`=92148);
