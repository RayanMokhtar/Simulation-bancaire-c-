#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <mysql.h>
#include <stdexcept>
#include <iostream>

class DatabaseException : public std::runtime_error {
public:
    DatabaseException(const std::string& message) : std::runtime_error(message) {}
};

class DatabaseConnection {
private:
    MYSQL* connection;
    std::string host = "mysql-museevasion.alwaysdata.net";
    std::string username = "332768_wassim";
    std::string password = "Dellpower0+";
    std::string database = "museevasion_projetcpp";
    unsigned int port = 3306;
    bool connected = false;

public:
    DatabaseConnection();
    ~DatabaseConnection();
    bool connect();  // Initialise et connecte
    void disconnect();
    bool isConnected() const { return connected; }
    MYSQL_RES* executeSelect(const std::string& query);
    bool executeQuery(const std::string& query);
    unsigned long long getLastInsertId();
};

#endif // DATABASE_CONNECTION_H