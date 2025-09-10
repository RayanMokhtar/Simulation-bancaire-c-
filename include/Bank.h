#ifndef BANK_H
#define BANK_H

#include "Caissier.h"
#include "FileAttente.h"
#include <vector>
#include <memory>

class Bank {
private:
    std::vector<std::unique_ptr<Cashier>> cashiers;
    std::unique_ptr<Queue> queue;

public:
    Bank(int cashierCount);
    
    // Getters
    std::vector<Cashier*> getCashiers();
    Queue* getQueue() const { return queue.get(); }
    
    // Methods
    Cashier* getFreeCashier();
    
    std::string toString() const;
};

#endif
