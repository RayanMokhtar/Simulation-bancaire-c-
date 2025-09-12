#ifndef ABSTRACT_CLIENT_H
#define ABSTRACT_CLIENT_H

#include <memory>
#include <string>
#include "AbstractOperation.h"


class AbstractClient {
protected:
    int arrivalTime;
    int serviceStartTime;
    int departureTime;
    std::unique_ptr<AbstractOperation> operation;
    int patienceTime;

public:
    AbstractClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    virtual ~AbstractClient() = default;

    virtual bool isPriority() const = 0;
    int getArrivalTime() const;
    int getDepartureTime() const;
    void setDepartureTime(int departureTime);
    int getServiceStartTime() const;
    void setServiceStartTime(int serviceStartTime);
    const AbstractOperation& getOperation() const;
    void reducePatience();
    bool isPatient() const;
    virtual std::string toString() const;
};



#endif // ABSTRACT_CLIENT_H