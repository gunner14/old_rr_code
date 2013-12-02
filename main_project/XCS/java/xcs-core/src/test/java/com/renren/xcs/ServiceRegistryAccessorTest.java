package com.renren.xcs;

import java.util.Arrays;
import java.util.List;

import junit.framework.Assert;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Before;
import org.junit.Test;

public class ServiceRegistryAccessorTest {

    private static Log logger = LogFactory.getLog(ServiceRegistryAccessorTest.class.getName());

    String service = "test4.xoa.renren.com";

    String version = "1";

    ServiceRegistryAccessor accessor = null;

    @Before
    @Test
    public void testConstructor() {
        try {
            accessor = new FindServiceRegistryAccessor(service, version, new TestConfiguration());
        } catch (Exception e) {
            e.printStackTrace();
            Assert.assertFalse(true);
        }
        Assert.assertNotNull(accessor);
        Assert.assertEquals(version, accessor.getVersion());
        Assert.assertEquals(service, accessor.getService());
    }

    @Test
    public void testListNodesAndListen() {
        try {

            List<String> states = accessor.listNodesAndListenChange("0",
                    new ChildrenChangeListener() {

                        @Override
                        public void childrenChanged(List<String> children) {
                            logger.info("Children changed: " + Arrays.toString(children.toArray()));
                        }
                    });

            logger.info("Nodes: " + Arrays.toString(states.toArray()));
        } catch (Exception e) {
            e.printStackTrace();
            Assert.assertFalse(true);
        }
    }

    @Test
    public void testgetNodeAndListen() {
        try {

            byte[] data = accessor.getNodeAndListenChange("0", "127.0.0.1:8081",
                    new DataChangeListener() {

                        @Override
                        public void dataChanged(byte[] data) {
                            logger.info("Children changed: " + new String(data));
                        }
                    });

            logger.info("Data: " + new String(data));
        } catch (Exception e) {
            e.printStackTrace();
            Assert.assertFalse(true);
        }
    }
}
