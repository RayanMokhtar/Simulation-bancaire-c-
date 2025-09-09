#ifndef CASHIER_H
#define CASHIER_H

#include <memory>
#include <string>
#include "AbstractClient.h"

namespace bank2::extension::bank {

// Définition d'un alias pour simplifier le code
using ClientPtr = std::unique_ptr<bank2::extension::client::AbstractClient>;

class Cashier {
private:
    int remainingServiceTime = 0;
    ClientPtr servingClient;  // Utilisation de l'alias

public:
    Cashier() = default;
    bool isFree() const;
    void work();
    bool serviceFinished() const;
    void serve(ClientPtr client);  // Utilisation de l'alias
    bank2::extension::client::AbstractClient* getServingClient() const;
    void setServingClient(ClientPtr client);  // Utilisation de l'alias
    std::string toString() const;
};

} // namespace bank2::extension::bank

#endif // CASHIER_H