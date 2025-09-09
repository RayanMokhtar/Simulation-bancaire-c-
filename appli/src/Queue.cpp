#include "Queue.h"
#include <algorithm>
using bank2::extension::client::AbstractClient;

namespace bank2::extension::bank {

bool Queue::isEmpty() const { return clients.empty(); }

void Queue::addQueueLast(std::unique_ptr<AbstractClient> client) { clients.push_back(std::move(client)); }

std::unique_ptr<AbstractClient> Queue::getQueueFirst() {
    auto client = std::move(clients.front());
    clients.pop_front();
    return client;
}

AbstractClient* Queue::findPriorityClient() const {
    for (const auto& client : clients) {
        if (client->isPriority()) return client.get();
    }
    return nullptr;
}

void Queue::removePriorityClient(AbstractClient* client) {
    clients.erase(std::remove_if(clients.begin(), clients.end(),
        [client](const std::unique_ptr<AbstractClient>& c) { return c.get() == client; }), clients.end());
}

void Queue::updateClientPatience() {
    for (auto& client : clients) client->reducePatience();
}

std::vector<std::unique_ptr<AbstractClient>> Queue::removeImpatientClients() {
    std::vector<std::unique_ptr<AbstractClient>> removeList;
    for (auto it = clients.begin(); it != clients.end(); ) {
        if (!(*it)->isPatient()) {
            removeList.push_back(std::move(*it));
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

} // namespace bank2::extension::bank