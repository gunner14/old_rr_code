package com.renren.xoa;

import org.junit.Assert;
import org.junit.Test;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-10 下午02:54:57
 */
public class MethodTest {

	
	@Test
	public void testPutUrl1() {
		String originPath = "/somepath";
		String url = "xoa://test.xoa.renren.com" + originPath;
		Method method = Method.put(url);
		Assert.assertEquals(originPath + "?_method=put", method.getPath());
	}
	
	@Test
	public void testPutUrl2() {
		String originPath = "/somepath?hehe=haha";
		String url = "xoa://test.xoa.renren.com" + originPath;
		Method method = Method.put(url);
		Assert.assertEquals(originPath + "&_method=put", method.getPath());
	}
	
}
