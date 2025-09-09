#include "Cashier.h"
#include "AbstractClient.h"


namespace bank2::extension::bank {

using bank2::extension::client::AbstractClient;  // Ajoutez cette ligne

bool Cashier::isFree() const { return !servingClient; }

void Cashier::work() { if (remainingServiceTime > 0) --remainingServiceTime; }

bool Cashier::serviceFinished() const { return servingClient && remainingServiceTime == 0; }

void Cashier::serve(std::unique_ptr<AbstractClient> client) {
    servingClient = std::move(client);
    remainingServiceTime = servingClient->getOperation().getServiceTime();
}

AbstractClient* Cashier::getServingClient() const { return servingClient.get(); }

void Cashier::setServingClient(std::unique_ptr<AbstractClient> client) { servingClient = std::move(client); }

std::string Cashier::toString() const {
    return "Cashier[Remains: " + std::to_string(remainingServiceTime) + "]";
}

} // namespace bank2::extension::bank