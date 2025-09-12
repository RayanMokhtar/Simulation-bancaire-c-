#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <memory>
#include <stdexcept>

#ifdef WITH_PERSISTENCE
#include <mysql.h>

class DatabaseException : public std::runtime_error {
public:
    DatabaseException(const std::string& message) : std::runtime_error(message) {}
};

class DatabaseConnection {
private:
    MYSQL* connection;
    std::string host;
    std::string username;
    std::string password;
    std::string database;
    unsigned int port;
    bool connected;

public:
    DatabaseConnection(const std::string& host = "localhost", 
                      const std::string& username = "root",
                      const std::string& password = "",
                      const std::string& database = "simulation_bancaire",
                      unsigned int port = 3306);
    
    ~DatabaseConnection();
    
    // Méthodes de connexion
    bool connect();
    void disconnect();
    bool isConnected() const { return connected; }
    
    // Méthodes d'exécution de requêtes
    bool executeQuery(const std::string& query);
    MYSQL_RES* executeSelect(const std::string& query);
    unsigned long long getLastInsertId();
    
    // Méthodes utilitaires
    std::string escapeString(const std::string& str);
    std::string getError() const;
    
    // Méthodes de transaction
    bool beginTransaction();
    bool commit();
    bool rollback();
    
    // Singleton pattern pour connection globale
    static DatabaseConnection& getInstance();
    
    // Méthode pour créer une connexion à partir d'un fichier de config
    static DatabaseConnection createFromConfig(const std::string& configFile = "config/database.conf", 
                                              const std::string& section = "database");
    
private:
    void initializeConnection();
};

#else
// Version stub sans MySQL pour compilation sans persistance
class DatabaseException : public std::runtime_error {
public:
    DatabaseException(const std::string& message) : std::runtime_error(message) {}
};

class DatabaseConnection {
public:
    DatabaseConnection(const std::string& = "", const std::string& = "", 
                      const std::string& = "", const std::string& = "", 
                      unsigned int = 0) {}
    ~DatabaseConnection() {}
    bool connect() { return false; }
    void disconnect() {}
    bool isConnected() const { return false; }
    static DatabaseConnection& getInstance() { static DatabaseConnection instance; return instance; }
};
#endif

#endif
