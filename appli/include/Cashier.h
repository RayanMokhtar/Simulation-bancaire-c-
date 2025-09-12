// filepath: c:\Users\mokht\Desktop\cpp\Simulation-bancaire-c-\appli\include\Cashier.h
#ifndef CASHIER_H
#define CASHIER_H

#include <string>
#include <memory>
#include "AbstractClient.h"

class Cashier {
private:
    int remainingServiceTime = 0;
    std::shared_ptr<AbstractClient> servingClient = nullptr;

public:
    Cashier() = default;
    bool isFree() const;
    void work();
    bool serviceFinished() const;
    void serve(std::shared_ptr<AbstractClient> client);
    std::shared_ptr<AbstractClient> getServingClient() const;
    void setServingClient(std::shared_ptr<AbstractClient> client);
    std::string toString() const;
};

#endif // CASHIER_H