package com.renren.xoa.util;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import junit.framework.Assert;

import org.junit.Test;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-1-20 上午12:01:52
 */
public class URIUtilTest {

	@Test
	public void simpleTest() {
		
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("userId", 68464);
		map.put("photoId", 1234);
		map.put("value", "haha");
		Set<String> hitParams = new HashSet<String>();
		String uri = "/{userId}/haha/{photoId}";
		
		String ret = URIUtil.replaceParams(uri, map, hitParams);
		Assert.assertEquals("/68464/haha/1234", ret);
		
		Assert.assertTrue(hitParams.size() == 2);
		Assert.assertTrue(hitParams.contains("userId"));
		Assert.assertTrue(hitParams.contains("photoId"));
	}
	
}
