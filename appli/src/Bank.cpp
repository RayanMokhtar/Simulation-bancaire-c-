#include "Bank.h"

namespace bank2::extension::bank {

Bank::Bank(int cashierCount) {
    for (int i = 0; i < cashierCount; ++i) {
        cashiers.emplace_back(std::make_unique<Cashier>());
    }
}

const std::vector<std::unique_ptr<Cashier>>& Bank::getCashiers() const { return cashiers; }

Queue& Bank::getQueue() { return queue; }

Cashier* Bank::getFreeCashier() {
    for (auto& cashier : cashiers) {
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

} // namespace bank2::extension::bank