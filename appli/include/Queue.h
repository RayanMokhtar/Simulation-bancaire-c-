#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <memory>
#include <string>
#include <vector>
#include "AbstractClient.h"

class Queue {
private:
    std::deque<std::shared_ptr<AbstractClient>> clients;

public:
    bool isEmpty() const;
    void addQueueLast(std::shared_ptr<AbstractClient> client);
    std::shared_ptr<AbstractClient> getQueueFirst();
    std::shared_ptr<AbstractClient> findPriorityClient() const;
    void removePriorityClient(std::shared_ptr<AbstractClient> client);
    void updateClientPatience();
    std::vector<std::shared_ptr<AbstractClient>> removeImpatientClients();
    std::string toString() const;
};

#endif // QUEUE_H