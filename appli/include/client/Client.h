#ifndef CLIENT_H
#define CLIENT_H

#include "AbstractClient.h"


class Client : public AbstractClient {
public:
    Client(int arrivalTime, std::unique_ptr<AbstractOperation> operation, int patienceTime);
    bool isPriority() const override;
};


#endif // CLIENT_H