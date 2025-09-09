#include "AbstractOperation.h"

namespace bank2::extension::client {

AbstractOperation::AbstractOperation(int serviceTime) : serviceTime(serviceTime) {}

int AbstractOperation::getServiceTime() const {
    return serviceTime;
}

} // namespace bank2::extension::client