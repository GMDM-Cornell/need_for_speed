USE need_for_speed;

CREATE TABLE IF NOT EXISTS database_audit (
    id BIGINT NOT NULL AUTO_INCREMENT,
    table_name VARCHAR(64) NOT NULL,
    record_id INT NOT NULL,
    action VARCHAR(10) NOT NULL,
    old_values JSON, -- JSON so all values can be stored
    new_values JSON,
    changed_at TIMESTAMP(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
    changed_by VARCHAR(255) NOT NULL,
    connection_id BIGINT UNSIGNED NOT NULL,
    PRIMARY KEY (id),
    INDEX idx_audit_table_record (table_name, record_id), -- Indexes that data for quicker select execution
    INDEX idx_audit_changed_at (changed_at)
);

DELIMITER // -- Instead of ; finishing a trigger it's //

DROP TRIGGER IF EXISTS country_after_insert//

CREATE TRIGGER country_after_insert AFTER INSERT ON country -- Create a trigger that activates afyer an insert happen on country
FOR EACH ROW -- Makes a log for each inserted row
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id) -- What goes into the audit log
    VALUES ('country', NEW.id, 'INSERT', NULL,
            JSON_OBJECT('name', NEW.name), USER(), CONNECTION_ID()); -- The values of what's inserted
END//


DROP TRIGGER IF EXISTS country_after_update//

CREATE TRIGGER country_after_update AFTER UPDATE ON country
FOR EACH ROW
BEGIN
    IF NOT (OLD.name <=> NEW.name) THEN -- If the old and new values are diffrent
        INSERT INTO database_audit
            (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
        VALUES ('country', NEW.id, 'UPDATE',
                JSON_OBJECT('name', OLD.name),
                JSON_OBJECT('name', NEW.name), USER(), CONNECTION_ID());
    END IF; -- Otherwise end
END//


DROP TRIGGER IF EXISTS country_after_delete//

CREATE TRIGGER country_after_delete AFTER DELETE ON country
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('country', OLD.id, 'DELETE',
            JSON_OBJECT('name', OLD.name), NULL, USER(), CONNECTION_ID());
END//


DROP TRIGGER IF EXISTS city_after_insert//

CREATE TRIGGER city_after_insert AFTER INSERT ON city
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('city', NEW.id, 'INSERT', NULL,
            JSON_OBJECT('country_id', NEW.country_id, 'name', NEW.name),
            USER(), CONNECTION_ID());
END//


DROP TRIGGER IF EXISTS city_after_update//

CREATE TRIGGER city_after_update AFTER UPDATE ON city
FOR EACH ROW
BEGIN
    IF NOT (OLD.country_id <=> NEW.country_id)
       OR NOT (OLD.name <=> NEW.name) THEN

        INSERT INTO database_audit
            (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
        VALUES ('city', NEW.id, 'UPDATE',
                JSON_OBJECT('country_id', OLD.country_id, 'name', OLD.name),
                JSON_OBJECT('country_id', NEW.country_id, 'name', NEW.name),
                USER(), CONNECTION_ID());
    END IF;
END//


DROP TRIGGER IF EXISTS city_after_delete//

CREATE TRIGGER city_after_delete AFTER DELETE ON city
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('city', OLD.id, 'DELETE',
            JSON_OBJECT('country_id', OLD.country_id, 'name', OLD.name),
            NULL, USER(), CONNECTION_ID());
END//


DROP TRIGGER IF EXISTS vehicle_type_after_insert//

CREATE TRIGGER vehicle_type_after_insert AFTER INSERT ON vehicle_type
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('vehicle_type', NEW.id, 'INSERT', NULL,
            JSON_OBJECT('brand_id', NEW.brand_id, 'name', NEW.name),
            USER(), CONNECTION_ID());
END//


DROP TRIGGER IF EXISTS vehicle_type_after_update//

CREATE TRIGGER vehicle_type_after_update AFTER UPDATE ON vehicle_type
FOR EACH ROW
BEGIN
    IF NOT (OLD.brand_id <=> NEW.brand_id)
       OR NOT (OLD.name <=> NEW.name) THEN

        INSERT INTO database_audit
            (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
        VALUES ('vehicle_type', NEW.id, 'UPDATE',
                JSON_OBJECT('brand_id', OLD.brand_id, 'name', OLD.name),
                JSON_OBJECT('brand_id', NEW.brand_id, 'name', NEW.name),
                USER(), CONNECTION_ID());
    END IF;
END//


DROP TRIGGER IF EXISTS vehicle_type_after_delete//

CREATE TRIGGER vehicle_type_after_delete AFTER DELETE ON vehicle_type
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('vehicle_type', OLD.id, 'DELETE',
            JSON_OBJECT('brand_id', OLD.brand_id, 'name', OLD.name),
            NULL, USER(), CONNECTION_ID());
END//


DROP TRIGGER IF EXISTS model_after_insert//

CREATE TRIGGER model_after_insert AFTER INSERT ON model
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('model', NEW.id, 'INSERT', NULL,
            JSON_OBJECT('vehicle_type_id', NEW.vehicle_type_id,
                        'name', NEW.name, 'power_ps', NEW.power_ps),
            USER(), CONNECTION_ID());
END//


DROP TRIGGER IF EXISTS model_after_update//

CREATE TRIGGER model_after_update AFTER UPDATE ON model
FOR EACH ROW
BEGIN
    IF NOT (OLD.vehicle_type_id <=> NEW.vehicle_type_id)
       OR NOT (OLD.name <=> NEW.name)
       OR NOT (OLD.power_ps <=> NEW.power_ps) THEN

        INSERT INTO database_audit
            (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
        VALUES ('model', NEW.id, 'UPDATE',
                JSON_OBJECT('vehicle_type_id', OLD.vehicle_type_id,
                            'name', OLD.name, 'power_ps', OLD.power_ps),
                JSON_OBJECT('vehicle_type_id', NEW.vehicle_type_id,
                            'name', NEW.name, 'power_ps', NEW.power_ps),
                USER(), CONNECTION_ID());
    END IF;
END//


DROP TRIGGER IF EXISTS model_after_delete//

CREATE TRIGGER model_after_delete AFTER DELETE ON model
FOR EACH ROW
BEGIN
    INSERT INTO database_audit
        (table_name, record_id, action, old_values, new_values, changed_by, connection_id)
    VALUES ('model', OLD.id, 'DELETE',
            JSON_OBJECT('vehicle_type_id', OLD.vehicle_type_id,
                        'name', OLD.name, 'power_ps', OLD.power_ps),
            NULL, USER(), CONNECTION_ID());
END//

DELIMITER ;

CREATE OR REPLACE VIEW vw_need_for_speed AS
SELECT
    brand.name AS Brand,
    vehicle_type.name AS `Type`,
    model.name AS Model,
    model.power_ps AS `Vermogen [PS]`,
    ROUND(model.power_ps * 0.73549875) AS `Vermogen [kW]`,
    CONCAT_WS(', ', city.name, country.name) AS Hoofdkantoor
FROM vehicle_type
INNER JOIN brand
    ON brand.id = vehicle_type.brand_id
LEFT JOIN model
    ON model.vehicle_type_id = vehicle_type.id
LEFT JOIN city
    ON city.id = brand.city_id
LEFT JOIN country
    ON country.id = city.country_id;
