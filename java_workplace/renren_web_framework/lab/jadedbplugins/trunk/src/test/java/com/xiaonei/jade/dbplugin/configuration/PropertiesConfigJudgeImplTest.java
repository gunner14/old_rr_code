package com.xiaonei.jade.dbplugin.configuration;

import java.util.Properties;


import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import com.xiaonei.jade.dbplugin.model.DataModel;

/**
 * 
 * @see PropertiesConfigJudgeImpl PropertiesConfigJudgeImplTest <br>
 * 
 * @author tai.wang@opi-corp.com Jul 14, 2010 - 4:40:47 PM
 */
public class PropertiesConfigJudgeImplTest {

    PropertiesConfigJudgeImpl test;

    @Before
    public void setUp() throws Exception {
        test = new PropertiesConfigJudgeImpl();
    }

    @After
    public void tearDown() throws Exception {
    }

    @Test
    public final void testIsAllowed() {
        Properties monitorProperties = new Properties();
        monitorProperties.setProperty("com.Test.*", "cost(15ms-20ms),time(00:00-12:59,  13:00-20:59, 23:00-23:59)");
        test.setMonitorProperties(monitorProperties);
        try {
            test.afterPropertiesSet();
        } catch (Exception e) {
            e.printStackTrace();
        }
        DataModel data = new DataModel();
        data.setClassName("com.Test");
        data.setMethodName("hi");
        data.setCostTime(15);
        Assert.assertTrue(test.isAllowed(data));
        data.setMethodName("hello");
        Assert.assertTrue(test.isAllowed(data));
        data.setCostTime(100);
        Assert.assertFalse(test.isAllowed(data));
        data.setClassName("com.Test2");
        data.setCostTime(10);
        Assert.assertFalse(test.isAllowed(data));
    }

    @Test
    public final void testIsAllowed1() {
        Properties monitorProperties = new Properties();
        monitorProperties.setProperty("com.Test.*", "cost(10ms),time(00:00-23:59)");
        test.setMonitorProperties(monitorProperties);
        try {
            test.afterPropertiesSet();
        } catch (Exception e) {
            e.printStackTrace();
        }
        DataModel data = new DataModel();
        data.setClassName("com.Test");
        data.setMethodName("hi");
        data.setCostTime(30);
        Assert.assertTrue(test.isAllowed(data));
        data.setClassName("com.Test");
        data.setMethodName("hi");
        data.setCostTime(5);
        Assert.assertFalse(test.isAllowed(data));
    }

    @Test
    public final void testIsAllowed2() {
        Properties monitorProperties = new Properties();
        monitorProperties.setProperty("com.*", "cost(10),time(00:00-23:59)");
        test.setMonitorProperties(monitorProperties);
        try {
            test.afterPropertiesSet();
        } catch (Exception e) {
            e.printStackTrace();
        }
        DataModel data = new DataModel();
        data.setClassName("com.Test");
        data.setMethodName("hi");
        data.setCostTime(30);
        Assert.assertTrue(test.isAllowed(data));
        data.setClassName("com.Test2");
        data.setMethodName("hi");
        data.setCostTime(30);
        Assert.assertTrue(test.isAllowed(data));
    }

    @Test
    public final void testIsAllowed3() {
        Properties monitorProperties = new Properties();
        monitorProperties.setProperty("*", "cost(10),time(00:00-23:59)");
        test.setMonitorProperties(monitorProperties);
        try {
            test.afterPropertiesSet();
        } catch (Exception e) {
            e.printStackTrace();
        }
        DataModel data = new DataModel();
        data.setClassName("org.Test");
        data.setMethodName("hi");
        data.setCostTime(30);
        Assert.assertTrue(test.isAllowed(data));
        data.setClassName("com.Test2");
        data.setMethodName("hi");
        data.setCostTime(30);
        Assert.assertTrue(test.isAllowed(data));
    }
    
    @Test
    @Ignore
    public final void testIsAllowed4() {
    	Properties monitorProperties = new Properties();
    	monitorProperties.setProperty("*", "cost(20ms),time(10:00-12:00, 17:00-18:00, 21:00-23:00)");
    	test.setMonitorProperties(monitorProperties);
    	try {
    		test.afterPropertiesSet();
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	DataModel data = new DataModel();
    	data.setClassName("org.Test");
    	data.setMethodName("hi");
    	data.setCostTime(30);
    	Assert.assertTrue(test.isAllowed(data));
    }

}
