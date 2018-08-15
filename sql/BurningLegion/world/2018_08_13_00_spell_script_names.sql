DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dh_demonic_infusion'
, 'spell_dh_blade_turning'
, 'spell_pal_vindicator'
, 'spell_pal_blessed_stalwart'
, 'spell_pal_blessed_stalwart_trigger'
, 'spell_pal_painful_truths'
, 'spell_mage_brain_freeze'
, 'spell_dru_ferocious_bite');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(247254, 'spell_dh_blade_turning'),
(236189, 'spell_dh_demonic_infusion'),
(200373, 'spell_pal_vindicator'),
(238133, 'spell_pal_blessed_stalwart'),
(242869, 'spell_pal_blessed_stalwart_trigger'),
(209341, 'spell_pal_painful_truths'),
(190446, 'spell_mage_brain_freeze'),
(22568, 'spell_dru_ferocious_bite');