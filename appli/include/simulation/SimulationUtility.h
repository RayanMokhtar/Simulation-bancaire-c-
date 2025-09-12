#ifndef SIMULATION_UTILITY_H
#define SIMULATION_UTILITY_H

#include <memory>
#include <random>
#include <string>
#include "client/AbstractOperation.h"
#include "bank/Bank.h"



int getRandomNumber(int min, int max);
void printServiceTimeTrace(int currentSystemTime, int serviceTime);
void printClientArrival(int currentSystemTime, bool served);
void printClientDeparture(int currentSystemTime);
void printClientDepartureWithoutBeingServed(int currentSystemTime);
void printBankStat(int currentSystemTime, const Bank& bank);
bool isPriorityClient(double priorityClientRate);
std::unique_ptr<AbstractOperation> getRandomOperation(int serviceTime);


#endif // SIMULATION_UTILITY_H