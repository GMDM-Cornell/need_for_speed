@include "C:/Users/Corne/AppData/Roaming/DBeaverData/workspace6/General/Scripts/stored_nfs.sql"

USE need_for_speed;

INSERT INTO country (name)
SELECT DISTINCT
    TRIM(SUBSTRING_INDEX(headquarters, ',', -1))
FROM staging_vehicle_data
WHERE headquarters IS NOT NULL
  AND TRIM(headquarters) <> ''
  AND headquarters LIKE '%,%'
  AND NOT EXISTS (
      SELECT 1
      FROM country
      WHERE country.name =
          TRIM(SUBSTRING_INDEX(staging_vehicle_data.headquarters, ',', -1))
  );

INSERT INTO city (country_id, name)
SELECT DISTINCT
    country.id,
    TRIM(SUBSTRING_INDEX(staging_vehicle_data.headquarters, ',', 1))
FROM staging_vehicle_data
INNER JOIN country
    ON country.name =
       TRIM(SUBSTRING_INDEX(staging_vehicle_data.headquarters, ',', -1))
WHERE staging_vehicle_data.headquarters IS NOT NULL
  AND TRIM(staging_vehicle_data.headquarters) <> ''
  AND staging_vehicle_data.headquarters LIKE '%,%'
  AND NOT EXISTS (
      SELECT 1
      FROM city
      WHERE city.country_id = country.id
        AND city.name =
            TRIM(SUBSTRING_INDEX(staging_vehicle_data.headquarters, ',', 1))
  );

INSERT INTO brand (city_id, name)
SELECT
    MAX(city.id),
    staging_vehicle_data.brand_name
FROM staging_vehicle_data
LEFT JOIN country
    ON country.name =
       TRIM(SUBSTRING_INDEX(staging_vehicle_data.headquarters, ',', -1))
LEFT JOIN city
    ON city.country_id = country.id
   AND city.name =
       TRIM(SUBSTRING_INDEX(staging_vehicle_data.headquarters, ',', 1))
WHERE staging_vehicle_data.brand_name IS NOT NULL
  AND TRIM(staging_vehicle_data.brand_name) <> ''
GROUP BY staging_vehicle_data.brand_name
ON DUPLICATE KEY UPDATE
    city_id = COALESCE(VALUES(city_id), city_id);

UPDATE brand
SET logo = LOAD_FILE(
    CONCAT('C:/School/Databases/Data/Logos/', name, '.png')
)
WHERE logo IS NULL;

INSERT INTO vehicle_type (brand_id, name)
SELECT DISTINCT
    brand.id,
    TRIM(staging_vehicle_data.vehicle_type_name)
FROM staging_vehicle_data
INNER JOIN brand
    ON brand.name = staging_vehicle_data.brand_name
WHERE staging_vehicle_data.vehicle_type_name IS NOT NULL
  AND TRIM(staging_vehicle_data.vehicle_type_name) <> ''
  AND NOT EXISTS (
      SELECT 1
      FROM vehicle_type
      WHERE vehicle_type.brand_id = brand.id
        AND vehicle_type.name =
            TRIM(staging_vehicle_data.vehicle_type_name)
  );

INSERT INTO model (vehicle_type_id, name, power_ps)

SELECT
    imported.vehicle_type_id,
    imported.model_name,
    imported.power_ps

FROM (
    SELECT
        vehicle_type.id AS vehicle_type_id,
        NULLIF(TRIM(staging_vehicle_data.model_name), '') AS model_name,
        MAX(staging_vehicle_data.power_ps) AS power_ps

    FROM staging_vehicle_data

    INNER JOIN brand
        ON brand.name = staging_vehicle_data.brand_name

    INNER JOIN vehicle_type
        ON vehicle_type.brand_id = brand.id
       AND vehicle_type.name = staging_vehicle_data.vehicle_type_name

    WHERE staging_vehicle_data.power_ps IS NOT NULL

    GROUP BY
        vehicle_type.id,
        NULLIF(TRIM(staging_vehicle_data.model_name), '')

    HAVING COUNT(DISTINCT staging_vehicle_data.power_ps) = 1

) AS imported

LEFT JOIN model
    ON model.vehicle_type_id = imported.vehicle_type_id
   AND model.name <=> imported.model_name

WHERE model.id IS NULL;


DROP TABLE staging_vehicle_data;
