package bank2.extension.client;

public class VIPClient extends AbstractClient {

	public VIPClient(int arrivalTime, AbstractOperation operation, int patienceTime) {
		super(arrivalTime, operation, patienceTime);
	}

	@Override
	public boolean isPriority() {
		return true;
	}

	@Override
	public String toString() {
		return "Priority " + super.toString();
	}

}
