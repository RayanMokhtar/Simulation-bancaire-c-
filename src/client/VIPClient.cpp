#include "client/VIPClient.h"

VIPClient::VIPClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : AbstractClient(arrivalTime, std::move(operation), patienceTime) {
}

std::string VIPClient::toString() const {
    return "Priority " + AbstractClient::toString();
}
