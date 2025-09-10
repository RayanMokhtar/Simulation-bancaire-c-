#ifndef VIP_CLIENT_H
#define VIP_CLIENT_H

#include "AbstractClient.h"

class VIPClient : public AbstractClient {
public:
    VIPClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    
    bool isPriority() const override { return true; }
    std::string toString() const override;
};

#endif
