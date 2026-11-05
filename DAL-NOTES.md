# DAL — what changed

All SQL moved out of `Database` into one repository per table.
The windows no longer include `database.h` and know nothing about tables, columns or SQL.

This branch has no `I*Repository` interfaces: the windows hold the concrete repositories.
`IWindow` is still in place, so the "minimaal één interface" bullet stays covered.

```
Applicatie   App, the four windows, ImageManager         (no SQL, no table names)
   |
Repository   BrandRepository / TypeRepository / ModelRepository
             CityRepository  / CountryRepository       (business queries, returns entities)
   |
DAO part     the same classes: one table each, CRUD only
   |
Database     Database: owns the one connection
```

The deck (Les 5, slide 12) allows the repository and the DAO role in one class:
"in dit voorbeeld combineert KlantRepository de rol van DAO en repository".

## Files

| File | What it is |
|---|---|
| `dal/Entities.h` | one struct per table: `Country`, `City`, `Brand`, `VehicleType`, `Model` |
| `dal/BrandRepository.*` | brands incl. logo, and moving a brand's headquarters |
| `dal/TypeRepository.*` | vehicle types of one brand |
| `dal/ModelRepository.*` | models of one vehicle type |
| `dal/CityRepository.*` | city, with find-or-create on `UNIQUE(country_id, name)` |
| `dal/CountryRepository.*` | country, with find-or-create on `UNIQUE(name)` |
| `dal/RepositoryBase.*` | connection + error handling shared by the five repositories |
| `database.*` | now only `Connect`, `IsConnected`, `GetConnection`, `GetLastError` |

Windows take the concrete repositories (`BrandRepository&`). Everything else is unchanged:
SQL still lives only in `dal/`, and `BrandWindow.cpp` still has no query and no table name.
What you lose is the seam: there is no second type a window could be handed, so the
klassendiagram arrow ends on a class that owns a connection.

Two things the old code got wrong and this fixes:

- `BrandLocation` / `CarDetails` are gone. The windows build their text from `Brand`,
  `City`, `Country`, `Model` — the objects have the shape of the rows.
- kW comes from `Model::PowerKW()`, using the same constant as `ps_to_kw()` in
  `stored_nfs.sql`, so the GUI and `vw_need_for_speed` show the same number.

## Before you run it tomorrow

1. `main.cpp` connects to `need_for_speed` as `cornell` / `Password123`. If your VM uses
   other credentials, change that one line.
2. `create_nfs.sql` starts with `USE need_for_speed;` and never creates the database or the
   user. Add at the top:

   ```sql
   CREATE DATABASE IF NOT EXISTS need_for_speed;
   CREATE USER IF NOT EXISTS 'cornell'@'localhost' IDENTIFIED BY 'Password123';
   GRANT ALL PRIVILEGES ON need_for_speed.* TO 'cornell'@'localhost';
   ```

3. Nothing loads `NFS.csv` into `staging_vehicle_data`; add one `LOAD DATA LOCAL INFILE`
   (client needs `--local-infile=1`) or import the CSV through DBeaver.
4. `brand` has no audit triggers, but the GUI changes `brand.city_id`. Three more triggers
   like the ones on `city` close that.
5. The klassendiagram in `klassendiagram.drawio` still draws `IDatabase`, `EditTypeWindow`
   and `TypeEdit`, which no longer exist. It has to be redrawn before the assessment.

## Built and checked

- Every translation unit parses as C++20 with `-Wall -Wextra` (g++ 14), against the real
  ImGui 1.92.2b headers.
- The SQL was run against MariaDB 11.8: schema, import (40 models), view, and the exact
  statement sequences the repositories use, including the no-op update and the
  find-or-create re-run.
- Not compiled with MSVC here (Windows-only), and not run against the real connector
  library — first build on the VM is the real test.
