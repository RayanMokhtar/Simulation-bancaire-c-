package bank2.extension.client;

public class Consultation extends AbstractOperation {

	public Consultation(int serviceTime) {
		super(serviceTime);
	}

	@Override
	public boolean isUrgent() {
		return false;
	}
	
	@Override
	public String toString() {
		return "Operation : Consultation";
	}
}
