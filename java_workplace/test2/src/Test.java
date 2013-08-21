import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class Test {

    public static void main(String[] args) {

        int poolSize = 8;
        ScheduledExecutorService exec = Executors.newScheduledThreadPool(poolSize);
        exec.scheduleAtFixedRate(new Runnable() {

            long loop = 1;

            @Override
            public void run() {
                System.out.println("loop: " + loop++);

            }
        }, 1, 1, TimeUnit.SECONDS);

        try {
            TimeUnit.HOURS.sleep(1);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

}
