package com.renren.xoa.lite.error;

import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;

import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;

public class BizErrorTest {
	
	private BizErrorService service;
	
	@Before
	public void init() {
		service = ServiceFactories.getFactory().getService(
				BizErrorService.class);
	}
	
	@Test
	public void test() throws Exception {
		
		/*long start = System.currentTimeMillis();
		ServiceFuture<String> f = service.get();
		LogErrorListener l = new LogErrorListener();
		f.addListener(l);
		f.submit();
		f.awaitUninterruptibly();
		assertFalse(f.isSuccess());
		assertEquals("XOA逻辑错误", l.getReturn());
		long end = System.currentTimeMillis();
		
		System.out.println(end - start);
		
		System.out.println(ServiceFutureHelper.getInvocationInfo(f));*/
		//System.out.println("error msg: " + l.getReturn());
	}
}
