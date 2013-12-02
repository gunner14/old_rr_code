package com.renren.xcs;

import java.io.IOException;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class FindFactorysTest {

    Configuration conf;

    @Before
    public void init() {
        conf = new TestConfiguration();
    }

    @Test
    public void testEqual() throws IOException {

        Assert.assertEquals(FindFactorys.getConsumer(conf), FindFactorys.getConsumer(conf));
    }

    @Test
    public void testConcurrentEqual() throws IOException {
        Assert.assertEquals(FindFactorys.getConsumer(conf), FindFactorys.getConsumer(conf));
        Assert.assertTrue(FindFactorys.getConsumer(conf) == FindFactorys.getConsumer(conf));
    }
}
