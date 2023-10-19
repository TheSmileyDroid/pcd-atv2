import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class TrafficController {
	Lock lock = new ReentrantLock();
	Condition left = lock.newCondition();
	Condition right = lock.newCondition();
	int carsOnBridge = 0;

	public void enterLeft() {
		lock.lock();
		try {
			while (carsOnBridge > 0) {
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
			while (carsOnBridge > 0) {
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
			right.signal();
			left.signal();
		} finally {
			lock.unlock();
		}
	}
}