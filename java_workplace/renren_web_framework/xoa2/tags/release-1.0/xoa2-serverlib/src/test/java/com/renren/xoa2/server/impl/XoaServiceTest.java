package com.renren.xoa2.server.impl;

import junit.framework.Assert;

import org.junit.Test;

public class XoaServiceTest {
	XoaService xoaService = new XoaService();
	
	@Test
	public void testRunException() {
		// Need catch the exception
		Thread serviceThread = new Thread(xoaService, "TestXoaService");
		
		xoaService.setServer(null);
		
		try {
			serviceThread.start();
			Thread.currentThread().join(100);		
			Assert.assertNull(serviceThread.getUncaughtExceptionHandler());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
