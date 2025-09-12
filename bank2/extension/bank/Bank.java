package bank2.extension.bank;

import java.util.ArrayList;
import java.util.List;

/**
 * A bank is composed of cashiers and a queue.
 */
public class Bank {
	private List<Cashier> cashiers = new ArrayList<Cashier>();
	private Queue queue = new Queue();

	public Bank(int cashierCount) {
		for (int count = 1; count <= cashierCount; count++) {
			Cashier cashier = new Cashier();
			cashiers.add(cashier);
		}
	}

	public List<Cashier> getCashiers() {
		return cashiers;
	}

	public Queue getQueue() {
		return queue;
	}

	public Cashier getFreeCashier() {
		for (Cashier cashier : cashiers) {
			if (cashier.isFree()) {
				return cashier;
			}
		}
		return null;
	}

	public String toString() {
		StringBuffer results = new StringBuffer();
		results.append(queue.toString() + "\n");
		results.append("Cashiers : ");
		for (Cashier cashier : cashiers) {
			results.append(cashier.toString());
		}
		return results.toString();
	}
}