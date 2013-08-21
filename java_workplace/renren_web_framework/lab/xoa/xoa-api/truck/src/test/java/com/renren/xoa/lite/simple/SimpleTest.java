package com.renren.xoa.lite.simple;

import org.junit.Test;
import static org.junit.Assert.*;


import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.mock.MockXoaClient;
import com.renren.xoa.util.TestUtil;

public class SimpleTest {
	@Test
	public void testOk() {
		
		/*ServiceFactory fact = TestUtil.getServiceFactory();
		SimpleStringService svc = fact.getService(SimpleStringService.class, new MockXoaClient());
		
		String content = svc.getOk().awaitUninterruptibly().getContent();
		assertEquals(content, "ok");*/
	}
	
}
