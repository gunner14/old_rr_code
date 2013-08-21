/**
 * 
 */
package com.xiaonei.click.business.work;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Future;

import junit.framework.Assert;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import com.xiaonei.click.BaseTestClass;
import com.xiaonei.click.business.analysis.RecordAnalysisFactory;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class TestPersistenceJob extends BaseTestClass {

    /**
     * @throws java.lang.Exception
     */
    @Before
    public void setUp() throws Exception {
        RecordAnalysisFactory.getInstance().init();
    }

    @Test
    public void testRunWithNullInitPersistenceJob() throws Exception {
        //null init
        Job<Void> j = new PersistenceJob();
        try {
            j.init(null);
            Assert.fail("new PersistenceJob with null param should throw an Exception");
        } catch (Exception e) {

        }
    }

    @Test
    public void testRightInitAfterErrInitRun() throws Exception {
        Job<Void> j = new PersistenceJob();
        try {
            j.init(null);
        } catch (Exception e1) {}
        //right init after error init
        Map<String, String> param = new HashMap<String, String>();
        int c = r.nextInt(10);
        for (int i = 0; i < c; i++) {
            param.put(getRandomString(), getRandomString());
        }
        j.init(param);
        j.call();
    }

    @Test
    public void testDoubleInit() throws Exception {
        Map<String, String> param = new HashMap<String, String>();
        int c = r.nextInt(10);
        for (int i = 0; i < c; i++) {
            param.put(getRandomString(), getRandomString());
        }
        //double init
        Job<Void> j = new PersistenceJob();
        j.init(param);
        try {
            j.init(param);
            Assert.fail("new PersistenceJob with double param should throw an Exception");
        } catch (Exception e) {

        }

    }

    @Ignore
    @Test
    public void testRunBeforeInit() throws Exception {

        //call before init
        Job<Void> j = new PersistenceJob();
        try {
            j.call();
            Assert.fail("new PersistenceJob without param should throw an Exception");
        } catch (Exception e) {}
    }

    @Ignore
    @Test
    public void testPersistenceJob() throws Exception {
        Map<String, String> param = new HashMap<String, String>();
        int c = r.nextInt(10);
        for (int i = 0; i < c; i++) {
            param.put(getRandomString(), getRandomString());
        }
        Job<Void> j = new PersistenceJob();
        j.init(param);
        Future<?> f = Boss.getInstance().submit(j);
        //Assert.assertEquals(true, f.get());
        Object o = RecordAnalysisFactory.getInstance().buildAnalysts().getRecord();
        Assert.assertEquals(param, o);
    }

    @After
    public void tearDown() throws Exception {
        RecordAnalysisFactory.getInstance().restore();
    }

}
