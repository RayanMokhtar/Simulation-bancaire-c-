package bank2.extension.client;

public class Client extends AbstractClient {

	public Client(int arrivalTime, AbstractOperation operation, int patienceTime) {
		super(arrivalTime, operation, patienceTime);		
	}

	@Override
	public boolean isPriority() {
		return false;
	}

}
