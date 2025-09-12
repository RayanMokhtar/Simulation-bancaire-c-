#include "AbstractClient.h"

AbstractClient::AbstractClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : arrivalTime(arrivalTime), operation(std::move(operation)), patienceTime(patienceTime),
      serviceStartTime(0), departureTime(0) {}

int AbstractClient::getArrivalTime() const { return arrivalTime; }
int AbstractClient::getDepartureTime() const { return departureTime; }
void AbstractClient::setDepartureTime(int departureTime) { this->departureTime = departureTime; }
int AbstractClient::getServiceStartTime() const { return serviceStartTime; }
void AbstractClient::setServiceStartTime(int serviceStartTime) { this->serviceStartTime = serviceStartTime; }
const AbstractOperation& AbstractClient::getOperation() const { return *operation; }
void AbstractClient::reducePatience() { if (patienceTime > 0) --patienceTime; }
bool AbstractClient::isPatient() const { return patienceTime > 0 || operation->isUrgent(); }
std::string AbstractClient::toString() const {
    return "Client[arrival: " + std::to_string(arrivalTime) + ", patience time: " + std::to_string(patienceTime) + ", " + operation->toString() + "]";
}

