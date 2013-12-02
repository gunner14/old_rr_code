package com.renren.ad.sns.services.antispam.rules;

import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.query.Query;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.User;
import com.renren.ad.sns.services.antispam.GJDataStore;
import com.renren.ad.sns.services.antispam.model.SpamHandler;

/**
 * feature: 0 fans, <=1 follows, <=3 likeCnt, >=5 shareGoodsCnt in last 1 days
 * freq: once a day
 * 
 * @author xiangzhang
 * 
 */
public class RubbishGoodsSpam {

    private static Datastore ds = GJDataStore.getDefaultDataStore();

    private static final Log LOG = LogFactory.getLog(RubbishGoodsSpam.class);

    private List<Map.Entry<Long, Integer>> uid2shareCnt = new ArrayList<Map.Entry<Long, Integer>>();

    private String resultFile = "./rubbish-goods-spam";

    private static final String SPAM_REASON = "rubbish_goods_spam";

    public RubbishGoodsSpam(String resultFile) {
        super();
        this.resultFile = resultFile;
    }

    public void run() {
        // 1. scan User, get '0 fans, 0 follows, likeCount<3' user
        Query<User> users = ds.createQuery(User.class).filter("userAbsInfo.fansCount", 0)
                .filter("userAbsInfo.followCount <=", 1).filter("userAbsInfo.likeCount <=", 3)
                .retrievedFields(true, "userId");
        Set<Long> userIds = new HashSet<Long>();
        for (User user : users) {
            userIds.add(user.getUserId());
        }
        LOG.info("inactive user size:" + userIds.size());

        // 2. scan Goods, filter user who share goods>=5 yesterday
        final int SHARE_CNT_T = 5;
        final int THREE_DAYS = -1;
        Calendar cal = Calendar.getInstance();
        cal.add(Calendar.DAY_OF_MONTH, THREE_DAYS);
        SimpleDateFormat sdf = new SimpleDateFormat("yyMMdd");
        long minGoodsId = Long.parseLong(sdf.format(cal.getTime()) + "0000000000000");
        List<Goods> goods = ds.createQuery(Goods.class).filter("goodsId >", minGoodsId)
                .retrievedFields(true, "ownerId", "goodsId").order("-goodsId").asList();
        LOG.info("total goods size:" + goods.size());
        Map<Long, Integer> spammer2goodsCnt = new HashMap<Long, Integer>();
        for (Goods g : goods) {
            long uid = g.getOwnerId();
            if (userIds.contains(uid)) {
                if (spammer2goodsCnt.containsKey(uid)) {
                    spammer2goodsCnt.put(uid, spammer2goodsCnt.get(uid) + 1);
                } else {
                    spammer2goodsCnt.put(uid, 1);
                }
            }
        }

        List<Map.Entry<Long, Integer>> spammerList = new ArrayList<Map.Entry<Long, Integer>>(
                spammer2goodsCnt.entrySet());
        Collections.sort(spammerList, new Comparator<Map.Entry<Long, Integer>>() {

            public int compare(Map.Entry<Long, Integer> o1, Map.Entry<Long, Integer> o2) {
                return (o2.getValue() - o1.getValue());
            }
        });

        for (Map.Entry<Long, Integer> entry : spammerList) {
            if (entry.getValue() < SHARE_CNT_T) {
                break;
            }

            uid2shareCnt.add(entry);
        }
        LOG.info("rubbish-goods-user size:" + uid2shareCnt.size());

        //3. sava & handle spam user
        dumpToFile(resultFile, uid2shareCnt);
        List<Long> spamIds = new ArrayList<Long>();
        for (Map.Entry<Long, Integer> entry : uid2shareCnt) {
            spamIds.add(entry.getKey());
        }
        SpamHandler.handleSpamUser(spamIds, SPAM_REASON);
    }

    private static void dumpToFile(String fileName, List<Map.Entry<Long, Integer>> spammer) {
        BufferedOutputStream bos = null;
        try {
            bos = new BufferedOutputStream(new FileOutputStream(fileName));
            for (Map.Entry<Long, Integer> entry : spammer) {
                byte[] line = new String(entry.getKey() + "\t" + entry.getValue() + "\n")
                        .getBytes();
                bos.write(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (bos != null) {
                try {
                    bos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    public String getResultFile() {
        return resultFile;
    }

    public void setResultFile(String resultFile) {
        this.resultFile = resultFile;
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Usage: RubbishGoodsSpam resultFile");
        } else {
            new RubbishGoodsSpam(args[0]).run();
        }

    }

}
