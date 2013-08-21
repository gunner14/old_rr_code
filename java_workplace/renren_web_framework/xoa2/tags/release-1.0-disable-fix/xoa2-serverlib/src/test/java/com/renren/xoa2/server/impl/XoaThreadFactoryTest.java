package com.renren.xoa2.server.impl;

import junit.framework.Assert;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class XoaThreadFactoryTest {

	class ErrorRunnable implements Runnable {
		@Override
		public void run() {
			int a = 1/0;
			throw new RuntimeException("Error Here!" + a);
		}
	}
	
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

	@Before
	public void setUp() throws Exception {
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void testNewThread() {
		XoaThreadFactory threadFactory = new XoaThreadFactory();
		Thread thread = threadFactory.newThread(new ErrorRunnable());
		try {
			thread.start();
			thread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
			Assert.assertFalse(true);
		}
		
		
		thread = new Thread(new ErrorRunnable());
		try {
			thread.start();
			thread.join();
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
}
