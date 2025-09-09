#include "Transfer.h"
using bank2::extension::client::AbstractOperation;


namespace bank2::extension::client {

Transfer::Transfer(int serviceTime) : AbstractOperation(serviceTime) {}

bool Transfer::isUrgent() const { return true; }

std::string Transfer::toString() const { return "Operation: Transfer"; }

} // namespace bank2::extension::client
