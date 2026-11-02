CREATE OR REPLACE VIEW vw_need_for_speed AS
SELECT
    brand.name AS Brand,
    vehicle_type.name AS Type,
    model.name AS Model,
    model.power_ps AS `Vermogen [pk]`,
    CONCAT(city.name, ', ', country.name) AS Hoofdkantoor

FROM model

INNER JOIN vehicle_type
    ON vehicle_type.id = model.vehicle_type_id

INNER JOIN brand
    ON brand.id = vehicle_type.brand_id

LEFT JOIN city
    ON city.id = brand.city_id

LEFT JOIN country
    ON country.id = city.country_id;