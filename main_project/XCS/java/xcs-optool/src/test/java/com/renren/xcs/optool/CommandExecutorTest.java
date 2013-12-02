/**
 * @(#)CommandExecutorTest.java, 2012-11-22. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs.optool;

import org.junit.Test;

/**
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public class CommandExecutorTest {
    @Test
    public void testCreate(){
        CommandExecutor.main(new String[]{"create", "twitter.demo.xoa.renren.com", "1", "0"});
    }
    @Test
    public void testDelete(){
        CommandExecutor.main(new String[]{"delete", "twitter.demo.xoa.renren.com"});
    }
    
    @Test
    public void testEnable(){
        CommandExecutor.main(new String[]{"enable", "twitter.demo.xoa.renren.com", "1", "0", "10.2.8.66:9090"});
    }
    
    @Test
    public void testDisable(){
        CommandExecutor.main(new String[]{"disable", "twitter.demo.xoa.renren.com", "1", "0", "10.2.8.66:9090"});
    }
}
