package com.renren.sns.wiki.utils.concurrent;

import org.apache.log4j.xml.DOMConfigurator;

/**
 * @author yi.li@renren-inc.com since 2011-12-5
 *
 */
public class TestAsynExecutor {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		DOMConfigurator.configure("D:\\temp\\log4j.xml");
		AsynTaskExecutor executor = AsynTaskExecutorUtil.getAsynExecutor();
		executor.addTask("testtask", "01", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello");
				try{
					Thread.sleep(1500);
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		});
		executor.addTask("testtask", "02", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello2");
				throw new RuntimeException("test exception");
			}
		});
		executor.addTask("testtask", "04", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello3");
				try{
					Thread.sleep(1500);
				}catch(Exception e){
					System.out.println("+++" + e.toString());
				}
				
				try{
					System.out.println("in this place");
					Thread.sleep(1500);
				}catch(Exception e){
				}
				System.out.println("Hello3_1");
			}
		}, 0L);
		executor.addTask("testtask", "04", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello4");
				try{
					Thread.sleep(5000);
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		}, 0L);
		executor.addTask("testtask", "04", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello5");
				try{
					Thread.sleep(5000);
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		}, 0L);
		executor.addTask("testtask", "04", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello4_f");
				try{
					Thread.sleep(5000);
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		}, 0L);
		executor.addTask("testtask", "04", new Runnable() {
			
			@Override
			public void run() {
				System.out.println("Hello4_f2");
				try{
					Thread.sleep(5000);
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		}, 0L);
	}

}
