package bank2.extension.simulation;

import bank2.extension.bank.Bank;
import bank2.extension.client.AbstractOperation;
import bank2.extension.client.Consultation;
import bank2.extension.client.Transfer;
import bank2.extension.client.Withdraw;

/**
 * Provides utility static methods for simulation.
 */
public class SimulationUtility {
	public static int getRandomNumber(int min, int max) {
		return (int) (Math.random() * (max + 1 - min)) + min;
	}

	public static void printServiceTimeTrace(int currentSystemTime, int serviceTime) {
		System.out.println("Start service for " + serviceTime + " time units.");
	}

	public static void printClientArrival(int currentSystemTime, boolean served) {
		if (served) {
			System.out.println("A new client arrives and is served immediately.");
		} else {
			System.out.println("A new client arrives and joins the queue.");
		}
	}

	public static void printClientDeparture(int currentSystemTime) {
		System.out.println("A client leaves after service.");
	}
	
	public static void printClientDepartureWithoutBeingServed(int currentSystemTime) {
		System.out.println("A client leaves without being served.");
	}

	public static void printBankStat(int currentSystemTime, Bank bank) {
		System.out.println("########### At time : " + currentSystemTime + " ##########");
		System.out.println(bank.toString());
	}

	public static boolean isPriorityClient(double priorityClientRate) {
		double random = Math.random();
		return random < priorityClientRate;
	}
	public static AbstractOperation getRandomOperation(int serviceTime) {
		int operationChoice = getRandomNumber(1, 3);
		AbstractOperation operation;
		switch (operationChoice) {
		case 1:
			operation = new Consultation(serviceTime);
			break;
		case 2:
			operation = new Transfer(serviceTime);
			break;
		case 3:
			operation = new Withdraw(serviceTime);
			break;
		default:
			operation = new Consultation(serviceTime);
			break;
		}
		return operation;
	}
}
