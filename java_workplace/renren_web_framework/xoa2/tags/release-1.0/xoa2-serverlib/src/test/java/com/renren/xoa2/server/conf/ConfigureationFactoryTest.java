/**
 * 
 */

package com.renren.xoa2.server.conf;

import java.io.File;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.server.conf.Configuration.ConfigurationFactory;
import com.renren.xoa2.server.impl.AbstractServiceBuilder;
import com.renren.xoa2.server.impl.XoaServiceConfig;

/**
 * @author jinchao.wen
 * @email jinchao.wen@opi-corp.com
 * @date 2011-12-8
 */
public class ConfigureationFactoryTest {
	
    private static Logger logger = LoggerFactory.getLogger(ConfigureationFactoryTest.class);
    
    private String confFile = ".\\src\\test\\resource\\xoa.xml";
    
    private String confFileMulti = ".\\src\\test\\resource\\xoa-multi.xml";
    
    /**
     * Test method for
     * {@link com.renren.xoa2.server.conf.Configuration.ConfigurationFactory#newConfiguration(java.lang.String)}
     * .
     */
    @Test
    public void testNewConfiguration() {
    	String localConfFile = confFile.replace("\\", File.separator);
    	
        Configuration conf = ConfigurationFactory.newConfiguration(localConfFile);
       
        List<XoaServiceConfig> configList = conf.listServiceConfig();
        Assert.assertTrue( configList.size()==1 );
        Assert.assertTrue( configList.get(0).getServiceId().equals("twitter.xoa.renren.com"));
        Assert.assertTrue( configList.get(0).getServiceClass().equals("com.renren.xoa2.demo.twitter.TwitterServiceImpl"));
        Assert.assertTrue( configList.get(0).getPort()==9100);
        Assert.assertTrue( configList.get(0).getInitListenerClass().equals("com.renren.xoa2.server.DummyInitListenerImpl"));

        Assert.assertTrue( configList.get(0).getThreadPoolCoreSize() == 20);
        Assert.assertTrue( configList.get(0).getThreadPoolMaxSize() == 60);
        
    }
    
    @Test
    public void testConfigurationMulti() {
    	String localConfFile = confFileMulti.replace("\\", File.separator);
    	
        Configuration conf = ConfigurationFactory.newConfiguration(localConfFile);
       
        List<XoaServiceConfig> configList = conf.listServiceConfig();
        Assert.assertTrue(configList.size() == 2);

        Assert.assertTrue( configList.get(1).getServiceId().equals("twitteradmin.xoa.renren.com"));
        Assert.assertTrue( configList.get(1).getServiceClass().equals("com.renren.xoa2.demo.twitter.TwitterAdminServiceImpl"));
        Assert.assertTrue( configList.get(1).getPort()==9101);
        Assert.assertTrue( configList.get(1).getInitListenerClass().equals("com.renren.xoa2.server.DummyInitListenerImpl"));
    }
    
    @Test
    public void testException() {
    	try {
    		ConfigurationFactory.newConfiguration("	");
    		Assert.fail();
    	} catch (Exception e) {
    	}
    	
        Configuration conf = ConfigurationFactory.newConfiguration("NotExist");
        Assert.assertNull(conf);
    }
}
