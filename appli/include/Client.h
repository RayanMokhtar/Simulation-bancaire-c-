#ifndef CLIENT_H
#define CLIENT_H

#include "AbstractClient.h"

namespace bank2::extension::client {

class Client : public AbstractClient {
public:
    Client(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    bool isPriority() const override;
};

} // namespace bank2::extension::client

#endif // CLIENT_H