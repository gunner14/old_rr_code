package com.renren.xcs;

import java.net.URISyntaxException;

import org.junit.Assert;
import org.junit.Test;

public class ConfigurationTest {

    @Test
    public void testPropertiesFileConfiguration() {
        try {
            Configuration conf = new PropertiesFileConfiguration(ConfigurationTest.class
                    .getClassLoader().getResource("xcs-test.properties").toURI().getPath());
            Assert.assertEquals(conf.getRoot(), "online");
            Assert.assertEquals(conf.getServerUsername(), "server.test");
            Assert.assertEquals(conf.getServerPassword(), "server.test");
        } catch (InvalidConfigurationException e) {
            e.printStackTrace();
            Assert.assertTrue(false);
        } catch (URISyntaxException e) {
            e.printStackTrace();
            Assert.assertTrue(false);
        }
    }
}
