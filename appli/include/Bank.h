// filepath: c:\Users\mokht\Desktop\cpp\Simulation-bancaire-c-\appli\include\Bank.h
#ifndef BANK_H
#define BANK_H

#include <vector>
#include <string>
#include <memory>
#include "Cashier.h"
#include "Queue.h"

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

#endif // BANK_H