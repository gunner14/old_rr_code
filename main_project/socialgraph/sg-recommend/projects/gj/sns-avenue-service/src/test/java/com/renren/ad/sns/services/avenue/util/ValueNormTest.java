package com.renren.ad.sns.services.avenue.util;

import static org.junit.Assert.*;

import org.junit.Test;

public class ValueNormTest {

    @Test
    public void testGetNormValue() {

        for (int i = 1; i < 100; i++) {
            assertTrue(Math.abs(0.9 - ValueNorm.getNormValue(i, i)) < 0.1);
        }

        
        System.out.println(ValueNorm.getNormValue(0.9, 1));
    }
}
