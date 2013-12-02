package com.renren.ad.sns.services.uranus.util;

import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.DatastoreImpl;
import com.google.code.morphia.Morphia;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
import com.mongodb.ServerAddress;
import com.renren.ad.sns.dao.impl.mongo.SnsMongoOptions;
import com.renren.ad.sns.data.Goods;

public class LinkStat {

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
    
    public void onlyTest() {
        Datastore ds = getGJDataStore();

        final int BATCH_SIZE = 50000;
        long recordSize = ds.getCount(Goods.class);
        Map<String, Integer> urls = new HashMap<String, Integer>();
        Map<String, String> url2url = new HashMap<String, String>();
        for (int offset = 0; offset < recordSize; offset += BATCH_SIZE) {
            List<Goods> result = ds.find(Goods.class).retrievedFields(true, "link").order("link")
                    .skip(offset).limit(BATCH_SIZE).asList();
            for (Goods goods : result) {
                String link = goods.getLink();
                if (link != null) {
                    if (link.length() >= 20) {
                        String url = link.substring(0, 20);
                        if (urls.containsKey(url)) {
                            urls.put(url, urls.get(url) + 1);
                        } else {
                            System.out.println(url);
                            urls.put(url, 1);
                            url2url.put(url, link);
                        }
                    } else {
                        System.err.println("short link:" + link);
                    }
                } else {
                    System.err.println("link null");
                }
            }
            
            try {
                System.out.println(">> handled:["+offset+","+(offset+BATCH_SIZE)+"]");
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            
        }

        List<Map.Entry<String, Integer>> urlEntry = new ArrayList<Map.Entry<String, Integer>>(urls
                .entrySet());
        Collections.sort(urlEntry, new Comparator<Map.Entry<String, Integer>>() {

            public int compare(Map.Entry<String, Integer> o1, Map.Entry<String, Integer> o2) {
                return (o2.getValue() - o1.getValue());
            }
        });
        for (Map.Entry<String, Integer> entry : urlEntry) {
            System.out.println(entry.getKey() + "," + entry.getValue()+","+url2url.get(entry.getKey()));
        }
    }
}
