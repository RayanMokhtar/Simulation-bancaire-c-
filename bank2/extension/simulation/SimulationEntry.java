package bank2.extension.simulation;

/**
 * This class regroups simulation entry parameters.
 */
public class SimulationEntry {
	private int simulationDuration;
	private int cashierCount;
	private int minServiceTime;
	private int maxServiceTime;
	private int clientArrivalInterval;
	private double priorityClientRate;
	private int clientPatienceTime;

	public SimulationEntry(int simulationDuration, int cashierCount, int minServiceTime, int maxServiceTime, int clientArrivalInterval, double priorityClientRate,
			int clientPatienceTime) {
		this.simulationDuration = simulationDuration;
		this.cashierCount = cashierCount;
		this.minServiceTime = minServiceTime;
		this.maxServiceTime = maxServiceTime;
		this.clientArrivalInterval = clientArrivalInterval;
		this.priorityClientRate = priorityClientRate;
		this.clientPatienceTime = clientPatienceTime;
	}

	public int getSimulationDuration() {
		return simulationDuration;
	}

	public int getCashierCount() {
		return cashierCount;
	}

	public int getMinServiceTime() {
		return minServiceTime;
	}

	public int getMaxServiceTime() {
		return maxServiceTime;
	}

	public int getClientArrivalInterval() {
		return clientArrivalInterval;
	}

	public double getPriorityClientRate() {
		return priorityClientRate;
	}

	public int getClientPatienceTime() {
		return clientPatienceTime;
	}
}
