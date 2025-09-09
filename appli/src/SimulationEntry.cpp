#include "SimulationEntry.h"

namespace bank2::extension::simulation {

SimulationEntry::SimulationEntry(int simDur, int cashCnt, int minServ, int maxServ, int arrInt, double priRate, int patTime)
    : simulationDuration(simDur), cashierCount(cashCnt), minServiceTime(minServ), maxServiceTime(maxServ),
      clientArrivalInterval(arrInt), priorityClientRate(priRate), clientPatienceTime(patTime) {}

int SimulationEntry::getSimulationDuration() const { return simulationDuration; }
int SimulationEntry::getCashierCount() const { return cashierCount; }
int SimulationEntry::getMinServiceTime() const { return minServiceTime; }
int SimulationEntry::getMaxServiceTime() const { return maxServiceTime; }
int SimulationEntry::getClientArrivalInterval() const { return clientArrivalInterval; }
double SimulationEntry::getPriorityClientRate() const { return priorityClientRate; }
int SimulationEntry::getClientPatienceTime() const { return clientPatienceTime; }

} // namespace bank2::extension::simulation
