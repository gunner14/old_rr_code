import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;



public class Filter implements Lock {
    
    static final int IDLE = -1;
    volatile int[] level;
    volatile int[] victim;
    volatile int size;
    
    public Filter(int threads) {
        size = threads;
        level = new int[threads];
        victim = new int[threads-1];
    }

    @Override
    public void lock() {
        
    }

    @Override
    public void lockInterruptibly() throws InterruptedException {
        // TODO Auto-generated method stub

    }

    @Override
    public Condition newCondition() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean tryLock() {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean tryLock(long arg0, TimeUnit arg1) throws InterruptedException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void unlock() {
        // TODO Auto-generated method stub

    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub

    }

}
