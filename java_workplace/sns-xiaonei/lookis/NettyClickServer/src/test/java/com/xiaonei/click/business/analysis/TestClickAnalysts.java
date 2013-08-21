/**
 * 
 */
package com.xiaonei.click.business.analysis;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.xiaonei.click.BaseTestClass;
import com.xiaonei.click.business.persistence.MemoryPersistence;
import com.xiaonei.click.business.persistence.PersistenceManager;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class TestClickAnalysts extends BaseTestClass {

    @Before
    public void setUp() throws Exception {
        MemoryPersistence.init();
    }

    @Test
    public void clickNull() throws Exception {
        AnalysisFactory.getInstance().buildAnalysts().analysis(null);
    }

    @Test
    public void clickAddErrJ() throws Exception {
        Map<String, String> req = new HashMap<String, String>();
        String errFormatJson = "{fdsafdsa";
        req.put("J", errFormatJson);
        long sizeOld = ((MemoryPersistence) PersistenceManager.getInstance()).size();
        try {
            AnalysisFactory.getInstance().buildAnalysts().analysis(req);
            Assert.fail("error format String should throw a Exception");
        } catch (Exception e) {

        }
        Assert.assertEquals(sizeOld, ((MemoryPersistence) PersistenceManager.getInstance()).size());

    }

    public void clickAddRightJ() throws Exception {
        Map<String, String> req = new HashMap<String, String>();
        JSONObject jo = new JSONObject();
        jo.put("test", true);
        req.put("J", jo.toString());

        long sizeOld = ((MemoryPersistence) PersistenceManager.getInstance()).size();
        AnalysisFactory.getInstance().buildAnalysts().analysis(req);
        Assert.assertEquals(sizeOld + 1, ((MemoryPersistence) PersistenceManager.getInstance())
                .size());
    }

    @Test
    public void clickAddNoJ() throws Exception {
        Map<String, String> req = new HashMap<String, String>();
        JSONObject jo = new JSONObject();
        jo.put("test", true);
        req.put("E", jo.toString());
        long sizeOld = ((MemoryPersistence) PersistenceManager.getInstance()).size();
        AnalysisFactory.getInstance().buildAnalysts().analysis(req);
        Assert.assertEquals(sizeOld, ((MemoryPersistence) PersistenceManager.getInstance()).size());
    }

    @Test
    public void clickAddErrNoJ() throws Exception {
        Map<String, String> req = new HashMap<String, String>();
        String errFormatJson = "{fdsafdsa";
        req.put("E", errFormatJson);

        long sizeOld = ((MemoryPersistence) PersistenceManager.getInstance()).size();
        AnalysisFactory.getInstance().buildAnalysts().analysis(req);
        Assert.assertEquals(sizeOld, ((MemoryPersistence) PersistenceManager.getInstance()).size());
    }

    @After
    public void tearDown() throws Exception {
        ((MemoryPersistence) MemoryPersistence.getInstance()).restore();
    }

}
