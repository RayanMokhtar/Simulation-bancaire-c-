// filepath: c:\Users\mokht\Desktop\cpp\Simulation-bancaire-c-\appli\src\Queue.cpp
#include "Queue.h"
#include <algorithm>

bool Queue::isEmpty() const { return clients.empty(); }

void Queue::addQueueLast(std::shared_ptr<AbstractClient> client) { clients.push_back(client); }

std::shared_ptr<AbstractClient> Queue::getQueueFirst() {
    if (!clients.empty()) {
        auto client = clients.front();
        clients.pop_front();
        return client;
    }
    return nullptr;
}

std::shared_ptr<AbstractClient> Queue::findPriorityClient() const {
    for (const auto& client : clients) {
        if (client->isPriority()) return client;
    }
    return nullptr;
}

void Queue::removePriorityClient(std::shared_ptr<AbstractClient> client) {
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
}

void Queue::updateClientPatience() {
    for (auto& client : clients) client->reducePatience();
}

std::vector<std::shared_ptr<AbstractClient>> Queue::removeImpatientClients() {
    std::vector<std::shared_ptr<AbstractClient>> removeList;
    for (auto it = clients.begin(); it != clients.end(); ) {
        if (!(*it)->isPatient()) {
            removeList.push_back(*it);
            it = clients.erase(it);
        } else ++it;
    }
    return removeList;
}

std::string Queue::toString() const {
    std::string result = "Queue size[" + std::to_string(clients.size()) + "]: ";
    for (const auto& client : clients) {
        result += client->toString() + "-->";
    }
    return result;
}