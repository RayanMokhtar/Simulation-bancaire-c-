#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>

class DatabaseConfig {
private:
    std::map<std::string, std::map<std::string, std::string>> config;
    std::string configFile;
    bool loaded;

public:
    DatabaseConfig(const std::string& configFilePath = "config/database.conf");
    
    // Chargement de la configuration
    bool loadConfig();
    bool isLoaded() const { return loaded; }
    
    // Getters pour les paramètres de base de données
    std::string getHost(const std::string& section = "database") const;
    unsigned int getPort(const std::string& section = "database") const;
    std::string getUsername(const std::string& section = "database") const;
    std::string getPassword(const std::string& section = "database") const;
    std::string getDatabase(const std::string& section = "database") const;
    
    // Getters pour les paramètres de connexion
    unsigned int getTimeout() const;
    std::string getCharset() const;
    bool getAutoReconnect() const;
    
    // Getters pour les options de debug
    bool isLoggingEnabled() const;
    bool isQueryLoggingEnabled() const;
    std::string getLogFile() const;
    
    // Méthodes utilitaires
    std::string getValue(const std::string& section, const std::string& key, const std::string& defaultValue = "") const;
    bool hasSection(const std::string& section) const;
    bool hasKey(const std::string& section, const std::string& key) const;
    
    // Validation
    bool validateConfig() const;
    std::string getConfigError() const;

private:
    void trim(std::string& str);
    mutable std::string lastError;
};

#endif
