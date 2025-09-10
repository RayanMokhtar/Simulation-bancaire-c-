#ifndef SIMULATION_ENTRY_H
#define SIMULATION_ENTRY_H

struct SimulationEntry {
    int simulationDuration;
    int cashierCount;
    int clientArrivalInterval;
    int minServiceTime;
    int maxServiceTime;
    double priorityClientRate;
    int clientPatienceTime;
    
    SimulationEntry(int duration, int cashiers, int interval, int minService, 
                   int maxService, double vipRate, int patience)
        : simulationDuration(duration), cashierCount(cashiers), 
          clientArrivalInterval(interval), minServiceTime(minService),
          maxServiceTime(maxService), priorityClientRate(vipRate),
          clientPatienceTime(patience) {}
    
    // Getters
    int getSimulationDuration() const { return simulationDuration; }
    int getCashierCount() const { return cashierCount; }
    int getClientArrivalInterval() const { return clientArrivalInterval; }
    int getMinServiceTime() const { return minServiceTime; }
    int getMaxServiceTime() const { return maxServiceTime; }
    double getPriorityClientRate() const { return priorityClientRate; }
    int getClientPatienceTime() const { return clientPatienceTime; }
};

#endif
