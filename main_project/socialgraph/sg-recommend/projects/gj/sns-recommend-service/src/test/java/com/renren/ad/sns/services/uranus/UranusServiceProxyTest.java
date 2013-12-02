package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.*;
import static org.junit.Assert.assertNotNull;

import org.apache.thrift.TException;
import org.json.JSONArray;
import org.json.JSONException;
import org.junit.Test;

public class UranusServiceProxyTest {

    @Test
    public void testRecommend() throws TException {
                UranusServiceProxy proxy = new UranusServiceProxy("10.7.18.50", 9636, 5000);
//        UranusServiceProxy proxy = new UranusServiceProxy("10.3.24.96", 9635, 10000);
        assertNotNull(proxy);
        long[] gids = { 1203141131369748042L };
        long uid = 0;
        for (long gid : gids) {
            String result = proxy.recommend(uid, gid, 24);
            System.out.println(result);
            try {
                JSONArray json = new JSONArray(result);
                assertTrue(json.length() == 24);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
        proxy.cleanUp();
    }
}
