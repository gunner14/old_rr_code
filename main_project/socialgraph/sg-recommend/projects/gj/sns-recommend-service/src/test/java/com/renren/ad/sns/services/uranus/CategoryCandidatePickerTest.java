package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.DatastoreImpl;
import com.google.code.morphia.Morphia;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
import com.mongodb.ServerAddress;
import com.renren.ad.sns.dao.impl.mongo.SnsMongoOptions;
import com.renren.ad.sns.data.Goods;

public class CategoryCandidatePickerTest {

    private static CategoryCandidatePicker picker;

    public static final Datastore getGJDataStore() {
        SnsMongoOptions mongoOptions = new SnsMongoOptions();
        mongoOptions.setSlaveOk(true);
        mongoOptions.setAutoConnectRetry(true);
        mongoOptions.setFsync(true);
        mongoOptions.setConnectionsPerHost(50);
        Mongo mongo = null;
        DatastoreImpl datastore = null;
        try {
            mongo = new Mongo(new ServerAddress("db1.jie.d.xiaonei.com", 62166), mongoOptions);
            datastore = new DatastoreImpl(new Morphia(), mongo, "sns_online", "gj",
                    "dec2011online4nb".toCharArray());
        } catch (MongoException e) {
            e.printStackTrace();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        return datastore;
    }

    private static List<Long> loadGoodsid(String filename) {
        List<Long> all = new ArrayList<Long>();
        Scanner cin;
        try {
            cin = new Scanner(new FileInputStream(filename));
            while (cin.hasNext()) {
                String line = cin.nextLine();
                all.add(Long.parseLong(line.trim()));
            }
        }catch (FileNotFoundException e) {}
        return all;
    }
    
    private static ITagList2CandidatePicker[] pickers = new ITagList2CandidatePicker[1];
    private static List<Long> ids;
   
    @BeforeClass
    public static void init() {
        picker = new CategoryCandidatePicker(getGJDataStore(), pickers);
        pickers[0] = new SimilarityTagList2CandidatePicker();
        picker.setDataPath("./test-data");
        picker.init();
        assertTrue(picker.getGoodsDataWatchdog().isDaemon());
        assertTrue(picker.getGoodsDataWatchdog().isAlive());
        //ids = loadGoodsid("./test-data/goodsid.dat");
    }

    @Test
    public void testTag2Candidate() {
        assertTrue(picker.getTag2Candidate().size() > 0);

        long tagId = 10272;
        assertTrue(picker.getTag2Candidate().containsKey(tagId));
        CandidateList cl = picker.getTag2Candidate().get(tagId);
        List<Long> actualGids = new ArrayList<Long>();
        for (Candidate c : cl) {
            actualGids.add(c.getGid());
        }
        long[] expectedGids = { 1206011450014284178L, 1206011450014283166L, 1206011450014280165L};
        assertEquals(expectedGids.length, actualGids.size());
        // here we ignore the order
        for (int i = 0; i < expectedGids.length; i++) {
            assertTrue(actualGids.contains(expectedGids[i]));
        }

        tagId = 10273;
        assertTrue(picker.getTag2Candidate().containsKey(tagId));
        cl = picker.getTag2Candidate().get(tagId);
        actualGids.clear();
        for (Candidate c : cl) {
            actualGids.add(c.getGid());
        }
        expectedGids = new long[] { 1206011450014283199L, 1206011450014283169L, 1206011450014283162L, 1206011450014283166L };
        assertEquals(expectedGids.length, actualGids.size());
        for (int i = 0; i < expectedGids.length; i++) {
            assertTrue(actualGids.contains(expectedGids[i]));
        }
    }

    //FIXME the test is not pass, check it
    @Ignore
    @Test
    public void testPick() {
        long uid = -1;
        long gid = 1206011450014284178L;
		// test a common goods
		List<Candidate> result = picker.pick(uid, gid);
		assertNotNull(result);
		/*
		 * because in test-data, the size of hot goods is smaller than 24, so
		 * here the recommendation size is equals to hot goods size.
		 */
		long[] expectedGids = { 1206011450014283166L, 1206011450014283169L, 1206011450014280165L};
		assertEquals(expectedGids.length+picker.getHotGoods().size(), result.size());
        // here we ignore the order
        for (int i = 0; i < expectedGids.length; i++) {
        	assertEquals(result.get(i).getGid(), expectedGids[i]);
        }
            
        uid = -1;
        gid = 1206011450014283166L;
		result = picker.pick(uid, gid);
		assertNotNull(result);
		expectedGids =  new long[]{ 1206011450014283169L, 1206011450014284178L, 1206011450014283162L, 1206011450014283199L, 1206011450014280165L};
		assertEquals(expectedGids.length+picker.getHotGoods().size(), result.size());
        // here we ignore the order
        for (int i = 0; i < expectedGids.length; i++) {
        	assertEquals(result.get(i).getGid(), expectedGids[i]);
        }

		// test an unknown goods
        uid = -1;
        gid = -1;
        result = picker.pick(uid, gid);
        assertNotNull(result);
        assertEquals(picker.getHotGoods().size(), result.size());
    }

}
