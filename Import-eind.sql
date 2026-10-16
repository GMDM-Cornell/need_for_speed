INSERT INTO tblLand (Naam)
SELECT DISTINCT TRIM(SUBSTRING_INDEX(Hoofdkantoor, ',', -1))
FROM tblImport;

INSERT INTO tblKantoor (LandID, Stad)
SELECT DISTINCT l.ID, TRIM(SUBSTRING_INDEX(i.Hoofdkantoor, ',', 1))
FROM tblImport i
JOIN tblLand l ON l.Naam = TRIM(SUBSTRING_INDEX(i.Hoofdkantoor, ',', -1));

INSERT INTO tblMerk (KantoorID, Naam)
SELECT DISTINCT k.ID, i.Merk
FROM tblImport i
JOIN tblLand l ON l.Naam = TRIM(SUBSTRING_INDEX(i.Hoofdkantoor, ',', -1))
JOIN tblKantoor k ON k.LandID = l.ID
                 AND k.Stad = TRIM(SUBSTRING_INDEX(i.Hoofdkantoor, ',', 1));

UPDATE tblImport SET Paardenkracht = `Vermogen [pk]`;

SELECT COUNT(*) AS rijen, COUNT(Paardenkracht) AS pk FROM tblImport;

INSERT INTO tblType (MerkID, Naam, Paardenkracht)
SELECT m.ID, i.Type, i.Paardenkracht
FROM tblImport i
JOIN tblMerk m ON m.Naam = i.Merk;

ALTER TABLE tblType
ADD COLUMN KW INT GENERATED ALWAYS AS (ROUND(Paardenkracht * 0.7355)) STORED;

DROP TABLE tblImport;

CREATE VIEW vwNeedForSpeed AS
SELECT
    m.Naam                      AS `Merk`,
    t.Naam                      AS `Type`,
    t.Paardenkracht             AS `Vermogen [pk]`,
    t.KW                        AS `Vermogen [kW]`,
    CONCAT(k.Stad, ', ', l.Naam) AS `Hoofdkantoor`
FROM tblType t
JOIN tblMerk m    ON m.ID = t.MerkID
JOIN tblKantoor k ON k.ID = m.KantoorID
JOIN tblLand l    ON l.ID = k.LandID
ORDER BY m.Naam, t.Paardenkracht;