package bank2.extension.client;

/**
 * We need to know when a client arrives, when the service starts and finishes.
 * When creating the client, only his arrival time can be defined.
 */
public abstract class AbstractClient {
	private int arrivalTime;
	private int serviceStartTime;
	private int departureTime;
	private AbstractOperation operation;
	private int patienceTime;

	public AbstractClient(int arrivalTime, AbstractOperation operation, int patienceTime) {
		this.arrivalTime = arrivalTime;
		this.operation = operation;
		this.patienceTime = patienceTime;
	}
	
	public abstract boolean isPriority();

	public int getArrivalTime() {
		return arrivalTime;
	}

	public int getDepartureTime() {
		return departureTime;
	}

	public void setDepartureTime(int departureTime) {
		this.departureTime = departureTime;
	}

	public int getServiceStartTime() {
		return serviceStartTime;
	}

	public void setServiceStartTime(int serviceStartTime) {
		this.serviceStartTime = serviceStartTime;
	}

	public AbstractOperation getOperation() {
		return operation;
	}

	public void reducePatience() {
		if (patienceTime > 0) {
			patienceTime--;
		}
	}

	public boolean isPatient() {
		return patienceTime > 0 || operation.isUrgent();
	}

	public String toString() {
		return "Client[arrival : " + arrivalTime + ", patience time : " + patienceTime + ", " + operation.toString() + "]";
	}
}
