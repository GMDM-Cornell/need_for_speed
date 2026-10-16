
CREATE USER IF NOT EXISTS 'cornell'@'localhost'
IDENTIFIED BY 'login123';

GRANT ALL PRIVILEGES ON `eindopdracht`.*
TO 'cornell'@'localhost';


CREATE TABLE IF NOT EXISTS tblLand (
    ID INT NOT NULL AUTO_INCREMENT,
    Naam VARCHAR(255) NOT NULL,
    PRIMARY KEY (ID)
);

CREATE TABLE IF NOT EXISTS tblKantoor (
    ID INT NOT NULL AUTO_INCREMENT,
    LandID INT NOT NULL,
    Stad VARCHAR(255) NOT NULL,
    PRIMARY KEY (ID),
    FOREIGN KEY (LandID) REFERENCES tblLand(ID)
);

CREATE TABLE IF NOT EXISTS tblMerk (
    ID INT NOT NULL AUTO_INCREMENT,
    KantoorID INT NOT NULL,
    Naam VARCHAR(255) NOT NULL,
    PRIMARY KEY (ID),
    FOREIGN KEY (KantoorID) REFERENCES tblKantoor(ID)
);

CREATE TABLE IF NOT EXISTS tblType (
    ID INT NOT NULL AUTO_INCREMENT,
    MerkID INT NOT NULL,
    Naam VARCHAR(255) NOT NULL,
    Paardenkracht INT NOT NULL,
    PRIMARY KEY (ID),
    FOREIGN KEY (MerkID) REFERENCES tblMerk(ID)
);

CREATE TABLE IF NOT EXISTS tblImport (
    Merk VARCHAR(255),
    `Type` VARCHAR(255),
    Paardenkracht INT,
    Hoofdkantoor VARCHAR(255)
);
