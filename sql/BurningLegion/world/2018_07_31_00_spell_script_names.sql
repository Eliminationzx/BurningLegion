DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_power_of_goldrinn', 
'spell_dru_moon_and_stars', 
'spell_dru_power_of_the_archdruid',
'spell_dru_power_of_the_archdruid_trigger',
'spell_dru_mangle',
'spell_dru_clearcasting');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(202940, 'spell_dru_moon_and_stars'),
(189870, 'spell_dru_power_of_the_archdruid'),
(189877, 'spell_dru_power_of_the_archdruid_trigger'),
(16870, 'spell_dru_clearcasting'),
(33917, 'spell_dru_mangle'),
(202996, 'spell_dru_power_of_goldrinn');

DELETE FROM `spell_proc` WHERE `SpellId`=16870;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(16870, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0, 0x0, 0x1, 0x0, 0x8, 0, 0, 0, 0);

DELETE FROM `spell_proc` WHERE `SpellId`=189877;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(189877, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0, 0x0, 0x1, 0x0, 0x8, 0, 0, 0, 0);

DELETE FROM `spell_proc` WHERE `SpellId`=164547;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(164547, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0, 0x0, 0x1, 0x0, 0x8, 0, 0, 0, 0);