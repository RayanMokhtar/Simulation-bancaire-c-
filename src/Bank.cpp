#include "../include/Bank.h"
#include <sstream>

Bank::Bank(int cashierCount) : queue(std::make_unique<Queue>()) {
    for (int count = 1; count <= cashierCount; count++) {
        cashiers.push_back(std::make_unique<Cashier>());
    }
}

std::vector<Cashier*> Bank::getCashiers() {
    std::vector<Cashier*> result;
    for (auto& cashier : cashiers) {
        result.push_back(cashier.get());
    }
    return result;
}

Cashier* Bank::getFreeCashier() {
    for (auto& cashier : cashiers) {
        if (cashier->isFree()) {
            return cashier.get();
        }
    }
    return nullptr;
}

std::string Bank::toString() const {
    std::stringstream ss;
    ss << queue->toString() << "\n";
    ss << "Cashiers : ";
    for (const auto& cashier : cashiers) {
        ss << cashier->toString();
    }
    return ss.str();
}
