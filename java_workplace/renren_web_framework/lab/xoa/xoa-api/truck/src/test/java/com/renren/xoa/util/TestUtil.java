package com.renren.xoa.util;

import org.junit.Ignore;
import org.junit.Test;

import com.renren.xoa.lite.DefaultServiceFactory;
import com.renren.xoa.lite.ServiceFactory;

@Ignore
public class TestUtil {

	private static ServiceFactory serviceFactroy = new DefaultServiceFactory();
	
	public static ServiceFactory getServiceFactory() {
		return serviceFactroy;
	}
	
}
