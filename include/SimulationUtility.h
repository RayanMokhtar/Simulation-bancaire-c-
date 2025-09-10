#ifndef SIMULATION_UTILITY_H
#define SIMULATION_UTILITY_H

#include "AbstractOperation.h"
#include "Bank.h"
#include <memory>

class SimulationUtility {
public:
    // Random generation methods
    static int getRandomNumber(int min, int max);
    static std::unique_ptr<AbstractOperation> getRandomOperation(int serviceTime);
    static bool isPriorityClient(double priorityClientRate);
    
    // Print methods
    static void printBankStat(int currentSystemTime, const Bank& bank);
    static void printClientArrival(int currentSystemTime, bool servedImmediately);
    static void printClientDeparture(int currentSystemTime);
    static void printClientDepartureWithoutBeingServed(int currentSystemTime);
    static void printServiceTimeTrace(int currentSystemTime, int serviceTime);
};

#endif
