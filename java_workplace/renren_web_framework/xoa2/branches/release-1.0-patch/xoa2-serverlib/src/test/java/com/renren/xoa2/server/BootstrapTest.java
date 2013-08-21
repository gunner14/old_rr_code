package com.renren.xoa2.server;

import static org.junit.Assert.*;

import java.io.File;

import junit.framework.Assert;

import org.apache.thrift.TException;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.xoa2.server.ping.PingClient;

/**
 * 这里只是简单测试一下BootStrap逻辑的可用性
 * 
 * @author yuan.liu1@renren-inc.com
 */
public class BootstrapTest {
	private String confFile = ".\\src\\test\\resource\\xoa-ping.xml";
	
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

	@Test
	public void testRun() {
		Bootstrap bootstrap = new Bootstrap();
		String localConf = confFile.replace("\\", File.separator);
		
		bootstrap.startServices(localConf);
		
		PingClient client = new PingClient();
		client.init(9102);
		try {
			for (int i = 0; i < 10; ++i) {
				int number = client.invoke();
				Assert.assertEquals(number, (i+1));
			}
		} catch (TException e) {
			fail();
		}
		
		client.close();
		Assert.assertTrue(bootstrap.getInvalidServicNumber() == 0);
		
		bootstrap.haltAll();
		
		client.init(9102);
		try {
			client.invoke();
			fail();
		} catch (TException e) {
			client.close();
		}
		
		Assert.assertTrue(bootstrap.getInvalidServicNumber()> 0);
	}
}
