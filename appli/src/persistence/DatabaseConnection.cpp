#include "persistence/DatabaseConnection.h"

DatabaseConnection::DatabaseConnection() : connection(nullptr) {
    connection = mysql_init(nullptr);
    if (!connection) {
        throw DatabaseException("MySQL init failed");
    }
}

DatabaseConnection::~DatabaseConnection() {
    if (connection) {
        mysql_close(connection);
    }
}

bool DatabaseConnection::connect() {
    if (mysql_real_connect(connection, host.c_str(), username.c_str(), password.c_str(), database.c_str(), port, nullptr, 0)) {
        connected = true;
        std::cout << "Connected to DB" << std::endl;
        return true;
    } else {
        throw DatabaseException("Connection failed: " + std::string(mysql_error(connection)));
    }
}

void DatabaseConnection::disconnect() {
    if (connection) {
        mysql_close(connection);
        connected = false;
    }
}

MYSQL_RES* DatabaseConnection::executeSelect(const std::string& query) {
    if (!connected) return nullptr;
    if (mysql_query(connection, query.c_str()) == 0) {
        return mysql_store_result(connection);
    }
    return nullptr;
}

bool DatabaseConnection::executeQuery(const std::string& query) {
    if (!connected) return false;
    return mysql_query(connection, query.c_str()) == 0;
}

unsigned long long DatabaseConnection::getLastInsertId() {
    return mysql_insert_id(connection);
}