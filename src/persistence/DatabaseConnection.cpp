#include "persistence/DatabaseConnection.h"
#include "persistence/DatabaseConfig.h"
#include <iostream>
#include <cstring>

DatabaseConnection::DatabaseConnection(const std::string& host, 
                                     const std::string& username,
                                     const std::string& password,
                                     const std::string& database,
                                     unsigned int port)
    : connection(nullptr), host(host), username(username), 
      password(password), database(database), port(port), connected(false) {
    initializeConnection();
}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

void DatabaseConnection::initializeConnection() {
    connection = mysql_init(nullptr);
    if (!connection) {
        throw DatabaseException("Failed to initialize MySQL connection");
    }
    
    // Configuration des options de connexion
    bool reconnect = 1;
    mysql_options(connection, MYSQL_OPT_RECONNECT, &reconnect);
    
    // Timeout de connexion
    unsigned int timeout = 10;
    mysql_options(connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    
    // Charset UTF-8
    mysql_options(connection, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    
    // Effectuer la connexion directement
    MYSQL* result = mysql_real_connect(
        connection, 
        host.c_str(), 
        username.c_str(), 
        password.c_str(), 
        database.c_str(), 
        port, 
        nullptr, 
        CLIENT_MULTI_STATEMENTS
    );
    
    if (result) {
        connected = true;
        std::cout << "Connected to MySQL database: " << database << std::endl;
    } else {
        std::cerr << "MySQL connection failed: " << mysql_error(connection) << std::endl;
        throw DatabaseException("Failed to connect to MySQL: " + std::string(mysql_error(connection)));
    }
}

bool DatabaseConnection::connect() {
    if (connected) {
        return true;
    }
    
    if (!connection) {
        initializeConnection();
    }
    
    MYSQL* result = mysql_real_connect(
        connection, 
        host.c_str(), 
        username.c_str(), 
        password.c_str(), 
        database.c_str(), 
        port, 
        nullptr, 
        CLIENT_MULTI_STATEMENTS
    );
    
    if (result) {
        connected = true;
        std::cout << "Connected to MySQL database: " << database << std::endl;
        return true;
    } else {
        std::cerr << "MySQL connection failed: " << mysql_error(connection) << std::endl;
        return false;
    }
}

void DatabaseConnection::disconnect() {
    if (connection) {
        mysql_close(connection);
        connection = nullptr;
        connected = false;
        std::cout << "Disconnected from MySQL database" << std::endl;
    }
}

bool DatabaseConnection::executeQuery(const std::string& query) {
    if (!connected || !connection) {
        return false;
    }
    
    int result = mysql_real_query(connection, query.c_str(), query.length());
    if (result != 0) {
        std::cerr << "Query execution failed: " << mysql_error(connection) << std::endl;
        std::cerr << "Query: " << query << std::endl;
        return false;
    }
    
    return true;
}

MYSQL_RES* DatabaseConnection::executeSelect(const std::string& query) {
    if (!executeQuery(query)) {
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result && mysql_field_count(connection) > 0) {
        std::cerr << "Failed to retrieve result set: " << mysql_error(connection) << std::endl;
        return nullptr;
    }
    
    return result;
}

unsigned long long DatabaseConnection::getLastInsertId() {
    if (!connected || !connection) {
        return 0;
    }
    return mysql_insert_id(connection);
}

std::string DatabaseConnection::escapeString(const std::string& str) {
    if (!connected || !connection) {
        return str;
    }
    
    char* escaped = new char[str.length() * 2 + 1];
    unsigned long length = mysql_real_escape_string(connection, escaped, str.c_str(), str.length());
    std::string result(escaped, length);
    delete[] escaped;
    return result;
}

std::string DatabaseConnection::getError() const {
    if (connection) {
        return std::string(mysql_error(connection));
    }
    return "No connection";
}

bool DatabaseConnection::beginTransaction() {
    return executeQuery("START TRANSACTION");
}

bool DatabaseConnection::commit() {
    return executeQuery("COMMIT");
}

bool DatabaseConnection::rollback() {
    return executeQuery("ROLLBACK");
}

DatabaseConnection& DatabaseConnection::getInstance() {
    static bool initialized = false;
    static DatabaseConnection instance = []() {
        try {
            DatabaseConfig config("config/database.conf");
            if (!config.loadConfig()) {
                std::cerr << "WARNING: Failed to load config file, using default values" << std::endl;
                return DatabaseConnection();  // valeurs par défaut
            }
            
            return DatabaseConnection(
                config.getHost("database"),
                config.getUsername("database"),
                config.getPassword("database"),
                config.getDatabase("database"),
                config.getPort("database")
            );
        } catch (const std::exception& e) {
            std::cerr << "WARNING: Error loading database config: " << e.what() << std::endl;
            return DatabaseConnection();  // valeurs par défaut en cas d'erreur
        }
    }();
    
    return instance;
}

// Nouvelle méthode statique pour créer une connexion à partir du fichier de config
DatabaseConnection DatabaseConnection::createFromConfig(const std::string& configFile, const std::string& section) {
    DatabaseConfig config(configFile);
    if (!config.loadConfig()) {
        throw DatabaseException("Failed to load database configuration: " + config.getConfigError());
    }
    
    return DatabaseConnection(
        config.getHost(section),
        config.getUsername(section),
        config.getPassword(section),
        config.getDatabase(section),
        config.getPort(section)
    );
}
