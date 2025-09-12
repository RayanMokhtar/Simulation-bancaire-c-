package bank2.extension.client;

public abstract class AbstractOperation {
	private int serviceTime;

	public AbstractOperation(int serviceTime) {
		this.serviceTime = serviceTime;
	}

	public int getServiceTime() {
		return serviceTime;
	}

	public abstract boolean isUrgent();
}
