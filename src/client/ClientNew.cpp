#include "client/ClientNew.h"

ClientNew::ClientNew(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime)
    : AbstractClient(arrivalTime, std::move(operation), patienceTime) {
}
