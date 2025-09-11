#include "bank/FileAttente.h"
#include <sstream>
#include <algorithm>

Queue::Queue() {
}

void Queue::addQueueLast(std::shared_ptr<AbstractClient> client) {
    if (client) {
        clients.push_back(client);
    }
}

std::shared_ptr<AbstractClient> Queue::getQueueFirst() {
    if (!clients.empty()) {
        auto client = clients.front();
        clients.pop_front();
        return client;
    }
    return nullptr;
}

std::shared_ptr<AbstractClient> Queue::findPriorityClient() {
    for (auto& client : clients) {
        if (client->isPriority()) {
            return client;
        }
    }
    return nullptr;
}

void Queue::removePriorityClient(std::shared_ptr<AbstractClient> client) {
    auto it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end()) {
        clients.erase(it);
    }
}

void Queue::updateClientPatience() {
    for (auto& client : clients) {
        client->reducePatience();
    }
}

std::vector<std::shared_ptr<AbstractClient>> Queue::removeImpatientClients() {
    std::vector<std::shared_ptr<AbstractClient>> removeList;
    
    // Find impatient clients
    for (auto& client : clients) {
        if (!client->isPatient()) {
            removeList.push_back(client);
        }
    }
    
    // Remove them from the queue
    for (auto& client : removeList) {
        auto it = std::find(clients.begin(), clients.end(), client);
        if (it != clients.end()) {
            clients.erase(it);
        }
    }
    
    return removeList;
}

std::string Queue::toString() const {
    std::stringstream ss;
    ss << "Queue size[" << clients.size() << "] : ";
    for (const auto& client : clients) {
        ss << client->toString() << "-->";
    }
    return ss.str();
}
