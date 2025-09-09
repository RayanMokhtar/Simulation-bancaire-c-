// filepath: c:\Users\mokht\Desktop\cpp\Simulation-bancaire-c-\appli\include\Queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <memory>
#include <string>
#include <vector>
#include "AbstractClient.h"

namespace bank2::extension::bank {

using bank2::extension::client::AbstractClient;  // Ajoutez cette ligne

class Queue {
private:
    std::deque<std::unique_ptr<AbstractClient>> clients;

public:
    bool isEmpty() const;
    void addQueueLast(std::unique_ptr<AbstractClient> client);
    std::unique_ptr<AbstractClient> getQueueFirst();
    AbstractClient* findPriorityClient() const;
    void removePriorityClient(AbstractClient* client);
    void updateClientPatience();
    std::vector<std::unique_ptr<AbstractClient>> removeImpatientClients();
    std::string toString() const;
};

} // namespace bank2::extension::bank

#endif // QUEUE_H