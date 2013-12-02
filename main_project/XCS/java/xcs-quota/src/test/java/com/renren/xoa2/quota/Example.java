package com.renren.xoa2.quota;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 *
 * @author Xue Wenxin Aug 15, 2012 9:15:31 PM
 */
public class Example {
	private String number;
	private int counter;
	private int temp=0;
    private  SimpleDateFormat format=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	
	private ScheduledThreadPoolExecutor timerExecutor = new ScheduledThreadPoolExecutor(2);

	public Example(String number, int counter){
		this.number = number;
		this.counter = counter;
		runTimer();
	}
	public Example(){
		
	}
	private void runTimer(){
		timerExecutor.scheduleAtFixedRate(new Runnable() {
			
			public void run() {
				try {
					setTemp(getTemp() + 1);
	                System.out.println("in example "+number+counter+":"+format.format(new Date()));
				} catch (Throwable e) {
				}
			}
		}, 1, 5, TimeUnit.SECONDS);
		//TODO：问刘源，是否有对总数配额进行限制到需求，如果有的话，拟采用如下方案
		//方案：需要配置一个client端机器数，然后定时器每十分钟执行一次，用当前single数量*机器数量然后判断是否超过总配额；
		
	}

	public int getTemp() {
		return temp;
	}

	public void setTemp(int temp) {
		this.temp = temp;
	}
	
	public static void main(String[] args) {
		List<Example> list = new ArrayList<Example>();
		for(int i=0;i<5;i++){
			Example ex1 = new Example();
			ex1.setTemp(i);
			list.add(ex1);
		}
		for(int j=0;j<5;j++){
			Example ex2 = list.get(j);
			ex2.setTemp(ex2.getTemp()+1);
		}
		for(int x=0;x<5;x++){
			System.out.println(list.get(x).getTemp());
		}
	}
}
