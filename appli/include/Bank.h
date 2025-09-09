#ifndef BANK_H
#define BANK_H

#include <memory>
#include <string>
#include <vector>
#include "Cashier.h"
#include "Queue.h"

namespace bank2::extension::bank {

class Bank {
private:
    std::vector<std::unique_ptr<Cashier>> cashiers;
    Queue queue;

public:
    Bank(int cashierCount);
    const std::vector<std::unique_ptr<Cashier>>& getCashiers() const;
    Queue& getQueue();
    Cashier* getFreeCashier();
    std::string toString() const;
};

} // namespace bank2::extension::bank

#endif // BANK_H