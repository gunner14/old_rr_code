package util;

import java.util.Date;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Counter {

	private static final long DELAY = 60;

	private String name;
	
	private ScheduledExecutorService exe = Executors.newSingleThreadScheduledExecutor();
	
	public Counter(final String name){
		this.name = name;
		exe.scheduleAtFixedRate(new Runnable() {

			@Override
			public void run() {
				
				if (startTime != null) {
					long during = System.currentTimeMillis() - startTime.getTime();
					int duringS = (int) (during /1000); 
					int c = count.get();
					System.out.println(name + " -- " + c + " requests in " + duringS + "s, " + c * 1.0 / duringS + "req/sec");
				}
			}
			
		}, DELAY, DELAY, TimeUnit.SECONDS);
	}
	
	private AtomicInteger count = new AtomicInteger();
	
	private Date startTime;
	
	private void initStartTime() {
		if (startTime == null) {
			synchronized (this) {
				if (startTime == null) {
					startTime = new Date();
				}
			}
		}
	}
	
	public int incr() {
		initStartTime();
		return count.incrementAndGet();
	}
	
	public int get() {
		return count.get();
	}
	
	public synchronized void reset() {
		count.set(0);
		startTime = null;
	}
	
	
}
