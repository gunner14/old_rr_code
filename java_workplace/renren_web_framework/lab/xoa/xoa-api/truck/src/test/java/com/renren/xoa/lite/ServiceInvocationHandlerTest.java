package com.renren.xoa.lite;

import org.junit.Assert;
import org.junit.Test;

import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaService;

public class ServiceInvocationHandlerTest {

	@XoaService(serviceId = "test.xoa.renren.com")
	private static interface TestInterface {

		@XoaGet("no_slash_at_beginning")
		public ServiceFuture<Object> get();
		
		@XoaGet("")
		public ServiceFuture<Object> get2();
		
		public Object anotherMethod();
	}
	
	/**
	 * 测试检测URI必须以'/'开头的检测机制
	 */
	@Test
	public void testUriCheck() {
		
		ServiceFactory fact = ServiceFactories.getFactory();
		TestInterface svc = fact.getService(TestInterface.class);
		boolean fail = false;
		try {
			svc.get();
		} catch (IllegalArgumentException e) {
			fail = true;
		}
		Assert.assertTrue(fail);
		
		fail = false;
		try {
			svc.get2();
		} catch (IllegalArgumentException e) {
			fail = true;
		}
		Assert.assertFalse(fail);
	}
}
