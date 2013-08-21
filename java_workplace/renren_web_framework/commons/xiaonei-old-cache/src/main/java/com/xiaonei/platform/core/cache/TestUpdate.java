package com.xiaonei.platform.core.cache;

import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Level;
import org.apache.log4j.LogManager;

public class TestUpdate {
	private MemCacheManager manager = null;
	public TestUpdate(){
		manager = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
	}
	public void run(){
		LogManager.getRootLogger().setLevel(Level.OFF);
		BasicConfigurator.configure();
		manager.set("name", "zhangjie");
		GetThread get = new GetThread();
		get.start();
		
		UpdateThread update = new UpdateThread();
		update.start();
	}
	
	public static void main(String[] args){
		TestUpdate test = new TestUpdate();
		test.run();
	}
	private class GetThread extends Thread{
		@Override
		public void run() {
			while(true){
				manager.get("name");
			}
		}
	}
	private class UpdateThread extends Thread{
		public void run(){
			while(true){
				manager.update("wo ai");
				try {
					Thread.sleep(1000*5);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
