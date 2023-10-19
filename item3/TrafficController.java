import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class TrafficController {
	Lock lock = new ReentrantLock();
	Condition left = lock.newCondition();
	Condition right = lock.newCondition();
	int control = 0; // 0 = no control, 1 = right, -1 = left
	int carsOnBridge = 0;

	public void enterLeft() {
		lock.lock();
		try {
			while (((control != -1) && carsOnBridge > 0)) {
				left.await();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			carsOnBridge++;
			System.out.println("Left");
			lock.unlock();
		}
	}

	public void enterRight() {
		lock.lock();
		try {
			while ((control != 1) && carsOnBridge > 0) {
				right.await();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			carsOnBridge++;
			System.out.println("Right");
			lock.unlock();
		}
	}

	public void leaveLeft() {
		lock.lock();
		try {
			carsOnBridge--;
			System.out.println("Leave Left: " + carsOnBridge + " cars on bridge");
			left.signal();
			right.signal();
		} finally {
			lock.unlock();
		}
	}

	public void leaveRight() {
		lock.lock();
		try {
			carsOnBridge--;
			System.out.println("Leave Right: " + carsOnBridge + " cars on bridge");
			right.signal();
			left.signal();
		} finally {
			lock.unlock();
		}
	}
}