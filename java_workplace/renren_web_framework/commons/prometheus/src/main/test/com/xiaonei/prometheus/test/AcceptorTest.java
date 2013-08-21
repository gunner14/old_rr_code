package com.xiaonei.prometheus.test;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;

import org.junit.Before;
import org.junit.Test;


public class AcceptorTest {
	private CountDownLatch latch;
	@Before
	public void init(){
		latch = new CountDownLatch(1);
	}
	@Test
	public void test() throws InterruptedException, IOException{
	
	}
}
