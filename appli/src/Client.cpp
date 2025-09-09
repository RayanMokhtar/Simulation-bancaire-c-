#include "Client.h"

namespace bank2::extension::client {

Client::Client(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : AbstractClient(arrivalTime, std::move(operation), patienceTime) {}

bool Client::isPriority() const { return false; }

} // namespace bank2::extension::client