import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class TrafficController {
    Lock lock = new ReentrantLock();
    Condition left = lock.newCondition();
    Condition right = lock.newCondition();
    int control = 0;
    int carsOnBridge = 0;

    public void enterLeft() {
        lock.lock();
        try {
            while (carsOnBridge >= 1 && control != -1) {
                left.await();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            control = -1;
            carsOnBridge++;
            System.out.println("Left");
            lock.unlock();
        }
    }

    public void enterRight() {
        lock.lock();
        try {
            if (control != 1 && control != 0) {
                right.await();
            }
            while (carsOnBridge >= 1 && control != 1) {
                right.await();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            control = 1;
            carsOnBridge++;
            System.out.println("Right");
            lock.unlock();
        }
    }

    public void leaveLeft() {
        lock.lock();
        try {
            control = 0;
            left.signal();
            right.signal();
        } finally {
            System.out.println("Leave Left");
            carsOnBridge--;
            lock.unlock();
        }
    }

    public void leaveRight() {
        lock.lock();
        try {
            control = 0;
            right.signal();
            left.signal();
        } finally {
            System.out.println("Leave Right");
            carsOnBridge--;
            lock.unlock();
        }
    }
}