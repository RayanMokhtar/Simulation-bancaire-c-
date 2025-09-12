// filepath: c:\Users\mokht\Desktop\cpp\Simulation-bancaire-c-\appli\src\Bank.cpp
#include "Bank.h"

Bank::Bank(int cashierCount) {
    for (int i = 0; i < cashierCount; ++i) {
        cashiers.push_back(std::make_unique<Cashier>());
    }
}

const std::vector<std::unique_ptr<Cashier>>& Bank::getCashiers() const { return cashiers; }

Queue& Bank::getQueue() { return queue; }

Cashier* Bank::getFreeCashier() {
    for (const auto& cashier : cashiers) {
        if (cashier->isFree()) return cashier.get();
    }
    return nullptr;
}

std::string Bank::toString() const {
    std::string result = queue.toString() + "\nCashiers: ";
    for (const auto& cashier : cashiers) {
        result += cashier->toString();
    }
    return result;
}