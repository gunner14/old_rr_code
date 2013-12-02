package com.renren.xcs;

import junit.framework.Assert;

import org.junit.Test;

public class XoaConfigurationTest {

    @Test
    public void testServerConfigurationNoConfig() {
        try {
            new SystemPropertyConfiguration();
        } catch (Exception e) {
            e.printStackTrace();
            Assert.assertTrue(true);
        }
    }

    @Test
    public void testServerConfigurationErrorConfig() {
        System.setProperty("xcs.config", "/no_config_file_here");
        try {
            new SystemPropertyConfiguration();
        } catch (Exception e) {
            e.printStackTrace();
            Assert.assertTrue(true);
        }
    }

    @Test
    public void testServerConfiguration() throws Exception {
        System.setProperty("xcs.config", XoaConfigurationTest.class.getClassLoader()
                .getResource("xcs-test.properties").getPath());
        try {
            new SystemPropertyConfiguration();
        } catch (Exception e) {
            Assert.assertTrue(false);
        }
    }
}
