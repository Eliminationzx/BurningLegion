UPDATE creature_template SET npcflag = 1, gossip_menu_id = 44468, ScriptName='npc_premium_minion' WHERE entry=84915;

INSERT INTO `gossip_menu_option` VALUES 
('44468', '0', '0', 'Repair all items', '0', '1', '1', '0'),
('44468', '1', '0', 'Morph me!', '0', '1', '1', '0'),
('44468', '2', '0', 'Demorph me!', '0', '1', '1', '0'),
('44468', '3', '0', 'Open bank', '0', '1', '1', '0'),
('44468', '4', '0', 'Reset my talents', '0', '1', '1', '0'),
('44468', '5', '0', 'Open all fly paths', '0', '1', '1', '0'),
('44468', '6', '0', 'Return to home', '0', '1', '1', '0'),
('44468', '7', '0', 'Buff me!', '0', '1', '1', '0'),
('44468', '8', '0', 'Find my friend', '0', '1', '1', '0'),
('44468', '9', '0', 'Change my race', '0', '1', '1', '0'),
('44468', '10', '0', 'Change my faction', '0', '1', '1', '0'),
('44468', '11', '0', 'Customize my character', '0', '1', '1', '0'),
('44468', '12', '0', 'Change personal rates', '0', '1', '1', '0'),
('44468', '13', '0', 'Rename my character', '0', '1', '1', '0');

INSERT INTO `gossip_menu_option_box` VALUES 
('44468', '8', '1', '0', 'Please type friend name', '0'),
('44468', '12', '1', '0', 'You can change personal rates from 1 to 100', '0');