#ifndef CASHIER_H
#define CASHIER_H

#include "client/AbstractClient.h"
#include <memory>

class Cashier {
private:
    std::shared_ptr<AbstractClient> servingClient;
    int remainingServiceTime;

public:
    Cashier();
    
    // Methods matching Java version
    bool isFree() const { return servingClient == nullptr; }
    void work();
    bool serviceFinished() const;
    void serve(std::shared_ptr<AbstractClient> client);
    
    // Getters/Setters
    std::shared_ptr<AbstractClient> getServingClient() const { return servingClient; }
    void setServingClient(std::shared_ptr<AbstractClient> client) { servingClient = client; }
    int getRemainingServiceTime() const { return remainingServiceTime; }
    
    std::string toString() const;
};

#endif
