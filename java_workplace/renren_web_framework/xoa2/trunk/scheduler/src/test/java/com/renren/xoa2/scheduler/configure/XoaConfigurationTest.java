package com.renren.xoa2.scheduler.configure;

import java.io.File;

import junit.framework.Assert;

import org.junit.BeforeClass;
import org.junit.Test;

public class XoaConfigurationTest {
    private static String confFile = ".\\src\\test\\resources\\jobconf_test.xml";
    private static String localConf; 
    
    @BeforeClass
    public static void setUpBeforeClass() throws Exception {
        localConf = confFile.replace("\\", File.separator);
    }

    @Test
    public void testGetJobConfigure() {
        XoaJobConfigure jobConf = XoaConfiguration.getJobConfigure(localConf);
        Assert.assertEquals(3, jobConf.getTaskNumber());
        Assert.assertEquals(100, jobConf.getServiceTime());
        Assert.assertEquals(200, jobConf.getServiceIdleTime());
        Assert.assertEquals("resources/test.conf", jobConf.getServiceConf());
        
        jobConf = XoaConfiguration.getJobConfigure("XXX-NOTEXIST");
        Assert.assertNull(jobConf);
    }
}
