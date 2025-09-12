#include "AbstractOperation.h"


AbstractOperation::AbstractOperation(int serviceTime) : serviceTime(serviceTime) {}

int AbstractOperation::getServiceTime() const {
    return serviceTime;
}

