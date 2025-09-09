#include "VIPClient.h"
using bank2::extension::client::AbstractClient;

namespace bank2::extension::client {

VIPClient::VIPClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : AbstractClient(arrivalTime, std::move(operation), patienceTime) {}

bool VIPClient::isPriority() const { return true; }

std::string VIPClient::toString() const {
    return "Priority " + AbstractClient::toString();
}

} // namespace bank2::extension::client