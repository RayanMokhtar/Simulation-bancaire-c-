#ifndef SIMULATION_UTILITY_H
#define SIMULATION_UTILITY_H

#include <memory>
#include <random>
#include <string>
#include "AbstractOperation.h"
#include "Bank.h"

namespace bank2::extension::simulation {

using bank2::extension::client::AbstractOperation;  // Ajoutez cette ligne
using bank2::extension::bank::Bank;

int getRandomNumber(int min, int max);
void printServiceTimeTrace(int currentSystemTime, int serviceTime);
void printClientArrival(int currentSystemTime, bool served);
void printClientDeparture(int currentSystemTime);
void printClientDepartureWithoutBeingServed(int currentSystemTime);
void printBankStat(int currentSystemTime, const Bank& bank);
bool isPriorityClient(double priorityClientRate);
std::unique_ptr<AbstractOperation> getRandomOperation(int serviceTime);

} // namespace bank2::extension::simulation

#endif // SIMULATION_UTILITY_H