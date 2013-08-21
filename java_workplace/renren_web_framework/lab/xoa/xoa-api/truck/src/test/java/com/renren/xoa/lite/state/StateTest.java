package com.renren.xoa.lite.state;

import static org.junit.Assert.*;

import org.junit.Test;

import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.simple.SimpleStringService;
import com.renren.xoa.mock.MockXoaClient;
import com.renren.xoa.util.TestUtil;

public class StateTest {

	@Test
	public void testNoSubmit() {
		ServiceFactory fact = TestUtil.getServiceFactory();
		SimpleStringService svc = fact.getService(SimpleStringService.class, new MockXoaClient());
		boolean fail = false;
		try {
			svc.getOk().awaitUninterruptibly().getContent();
		} catch (IllegalStateException e) {
			fail = true;
		}
		assertTrue(fail);
	}
	
	@Test
	public void testModifyMethodOnSubmitted() {
		ServiceFactory fact = TestUtil.getServiceFactory();
		ServiceFuture<String> f = fact.getService(SimpleStringService.class, new MockXoaClient()).getOk().submit();
		
		boolean fail = false;
		try {
			f.setHeader("fdsf", "fdsfd");
		} catch (IllegalStateException e) {
			fail = true;
		}
		assertTrue(fail);
		
		fail = false;
		try {
			f.setParam("ppp", "ppp");
		} catch (IllegalStateException e) {
			fail = true;
		}
		assertTrue(fail);
	}
	
}
