/*REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('213170', 'spell_class_hall_panel');
DELETE FROM spell_area WHERE spell = 213170;
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `classmask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('213170', '7879', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- mage
('213170', '7813', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- warrior
('213170', '7558', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- druid
('213170', '7834', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- prist
('213170', '7902', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- monk
('213170', '7503', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- hunter
('213170', '8022', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- dh
('213170', '7638', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- paladin
('213170', '8011', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- rogue
('213170', '7875', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- warlock
('213170', '7745', '0', '0', '0', '0', '0', '2', '1', '64', '11'),  -- shaman
('213170', '7679', '0', '0', '0', '0', '0', '2', '1', '64', '11');  -- dk*/

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('213170', 'spell_class_hall_panel');
DELETE FROM spell_area WHERE spell = 213170;
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `quest_start_status`, `quest_end_status`) VALUES 
('213170', '7879', '0', '0', '0', '0', '2', '64', '11'),  -- mage
('213170', '7813', '0', '0', '0', '0', '2', '64', '11'),  -- warrior
('213170', '7558', '0', '0', '0', '0', '2', '64', '11'),  -- druid
('213170', '7834', '0', '0', '0', '0', '2', '64', '11'),  -- prist
('213170', '7902', '0', '0', '0', '0', '2', '64', '11'),  -- monk
('213170', '7503', '0', '0', '0', '0', '2', '64', '11'),  -- hunter
('213170', '8022', '0', '0', '0', '0', '2', '64', '11'),  -- dh
('213170', '7638', '0', '0', '0', '0', '2', '64', '11'),  -- paladin
('213170', '8011', '0', '0', '0', '0', '2', '64', '11'),  -- rogue
('213170', '7875', '0', '0', '0', '0', '2', '64', '11'),  -- warlock
('213170', '7745', '0', '0', '0', '0', '2', '64', '11'),  -- shaman
('213170', '7679', '0', '0', '0', '0', '2', '64', '11');  -

-- misc
--DELETE FROM `gossip_menu_option` WHERE `menu_id` in(20069, 20000) AND id = 2;
DELETE FROM `gossip_menu_option` WHERE `Menuid` in(20069, 20000) AND OptionIndex = 2;


-- it questers fail. ToDo: check all.
INSERT IGNORE INTO `creature_questender` (`id`, `quest`) VALUES ('93568', '43266');