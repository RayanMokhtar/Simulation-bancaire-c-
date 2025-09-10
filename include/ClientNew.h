#ifndef CLIENT_NEW_H
#define CLIENT_NEW_H

#include "AbstractClient.h"

class ClientNew : public AbstractClient {
public:
    ClientNew(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    
    bool isPriority() const override { return false; }
};

#endif
