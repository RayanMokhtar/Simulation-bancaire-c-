#include <iostream>
#include <mysql.h>

int main() {
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) {
        std::cout << "Erreur mysql_init: " << mysql_error(conn) << std::endl;
        return 1;
    }

    // Configuration de connexion
    const char* host = "mysql-museevasion.alwaysdata.net";
    const char* user = "332768_wassim";
    const char* password = "Dellpower0+";
    const char* database = "museevasion_projetcpp";
    unsigned int port = 3306;

    std::cout << "Tentative de connexion à " << host << ":" << port << std::endl;
    std::cout << "Base de données: " << database << std::endl;
    std::cout << "Utilisateur: " << user << std::endl;

    if (!mysql_real_connect(conn, host, user, password, database, port, nullptr, 0)) {
        std::cout << "Erreur de connexion: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 1;
    }

    std::cout << "Connexion réussie !" << std::endl;

    // Test d'une requête simple
    if (mysql_query(conn, "SHOW TABLES")) {
        std::cout << "Erreur SHOW TABLES: " << mysql_error(conn) << std::endl;
    } else {
        std::cout << "Tables dans la base de données:" << std::endl;
        MYSQL_RES* result = mysql_store_result(conn);
        if (result) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                std::cout << "- " << row[0] << std::endl;
            }
            mysql_free_result(result);
        }
    }

    mysql_close(conn);
    std::cout << "Connexion fermée." << std::endl;
    return 0;
}
