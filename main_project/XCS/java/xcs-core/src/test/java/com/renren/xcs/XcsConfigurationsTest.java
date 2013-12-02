/**
 * @(#)XcsConfigurationsTest.java, 2012-11-22. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs;

import junit.framework.Assert;

import org.junit.Test;


/**
 * @author Xun Dai <xun.dai@renren-inc.com>
 *
 */
public class XcsConfigurationsTest {
    
    @Test
    public void testGetConfiguration(){
        try {
            XcsConfigurations.getConfiguration();
        } catch (IllegalStateException e) {
            Assert.assertTrue(true);
        } catch (Exception e) {
            Assert.assertTrue(false);
        }
        
        System.setProperty("xoa2.hosts.twitter.demo.xoa.renren.com", "127.0.0.1");
        try {
            XcsConfigurations.getConfiguration();
        } catch (Exception e) {
            Assert.assertTrue(false);
        }
    }
}
