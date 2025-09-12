package bank2.extension.simulation;

import java.util.List;

import bank2.extension.bank.Bank;
import bank2.extension.bank.Cashier;
import bank2.extension.bank.Queue;
import bank2.extension.client.AbstractClient;
import bank2.extension.client.AbstractOperation;
import bank2.extension.client.Client;
import bank2.extension.client.VIPClient;

/**
 * System simulation. Access point of all related information.
 */
public class Simulation {
	private Bank bank;
	private StatisticManager statisticManager;
	private SimulationEntry simulationEntry;

	public Simulation(SimulationEntry simulationEntry) {
		this.simulationEntry = simulationEntry;
		int cashierCount = simulationEntry.getCashierCount();
		bank = new Bank(cashierCount);
		statisticManager = new StatisticManager();
	}

	public void simulate() {
		int simulationDuration = simulationEntry.getSimulationDuration();
		int clientArrivalInterval = simulationEntry.getClientArrivalInterval();
		for (int currentSystemTime = 0; currentSystemTime <= simulationDuration; currentSystemTime++) {

			statisticManager.simulationDurationRecord();
			SimulationUtility.printBankStat(currentSystemTime, bank);

			updateBank(currentSystemTime);

			boolean newClientArrival = newClientArrival(clientArrivalInterval, currentSystemTime);
			if (newClientArrival) {
				int serviceTime = generateRandomServiceTime();
				AbstractOperation operation = SimulationUtility.getRandomOperation(serviceTime);
				AbstractClient client;
				double priorityClientRate = simulationEntry.getPriorityClientRate();
				if (SimulationUtility.isPriorityClient(priorityClientRate)) {
					client = new VIPClient(currentSystemTime, operation, simulationEntry.getClientPatienceTime());
				} else {
					client = new Client(currentSystemTime, operation, simulationEntry.getClientPatienceTime());
				}

				Cashier freeCashier = bank.getFreeCashier();
				if (freeCashier == null) {
					SimulationUtility.printClientArrival(currentSystemTime, false);
					Queue queue = bank.getQueue();
					queue.addQueueLast(client);
				} else {
					SimulationUtility.printClientArrival(currentSystemTime, true);
					serveClient(currentSystemTime, freeCashier, client);
				}
			}
		}
	}

	private void updateBank(int currentSystemTime) {
		List<Cashier> cashiers = bank.getCashiers();
		Queue queue = bank.getQueue();
		for (Cashier cashier : cashiers) {

			if (!cashier.isFree()) {
				statisticManager.cashierOccupationRecord();
			}

			cashier.work();

			if (cashier.serviceFinished()) {
				// Leaving client
				AbstractClient leavingClient = cashier.getServingClient();
				leavingClient.setDepartureTime(currentSystemTime);
				SimulationUtility.printClientDeparture(currentSystemTime);
				statisticManager.registerServedClient(leavingClient);

				cashier.setServingClient(null);

				// Serve a client in the queue
				if (!queue.isEmpty()) {
					AbstractClient nextClient;

					nextClient = queue.findPriorityClient();
					if (nextClient == null) {
						nextClient = queue.getQueueFirst();
					} else {
						queue.removePriorityClient(nextClient);
					}
					serveClient(currentSystemTime, cashier, nextClient);
				}
			}
		}

		// Leaving impatient clients
		queue.updateClientPatience();
		List<AbstractClient> impatientClients = queue.removeImpatientClients();
		for (AbstractClient client : impatientClients) {
			client.setDepartureTime(currentSystemTime);
			statisticManager.registerNonServedClient(client);
			SimulationUtility.printClientDepartureWithoutBeingServed(currentSystemTime);
		}
	}

	private void serveClient(int currentSystemTime, Cashier cashier, AbstractClient client) {
		client.setServiceStartTime(currentSystemTime);
		AbstractOperation operation = client.getOperation();
		int serviceTime = operation.getServiceTime();
		cashier.serve(client);
		SimulationUtility.printServiceTimeTrace(currentSystemTime, serviceTime);
	}

	private int generateRandomServiceTime() {
		int minServiceTime = simulationEntry.getMinServiceTime();
		int maxServiceTime = simulationEntry.getMaxServiceTime();
		int randomServiceTime = SimulationUtility.getRandomNumber(minServiceTime, maxServiceTime);
		return randomServiceTime;
	}

	private boolean newClientArrival(int clientArrivalInterval, int simIter) {
		return simIter % clientArrivalInterval == 0;
	}

	public String simulationResults() {
		StringBuffer results = new StringBuffer();
		results.append("########## Simulation results : #####################\n");
		results.append("Simulation Duration : " + simulationEntry.getSimulationDuration() + "\n");
		results.append("Served client count : " + statisticManager.servedClientCount() + "\n");
		results.append("Average client waiting time : " + statisticManager.calculateAverageClientWaitingTime() + " \n");
		results.append("Average client service time : " + statisticManager.calculateAverageClientServiceTime() + " \n");
		results.append("Cashier occupation rate : "
				+ statisticManager.calculateAverageCashierOccupationRate(simulationEntry.getCashierCount()) + " % \n");
		results.append("Non-Served client count : " + statisticManager.nonServedClientCount() + "\n");
		results.append("Client satisfaction rate : " + statisticManager.calculateClientSatisfactionRate() + " %");
		return results.toString();
	}

}
