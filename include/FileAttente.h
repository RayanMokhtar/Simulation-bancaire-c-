#ifndef QUEUE_H
#define QUEUE_H

#include "AbstractClient.h"
#include <deque>
#include <memory>
#include <vector>

class Queue {
private:
    std::deque<std::shared_ptr<AbstractClient>> clients;

public:
    Queue();
    
    // Methods matching Java version
    bool isEmpty() const { return clients.empty(); }
    void addQueueLast(std::shared_ptr<AbstractClient> client);
    std::shared_ptr<AbstractClient> getQueueFirst();
    std::shared_ptr<AbstractClient> findPriorityClient();
    void removePriorityClient(std::shared_ptr<AbstractClient> client);
    void updateClientPatience();
    std::vector<std::shared_ptr<AbstractClient>> removeImpatientClients();
    
    int size() const { return static_cast<int>(clients.size()); }
    std::string toString() const;
};

#endif
