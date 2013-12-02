package com.renren.ad.sns.services.antispam.rules;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.query.Query;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.data.User;
import com.renren.ad.sns.services.antispam.GJDataStore;

public class RubbishGoodsSpamTmp {

    private static final Datastore ds = GJDataStore.getDefaultDataStore();

    private static final String SPAM_REASON = "rubbish_goods_spam";

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyMMddHHmmss");

    public static void detect() {
        String fileName = "rubbish-goods-spammer.dat";

        // 1. scan User, get '0 fans, 0 follows, likeCount<3' user
        Query<User> users = ds.createQuery(User.class).filter("userAbsInfo.fansCount", 0)
                .filter("userAbsInfo.followCount", 0).filter("userAbsInfo.likeCount <=", 3)
                .retrievedFields(true, "userId");
        List<Long> userIds = new ArrayList<Long>();
        for (User user : users) {
            userIds.add(user.getUserId());
        }
        System.out.println("user num: " + userIds.size());

        Map<Long, Integer> spammer2goodsCnt = new HashMap<Long, Integer>();
        // 2. scan Goods, filter user by 'shareCnt>20'
        int batchSize = 10000;
        long handled = 0;
        long lastGoodsId = Long.MAX_VALUE;
        List<Goods> goods = ds.createQuery(Goods.class).filter("goodsId <", lastGoodsId)
                .retrievedFields(true, "ownerId", "goodsId").limit(batchSize).order("-goodsId")
                .asList();

        while (goods != null && goods.size() > 0) {
            // handle goods
            for (Goods g : goods) {
                long uid = g.getOwnerId();
                if (userIds.contains(new Long(uid))) {
                    if (spammer2goodsCnt.containsKey(uid)) {
                        spammer2goodsCnt.put(uid, spammer2goodsCnt.get(uid) + 1);
                    } else {
                        spammer2goodsCnt.put(uid, 1);
                    }
                }
            }
            handled += goods.size();
            System.out.println("handled: " + handled);

            // get next batch goods
            lastGoodsId = goods.get(goods.size() - 1).getGoodsId();
            System.out.println("last goods id: " + lastGoodsId);
            goods = ds.createQuery(Goods.class).filter("goodsId <", lastGoodsId)
                    .retrievedFields(true, "ownerId", "goodsId").order("-goodsId").limit(batchSize)
                    .asList();
        }

        System.out.println("dump to file...");
        dumpToFile(fileName, spammer2goodsCnt);
    }

    private static void dumpToFile(String fileName, Map<Long, Integer> spammer) {
        List<Map.Entry<Long, Integer>> spammerList = new ArrayList<Map.Entry<Long, Integer>>(
                spammer.entrySet());
        Collections.sort(spammerList, new Comparator<Map.Entry<Long, Integer>>() {

            public int compare(Map.Entry<Long, Integer> o1, Map.Entry<Long, Integer> o2) {
                return (o2.getValue() - o1.getValue());
            }
        });

        try {
            BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(fileName));
            for (Map.Entry<Long, Integer> entry : spammerList) {
                if (entry.getValue() > 30) {
                    byte[] line = new String(entry.getKey() + "\t" + entry.getValue() + "\n")
                            .getBytes();
                    bos.write(line);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public static void saveSpammer() {
        try {
            List<String> lines = FileUtils.readLines(new File("rubbish-goods-spam"));
            for (String line : lines) {
                String[] items = line.split("\\s+");
                long uid = Long.parseLong(items[0]);
                Spammer spammer = new Spammer(uid, dateFormat.format(new Date()), -1, SPAM_REASON);
                ds.save(spammer);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
//        saveSpammer();
    }
}
