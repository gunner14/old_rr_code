package com.xiaonei.tools.profiler;

import java.util.concurrent.atomic.AtomicInteger;

import com.xiaonei.xce.log.XceLogger;

public class ProfilerPoint {
	private String name;
	private AtomicInteger invocationCount = new AtomicInteger(0);//总调用次数
	private AtomicInteger time = new AtomicInteger(0);//总消费时间
	private AtomicInteger initCount = new AtomicInteger(0);//初始调用次数
	private int initCountThreshold = 2000;//初始调用次数阀值
	private long maxTime = Long.MIN_VALUE;
	private long minTime = Long.MAX_VALUE;
	private int upTime = 5000;//过大的时间，需要被抛弃
	private int downTime = 10;//过小的时间，需要被抛弃
	private volatile static int TimeLogThreshold = 1;
	private String domain;
	private String url;
	private static ThreadLocal<java.lang.Long> current = new ThreadLocal<Long>();
	private static final String[] logCategories = new String[]{"com","xiaonei","profiler","log"};
	
	public ProfilerPoint(String name,String domain){
		this.name = name;
		this.domain = domain;
		this.url = domain+" "+name+" ";
	}
	public static void setTimeLogThread(int time){
		TimeLogThreshold = time;
	}
	public String getName(){
		return name;
	}
	public long getMaxTime(){
		return maxTime;
	}
	public long getMinTime(){
		return minTime;
	}
	public String getDomain(){
		return domain;
	}
	public long getInvocationCount(){
		return invocationCount.get();
	}
	public void start(){
		current.set(System.currentTimeMillis());
	}
	public long getAverageTime(){
		if(invocationCount.get() >0 )return time.get() / invocationCount.get();
		return 0;
	}
	public void finish(){
		long startTime = current.get();
		if(startTime<=0)return;
		long endTime = System.currentTimeMillis();
		long useTime = endTime - startTime;
		
		if(useTime>upTime || useTime<downTime)return;
		
		int n=initCount.incrementAndGet();
		if(n > initCountThreshold){
			invocationCount.incrementAndGet();
			time.addAndGet((int)useTime);
			if(useTime < minTime)minTime = useTime;
			if(useTime > maxTime)maxTime = useTime;
			
			if(useTime >= TimeLogThreshold){
				long seconds = System.currentTimeMillis()/1000;
				try{
					XceLogger logger = ProfileLogger.getXceLogger();
					if(logger!=null){
						logger.addLog(logCategories,seconds+" "+url+useTime);
					}
				}catch(Exception ex){
					ex.printStackTrace();
				}
			}
		}
	}
	public String toString(){
		StringBuffer sb = new StringBuffer();
		sb.append(domain);sb.append(" ");sb.append(name);sb.append(" ");sb.append(getAverageTime());sb.append(" ");sb.append(invocationCount);
		return sb.toString();
	}
}