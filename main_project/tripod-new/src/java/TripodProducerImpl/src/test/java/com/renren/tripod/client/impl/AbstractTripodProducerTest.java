package com.renren.tripod.client.impl;

import java.util.Map;

import org.junit.Assert;
import org.junit.Test;


public class AbstractTripodProducerTest {

    @Test
    public void testCreate() {
        
        MockTripodProducer producer = new MockTripodProducer();
        String[] keys = {"Li Lei", "Han Meimei", "Lucy", "Lily"};
        Map<String, byte[]> ret = producer.create(keys, true);
        
        for(String key : keys) {
            Assert.assertTrue(ret.get(key).length > 0);
        }
    }
}
