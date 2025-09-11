#include "persistence/DatabaseConfig.h"
#include <iostream>
#include <algorithm>

DatabaseConfig::DatabaseConfig(const std::string& configFilePath) 
    : configFile(configFilePath), loaded(false) {
}

bool DatabaseConfig::loadConfig() {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        lastError = "Cannot open config file: " + configFile;
        return false;
    }
    
    std::string line;
    std::string currentSection;
    
    while (std::getline(file, line)) {
        trim(line);
        
        // Ignorer les lignes vides et les commentaires
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Section header
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            trim(currentSection);
            continue;
        }
        
        // Key-value pair
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            trim(key);
            trim(value);
            
            if (!currentSection.empty()) {
                config[currentSection][key] = value;
            }
        }
    }
    
    file.close();
    loaded = true;
    
    // Validation de base
    if (!validateConfig()) {
        loaded = false;
        return false;
    }
    
    std::cout << "Database configuration loaded from: " << configFile << std::endl;
    return true;
}

std::string DatabaseConfig::getHost(const std::string& section) const {
    return getValue(section, "host", "localhost");
}

unsigned int DatabaseConfig::getPort(const std::string& section) const {
    std::string portStr = getValue(section, "port", "3306");
    try {
        return static_cast<unsigned int>(std::stoul(portStr));
    } catch (const std::exception&) {
        return 3306;
    }
}

std::string DatabaseConfig::getUsername(const std::string& section) const {
    return getValue(section, "username", "root");
}

std::string DatabaseConfig::getPassword(const std::string& section) const {
    return getValue(section, "password", "");
}

std::string DatabaseConfig::getDatabase(const std::string& section) const {
    return getValue(section, "database", "simulation_bancaire");
}

unsigned int DatabaseConfig::getTimeout() const {
    std::string timeoutStr = getValue("connection", "timeout", "10");
    try {
        return static_cast<unsigned int>(std::stoul(timeoutStr));
    } catch (const std::exception&) {
        return 10;
    }
}

std::string DatabaseConfig::getCharset() const {
    return getValue("connection", "charset", "utf8mb4");
}

bool DatabaseConfig::getAutoReconnect() const {
    std::string reconnectStr = getValue("connection", "auto_reconnect", "true");
    std::transform(reconnectStr.begin(), reconnectStr.end(), reconnectStr.begin(), ::tolower);
    return reconnectStr == "true" || reconnectStr == "1" || reconnectStr == "yes";
}

bool DatabaseConfig::isLoggingEnabled() const {
    std::string loggingStr = getValue("debug", "enable_logging", "false");
    std::transform(loggingStr.begin(), loggingStr.end(), loggingStr.begin(), ::tolower);
    return loggingStr == "true" || loggingStr == "1" || loggingStr == "yes";
}

bool DatabaseConfig::isQueryLoggingEnabled() const {
    std::string queryLoggingStr = getValue("debug", "log_queries", "false");
    std::transform(queryLoggingStr.begin(), queryLoggingStr.end(), queryLoggingStr.begin(), ::tolower);
    return queryLoggingStr == "true" || queryLoggingStr == "1" || queryLoggingStr == "yes";
}

std::string DatabaseConfig::getLogFile() const {
    return getValue("debug", "log_file", "simulation_db.log");
}

std::string DatabaseConfig::getValue(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    auto sectionIt = config.find(section);
    if (sectionIt != config.end()) {
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            return keyIt->second;
        }
    }
    return defaultValue;
}

bool DatabaseConfig::hasSection(const std::string& section) const {
    return config.find(section) != config.end();
}

bool DatabaseConfig::hasKey(const std::string& section, const std::string& key) const {
    auto sectionIt = config.find(section);
    if (sectionIt != config.end()) {
        return sectionIt->second.find(key) != sectionIt->second.end();
    }
    return false;
}

bool DatabaseConfig::validateConfig() const {
    // Vérifier que la section database existe
    if (!hasSection("database")) {
        lastError = "Missing [database] section in config file";
        return false;
    }
    
    // Vérifier les paramètres obligatoires
    if (getValue("database", "host").empty()) {
        lastError = "Missing or empty 'host' parameter in [database] section";
        return false;
    }
    
    if (getValue("database", "database").empty()) {
        lastError = "Missing or empty 'database' parameter in [database] section";
        return false;
    }
    
    return true;
}

std::string DatabaseConfig::getConfigError() const {
    return lastError;
}

void DatabaseConfig::trim(std::string& str) {
    // Supprimer les espaces en début
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    
    // Supprimer les espaces en fin
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}
