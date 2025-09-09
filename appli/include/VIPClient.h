#ifndef VIP_CLIENT_H
#define VIP_CLIENT_H

#include "AbstractClient.h"

namespace bank2::extension::client {

class VIPClient : public AbstractClient {
public:
    VIPClient(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    bool isPriority() const override;
    std::string toString() const override;
};

} // namespace bank2::extension::client

#endif // VIP_CLIENT_H