#include "Database.h"

bool Database::Connect(const std::string& user, const std::string& password) {
    try {
        sql::Driver* driver = sql::mariadb::get_driver_instance();
        sql::SQLString url("jdbc:mariadb://localhost:3306/eindopdracht");
        sql::Properties properties({
            {"user", user.c_str()},
            {"password", password.c_str()}
        });

        conn.reset(driver->connect(url, properties));
        return true;
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
        return false;
    }
}

const std::string& Database::GetLastError() const {
    return lastError;
}

std::vector<ListItem> Database::GetBrands() {
    std::vector<ListItem> brands;
    lastError.clear();

    if (conn == nullptr) { 
        lastError = "Not connected to the database.";
        return brands;
    }

    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> result(
            stmt->executeQuery("SELECT ID, Naam FROM tblMerk ORDER BY Naam"));

        while (result->next()) {
            sql::SQLString name = result->getString("Naam");
            brands.push_back({ result->getInt("ID"), name.c_str() });
        }
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
    }

    return brands;
}

std::vector<ListItem> Database::GetTypes(int brandID) {
    std::vector<ListItem> types;
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return types;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
            "SELECT ID, Naam FROM tblType WHERE MerkID = ? ORDER BY Naam"));
        stmt->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

        while (result->next()) {
            sql::SQLString name = result->getString("Naam");
            types.push_back({ result->getInt("ID"), name.c_str() });
        }
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
    }

    return types;
}

BrandLocation Database::GetBrandLocation(int brandID) {
    BrandLocation location;
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return location;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "SELECT tblLand.Naam AS Land, tblKantoor.Stad "
                "FROM tblMerk "
                "JOIN tblKantoor "
                "ON tblMerk.KantoorID = tblKantoor.ID "
                "JOIN tblLand "
                "ON tblKantoor.LandID = tblLand.ID "
                "WHERE tblMerk.ID = ?"
            )
        );

        stmt->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

        if (result->next()) {
            location.country = result->getString("Land").c_str();
            location.city = result->getString("Stad").c_str();
        }
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
    }

    return location;
}


CarDetails Database::GetCarDetails(int typeID) {
    CarDetails details;
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return details;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "SELECT "
                "tblLand.Naam AS Land, "
                "tblKantoor.Stad AS Stad, "
                "tblMerk.Naam AS Merk, "
                "tblType.Naam AS Type, "
                "tblType.Paardenkracht AS PK "
                "FROM tblType "
                "JOIN tblMerk "
                "ON tblType.MerkID = tblMerk.ID "
                "JOIN tblKantoor "
                "ON tblMerk.KantoorID = tblKantoor.ID "
                "JOIN tblLand "
                "ON tblKantoor.LandID = tblLand.ID "
                "WHERE tblType.ID = ?"
            )
        );

        stmt->setInt(1, typeID);

        std::unique_ptr<sql::ResultSet> result(
            stmt->executeQuery()
        );

        if (result->next()) {
            details.country = result->getString("Land").c_str();
            details.city = result->getString("Stad").c_str();
            details.brand = result->getString("Merk").c_str();
            details.type = result->getString("Type").c_str();
            details.pk = result->getInt("PK");
        }
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
    }

    return details;
}

// ===== Paste at the bottom of Database.cpp =====

TypeEdit Database::GetTypeForEdit(int typeID) {
    TypeEdit type;
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return type;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
            "SELECT t.ID, t.MerkID, m.Naam AS MerkNaam, t.Naam, t.Paardenkracht "
            "FROM tblType t "
            "JOIN tblMerk m ON m.ID = t.MerkID "
            "WHERE t.ID = ?"));
        stmt->setInt(1, typeID);

        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

        if (result->next()) {
            type.id = result->getInt("ID");
            type.brandID = result->getInt("MerkID");
            type.brandName = result->getString("MerkNaam").c_str();
            type.name = result->getString("Naam").c_str();
            type.pk = result->getInt("Paardenkracht");
        }
        else {
            lastError = "This type no longer exists.";
        }
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
    }

    return type;
}

bool Database::AddType(int brandID, const std::string& name, int pk) {
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return false;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
            "INSERT INTO tblType (MerkID, Naam, Paardenkracht) VALUES (?, ?, ?)"));
        stmt->setInt(1, brandID);
        stmt->setString(2, name.c_str());
        stmt->setInt(3, pk);
        stmt->executeUpdate();
        return true;
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
        return false;
    }
}

bool Database::EditType(const TypeEdit& original, const std::string& newName, int newPk) {
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return false;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
            "UPDATE tblType SET Naam = ?, Paardenkracht = ? "
            "WHERE ID = ? AND Naam = ? AND Paardenkracht = ?"));
        stmt->setString(1, newName.c_str());
        stmt->setInt(2, newPk);
        stmt->setInt(3, original.id);
        stmt->setString(4, original.name.c_str());
        stmt->setInt(5, original.pk);

        if (stmt->executeUpdate() == 1) {
            return true;
        }
        lastError = "This type was changed or deleted by someone else. Close this window and open it again.";
        return false;
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
        return false;
    }
}

bool Database::DeleteType(const TypeEdit& original) {
    lastError.clear();

    if (conn == nullptr) {
        lastError = "Not connected to the database.";
        return false;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
            "DELETE FROM tblType WHERE ID = ? AND Naam = ? AND Paardenkracht = ?"));
        stmt->setInt(1, original.id);
        stmt->setString(2, original.name.c_str());
        stmt->setInt(3, original.pk);

        if (stmt->executeUpdate() == 1) {
            return true;
        }
        lastError = "This type was changed or deleted by someone else. Close this window and open it again.";
        return false;
    }
    catch (const sql::SQLException& e) {
        lastError = e.what();
        return false;
    }
}
