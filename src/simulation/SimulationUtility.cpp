#include "simulation/SimulationUtility.h"
#include "client/Consultation.h"
#include "client/Transfer.h"
#include "client/Withdraw.h"
#include <random>
#include <iostream>

int SimulationUtility::getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::unique_ptr<AbstractOperation> SimulationUtility::getRandomOperation(int serviceTime) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);
    
    int operationType = dis(gen);
    switch (operationType) {
        case 0: return std::make_unique<Consultation>(serviceTime);
        case 1: return std::make_unique<Transfer>(serviceTime);
        case 2: return std::make_unique<Withdraw>(serviceTime);
        default: return std::make_unique<Consultation>(serviceTime);
    }
}

bool SimulationUtility::isPriorityClient(double priorityClientRate) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < priorityClientRate;
}

void SimulationUtility::printBankStat(int currentSystemTime, const Bank& bank) {
    std::cout << "Time " << currentSystemTime << ": " << bank.toString() << std::endl;
}

void SimulationUtility::printClientArrival(int currentSystemTime, bool servedImmediately) {
    std::cout << "Time " << currentSystemTime << ": Client arrived";
    if (servedImmediately) {
        std::cout << " - served immediately";
    } else {
        std::cout << " - added to queue";
    }
    std::cout << std::endl;
}

void SimulationUtility::printClientDeparture(int currentSystemTime) {
    std::cout << "Time " << currentSystemTime << ": Client departed after being served" << std::endl;
}

void SimulationUtility::printClientDepartureWithoutBeingServed(int currentSystemTime) {
    std::cout << "Time " << currentSystemTime << ": Client left without being served (impatient)" << std::endl;
}

void SimulationUtility::printServiceTimeTrace(int currentSystemTime, int serviceTime) {
    std::cout << "Time " << currentSystemTime << ": Service started, duration: " << serviceTime << std::endl;
}
