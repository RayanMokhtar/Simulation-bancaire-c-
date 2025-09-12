#include "Client.h"


Client::Client(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : AbstractClient(arrivalTime, std::move(operation), patienceTime) {}

bool Client::isPriority() const { return false; }

