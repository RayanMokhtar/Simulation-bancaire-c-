#include "Withdraw.h"

namespace bank2::extension::client {

Withdraw::Withdraw(int serviceTime) : AbstractOperation(serviceTime) {}

bool Withdraw::isUrgent() const { return true; }

std::string Withdraw::toString() const { return "Operation: Withdraw"; }

} // namespace bank2::extension::client
