USE need_for_speed;

CREATE TABLE IF NOT EXISTS staging_vehicle_data (
    id INT NOT NULL AUTO_INCREMENT,
    brand_name VARCHAR(255) NOT NULL,
    vehicle_type_name VARCHAR(255) NOT NULL,
    model_name VARCHAR(255) NOT NULL,
    power_ps INT,
    headquarters VARCHAR(255),
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS country (
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(255) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE (name)
);

CREATE TABLE IF NOT EXISTS city (
    id INT NOT NULL AUTO_INCREMENT,
    country_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE (country_id, name),
    FOREIGN KEY (country_id)
        REFERENCES country(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS brand (
    id INT NOT NULL AUTO_INCREMENT,
    city_id INT,
    name VARCHAR(255) NOT NULL,
    logo MEDIUMBLOB,
    PRIMARY KEY (id),
    UNIQUE (name),
    FOREIGN KEY (city_id)
        REFERENCES city(id)
        ON UPDATE CASCADE
        ON DELETE SET NULL 
);

CREATE TABLE IF NOT EXISTS vehicle_type (
    id INT NOT NULL AUTO_INCREMENT,
    brand_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE (brand_id, name),
    FOREIGN KEY (brand_id)
        REFERENCES brand(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS model (
    id INT NOT NULL AUTO_INCREMENT,
    vehicle_type_id INT NOT NULL,
    name VARCHAR(255),
    power_ps INT NOT NULL,
    PRIMARY KEY (id),
    UNIQUE (vehicle_type_id, name),
    FOREIGN KEY (vehicle_type_id)
        REFERENCES vehicle_type(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
);
