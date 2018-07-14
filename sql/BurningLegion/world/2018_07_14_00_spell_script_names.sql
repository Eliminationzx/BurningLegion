DELETE FROM spell_script_names WHERE spell_id = 196923;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (196923, 'spell_pal_fervent_martyr');

DELETE FROM spell_script_names WHERE spell_id = 204147;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (204147, 'spell_hun_windburst');

DELETE FROM areatrigger_template WHERE Id = 10713;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES 
('10713','0','0','2','2','0','0','0','0','at_hun_windburst','26124');

DELETE FROM spell_script_names WHERE scriptname in ("spell_pal_aura_of_sacrifice", "spell_pal_aura_of_sacrifice_ally");
INSERT INTO spell_script_names VALUES
(210372, "spell_pal_aura_of_sacrifice_ally");

DELETE FROM spell_areatrigger WHERE SpellMiscId = 6777;
INSERT INTO spell_areatrigger (SpellMiscId, AreaTRiggerId) VALUES
(6777, 100102);

DELETE FROM areatrigger_template WHERE Id = 100102;
INSERT INTO areatrigger_template VALUES
(100102, 0, 4, 10, 10, 0, 0, 0, 0, "at_pal_aura_of_sacrifice", 0);