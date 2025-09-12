#ifndef SIMULATION_ENTRY_H
#define SIMULATION_ENTRY_H


class SimulationEntry {
private:
    const int simulationDuration;
    const int cashierCount;
    const int minServiceTime;
    const int maxServiceTime;
    const int clientArrivalInterval;
    const double priorityClientRate;
    const int clientPatienceTime;

public:
    SimulationEntry(int simDur, int cashCnt, int minServ, int maxServ, int arrInt, double priRate, int patTime);
    int getSimulationDuration() const;
    int getCashierCount() const;
    int getMinServiceTime() const;
    int getMaxServiceTime() const;
    int getClientArrivalInterval() const;
    double getPriorityClientRate() const;
    int getClientPatienceTime() const;
};


#endif // SIMULATION_ENTRY_H
