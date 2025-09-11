#include "bank/Caissier.h"
#include <sstream>

Cashier::Cashier() : servingClient(nullptr), remainingServiceTime(0) {
}

void Cashier::work() {
    if (remainingServiceTime > 0) {
        remainingServiceTime--;
    }
}

bool Cashier::serviceFinished() const {
    return servingClient != nullptr && remainingServiceTime == 0;
}

void Cashier::serve(std::shared_ptr<AbstractClient> client) {
    servingClient = client;
    int serviceTime = client->getOperation()->getServiceTime();
    remainingServiceTime = serviceTime;
}

std::string Cashier::toString() const {
    std::stringstream ss;
    ss << "Cashier[Remains : " << remainingServiceTime << "]";
    return ss.str();
}
