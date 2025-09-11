#include "client/AbstractClient.h"
#include <sstream>

AbstractClient::AbstractClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : arrivalTime(arrivalTime), serviceStartTime(-1), departureTime(-1), 
      operation(std::move(operation)), patienceTime(patienceTime) {
}

void AbstractClient::reducePatience() {
    if (patienceTime > 0) {
        patienceTime--;
    }
}

bool AbstractClient::isPatient() const {
    return patienceTime > 0 || operation->isUrgent();
}

std::string AbstractClient::toString() const {
    std::stringstream ss;
    ss << "Client[arrival : " << arrivalTime 
       << ", patience time : " << patienceTime 
       << ", " << operation->toString() << "]";
    return ss.str();
}
