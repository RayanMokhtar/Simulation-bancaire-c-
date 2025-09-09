#ifndef CASHIER_H
#define CASHIER_H

#include <memory>
#include <string>
#include "AbstractClient.h"

namespace bank2::extension::bank {

class Cashier {
private:
    std::unique_ptr<AbstractClient> servingClient;
    int remainingServiceTime = 0;

public:
    bool isFree() const;
    void work();
    bool serviceFinished() const;
    void serve(std::unique_ptr<AbstractClient> client);
    AbstractClient* getServingClient() const;
    void setServingClient(std::unique_ptr<AbstractClient> client);
    std::string toString() const;
};

} // namespace bank2::extension::bank

#endif // CASHIER_H