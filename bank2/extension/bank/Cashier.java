package bank2.extension.bank;

import bank2.extension.client.AbstractClient;

/**
 * We need to identify a cashier for statistics. Information about the current service is also very important.
 */
public class Cashier {
	private AbstractClient servingClient = null;
	private int remainingServiceTime = 0;

	public boolean isFree() {
		return servingClient == null;
	}

	/**
	 * Makes the cashier work for one unit time.
	 */
	public void work() {
		if (remainingServiceTime > 0) {
			remainingServiceTime--;
		}
	}

	/**
	 * Verifies if the cashier finishes the current work.
	 * 
	 * @return true if the cashier just finished the current work
	 */
	public boolean serviceFinished() {
		return servingClient != null && remainingServiceTime == 0;
	}

	/**
	 * Begins the service of a new client.
	 * 
	 * @param servingClient the client to be served
	 * @param serviceTime   the generated random service time
	 */
	public void serve(AbstractClient servingClient) {
		this.servingClient = servingClient;
		int serviceTime = servingClient.getOperation().getServiceTime();
		remainingServiceTime = serviceTime;
	}

	public AbstractClient getServingClient() {
		return servingClient;
	}

	public void setServingClient(AbstractClient servingClient) {
		this.servingClient = servingClient;
	}

	public String toString() {
		return "Cashier[Remains : " + remainingServiceTime + "]";
	}

}
