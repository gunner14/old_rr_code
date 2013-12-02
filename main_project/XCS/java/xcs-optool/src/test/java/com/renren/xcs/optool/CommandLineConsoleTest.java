/**
 * @(#)CommandLineConsoleTest.java, 2012-9-24. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs.optool;

import java.io.File;

import org.junit.Test;

import junit.framework.TestCase;

/**
 * 默认情况下，需要以下配置文件：
 * 1. pwd.txt
 * 
 * @author yuan.liu1
 */
public class CommandLineConsoleTest extends TestCase {
    String zkStr = "10.3.18.186:2181";
    
    @Test
    public void testProcess() {
        String [] args = new String[6];
        args[0] = new String("-server");
        args[1] = zkStr;
        args[2] = new String("xcsCreate");
        args[3] = new String("test.xcs.renren.com");
        args[4] = new String("1");
        args[5] = new String("10");
        
        try {
            CommandLineConsole console = new CommandLineConsole(args);
            console.process();
        } catch (Exception e) {
            fail();
            e.printStackTrace();
        }
        
        String [] argsD = new String[4];
        argsD[0] = new String("-server");
        argsD[1] = zkStr;
        argsD[2] = new String("xcsDelete");
        argsD[3] = new String("test.xcs.renren.com");

        try {
            CommandLineConsole console = new CommandLineConsole(argsD);
            console.process();
        } catch (Exception e) {
            fail();
            e.printStackTrace();
        }
    }
    
    @Test
    public void  testXoaXml() {
        System.setProperty("xoa.config", "conf\\xoa.xml".replace("\\", File.separator));
        System.setProperty("xcs.root", "sandbox");
        String [] args = new String[3];
        args[0] = new String("-server");
        args[1] = zkStr;
        args[2] = new String("xcsCreate");
        
        try {
            CommandLineConsole console = new CommandLineConsole(args);
            console.process();
        } catch (Exception e) {
            fail();
            e.printStackTrace();
        }
        
        args[2] = new String("xcsDelete");
        try {
            CommandLineConsole console = new CommandLineConsole(args);
            console.process();
        } catch (Exception e) {
            fail();
            e.printStackTrace();
        }
    }
}
