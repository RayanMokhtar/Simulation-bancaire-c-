#include "SimulationUtility.h"
#include "Consultation.h"
#include "Transfer.h"
#include "Withdraw.h"
#include <iostream>


namespace bank2::extension::simulation {

using bank2::extension::client::Transfer;  
using bank2::extension::client::Withdraw; 
using bank2::extension::client::Consultation; 

std::mt19937 gen(std::random_device{}());

int getRandomNumber(int min, int max) {
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

void printServiceTimeTrace(int currentSystemTime, int serviceTime) {
    std::cout << "Start service for " << serviceTime << " time units." << std::endl;
}

void printClientArrival(int currentSystemTime, bool served) {
    if (served) {
        std::cout << "A new client arrives and is served immediately." << std::endl;
    } else {
        std::cout << "A new client arrives and joins the queue." << std::endl;
    }
}

void printClientDeparture(int currentSystemTime) {
    std::cout << "A client leaves after service." << std::endl;
}

void printClientDepartureWithoutBeingServed(int currentSystemTime) {
    std::cout << "A client leaves without being served." << std::endl;
}

void printBankStat(int currentSystemTime, const Bank& bank) {
    std::cout << "########### At time: " << currentSystemTime << " ##########" << std::endl;
    std::cout << bank.toString() << std::endl;
}

bool isPriorityClient(double priorityClientRate) {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    return dist(gen) < priorityClientRate;
}

std::unique_ptr<AbstractOperation> getRandomOperation(int serviceTime) {
    int choice = getRandomNumber(1, 3);
    switch (choice) {
        case 1: return std::make_unique<Consultation>(serviceTime);
        case 2: return std::make_unique<Transfer>(serviceTime);
        case 3: return std::make_unique<Withdraw>(serviceTime);
        default: return std::make_unique<Consultation>(serviceTime);
    }
}

} // namespace bank2::extension::simulation