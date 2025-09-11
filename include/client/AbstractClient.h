#ifndef ABSTRACT_CLIENT_H
#define ABSTRACT_CLIENT_H

#include "client/AbstractOperation.h"
#include <memory>
#include <string>

class AbstractClient {
private:
    int arrivalTime;
    int serviceStartTime;
    int departureTime;
    std::unique_ptr<AbstractOperation> operation;
    int patienceTime;

public:
    AbstractClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    virtual ~AbstractClient() = default;
    
    virtual bool isPriority() const = 0;
    
    // Getters
    int getArrivalTime() const { return arrivalTime; }
    int getDepartureTime() const { return departureTime; }
    int getServiceStartTime() const { return serviceStartTime; }
    AbstractOperation* getOperation() const { return operation.get(); }
    int getPatienceTime() const { return patienceTime; }
    
    // Setters
    void setDepartureTime(int departureTime) { this->departureTime = departureTime; }
    void setServiceStartTime(int serviceStartTime) { this->serviceStartTime = serviceStartTime; }
    
    // Methods
    void reducePatience();
    bool isPatient() const;
    
    virtual std::string toString() const;
};

#endif
