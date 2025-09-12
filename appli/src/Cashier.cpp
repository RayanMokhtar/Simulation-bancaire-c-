#include "Cashier.h"

bool Cashier::isFree() const { return servingClient == nullptr; }

void Cashier::work() { if (remainingServiceTime > 0) --remainingServiceTime; }

bool Cashier::serviceFinished() const { return servingClient != nullptr && remainingServiceTime == 0; }

void Cashier::serve(std::shared_ptr<AbstractClient> client) {
    servingClient = client;
    remainingServiceTime = servingClient->getOperation().getServiceTime();
}

std::shared_ptr<AbstractClient> Cashier::getServingClient() const { return servingClient; }

void Cashier::setServingClient(std::shared_ptr<AbstractClient> client) { servingClient = client; }

std::string Cashier::toString() const {
    return "Cashier[Remains: " + std::to_string(remainingServiceTime) + "]";
}