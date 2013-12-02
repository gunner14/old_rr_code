package com.renren.ad.sns.services.antispam.model;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.query.Query;
import com.google.code.morphia.query.UpdateOperations;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.data.User;
import com.renren.ad.sns.services.antispam.GJDataStore;

public class SpamHandler {

    private static final Log LOG = LogFactory.getLog(SpamHandler.class);

    private static Datastore datastore = GJDataStore.getDefaultDataStore();

    private static int defaultFansLimit = 10;

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyMMddHHmmss");

    /**
     * 处理spam商品的策略： 1)若用户为非活跃用户，则将用户加入Spammer表，socreDelta累加-spamLevel
     * 2)封禁该商品
     * 
     * @param spamRecords
     */
    public static void handleSpamGoods(List<AntiSpamRecord> spamRecords) {
        for (AntiSpamRecord record : spamRecords) {
            Goods goods = datastore.find(Goods.class, "goodsId", record.goodsId)
                    .retrievedFields(true, "goodsId", "ownerId").get();
            if (goods != null) {
                String msg = "handle goods(" + record.goodsId + "):";
                if (!checkUser(goods.getOwnerId())) {
                    Query<Spammer> querySpammer = datastore.createQuery(Spammer.class).filter(
                            "_id =", goods.getOwnerId());
                    if (querySpammer != null && querySpammer.asList().size() == 1) {
                        int newScoreDelta = (int) (querySpammer.get().getScoreDelta() + (-record.spamLevel));
                        UpdateOperations<Spammer> uo = datastore.createUpdateOperations(
                                Spammer.class).set("scoreDelta", newScoreDelta);
                        datastore.update(querySpammer, uo);
                        msg += "update spammer(" + goods.getOwnerId() + ") scoreDelta to "
                                + newScoreDelta + "; ";
                    } else {
                        Spammer spammer = new Spammer(goods.getOwnerId(),
                                dateFormat.format(new Date()), -record.spamLevel, "like spam");
                        datastore.save(spammer);
                        msg += "insert ownerId(" + goods.getOwnerId() + ") into " + spammer + "; ";
                    }

                } else {
                    msg += "ownerId(" + goods.getOwnerId()
                            + ") pass check, not insert into Spammer; ";
                }

                if (banGoods(record.goodsId)) {
                    msg += "set goods avenueStatus to -1.";
                }
                LOG.info(msg);
            } else {
                LOG.error("No such goods:" + record.goodsId);
            }
        }
    }

    public static void handleSpamGoods(Map<Long, Integer> goods2level) {
        if (goods2level != null && goods2level.size() > 0) {
            // construct a mock record list
            List<AntiSpamRecord> spamRecords = new ArrayList<AntiSpamRecord>();
            for (long goodsId : goods2level.keySet()) {
                int spamLevel = goods2level.get(goodsId);
                AntiSpamRecord record = new AntiSpamRecord();
                record.goodsId = goodsId;
                record.spamLevel = spamLevel;
                spamRecords.add(record);
            }

            handleSpamGoods(spamRecords);
        }
    }

    public static void handleSpamUser(Collection<Long> uids, String reason) {
        for (Long uid : uids) {
            String msg = "handle user(" + uid + "):";
            if (!checkUser(uid)) {
                Spammer spammer = new Spammer(uid, dateFormat.format(new Date()), -1, reason);
                datastore.save(spammer);
                msg += "insert ownerId(" + uid + ") into " + spammer;
            } else {
                msg += "ownerId(" + uid + ") pass check, not insert into Spammer";
            }
        }
    }

    /**
     * 检测用户是否活跃。 若粉丝数小于等于阈值/或头像为默认头像，则为非活跃用户
     * 
     * @param uid
     * @return
     */
    protected static boolean checkUser(long uid) {
        boolean res = true;
        try {
            User user = datastore.find(User.class).filter("userId", uid)
                    .retrievedFields(true, "userAbsInfo.fansCount", "userAbsInfo.picList").get();
            int cnt = user.getUserAbsInfo().getFansCount();
            String pic = user.getUserAbsInfo().getLargeHead();
            if (cnt <= defaultFansLimit || pic == null || pic.endsWith("_main.gif")) {
                res = false;
            }
        } catch (Exception e) {
            res = false;
            e.printStackTrace();
        }
        return res;
    }

    /**
     * 封禁一个商品，置商街状态为负值，使其不被索引
     * 
     * @param goodsId
     * @return 操作是否成功
     */
    public static boolean banGoods(long goodsId) {
        boolean success = true;
        if (datastore != null) {
            final int avenueStatus = -1;
            Query<Goods> query = datastore.createQuery(Goods.class).filter("goodsId =", goodsId);
            if (query.asList().size() == 1) {
                if (query.get().getAvenueSts() != avenueStatus) {
                    UpdateOperations<Goods> uo = datastore.createUpdateOperations(Goods.class).set(
                            "avenueSts", avenueStatus);
                    success = datastore.update(query, uo).getUpdatedCount() == 1 ? true : false;
                }
            } else {
                LOG.warn("No such goods:" + goodsId);
                success = false;
            }
        } else {
            LOG.error("Datastore not ready.");
            success = false;
        }

        return success;
    }

    private static Map<Long, Integer> loadResultFromFiles(String... files) {
        Map<Long, Integer> goods2level = new HashMap<Long, Integer>();

        for (String file : files) {
            try {
                List<String> lines = FileUtils.readLines(new File(file));
                for (String line : lines) {
                    long goodsId = -1;
                    int spamLevel = 1;
                    if (line.split("\\s+").length == 2) {
                        goodsId = Long.parseLong(line.split("\\s+")[0]);
                        spamLevel = Integer.parseInt(line.split("\\s+")[1]);
                    } else {
                        goodsId = Long.parseLong(line);
                    }

                    goods2level.put(goodsId, spamLevel);
                }
            } catch (IOException e) {
                LOG.error("can not read spam result file:" + file);
                e.printStackTrace();
            }
        }
        LOG.info("load " + goods2level.size() + " spam records from files.");

        return goods2level;
    }

    /**
     * 处理spam goods结果文件 结果文件的每一行有两种格式： 1) 仅包含goodsId 2) 包含goodsId spamLevel
     * 
     * @param files
     */
    public static void handleSpamFiles(String... files) {
        //1. load all result from files
        Map<Long, Integer> goods2level = loadResultFromFiles(files);

        //2. handle spam goods
        handleSpamGoods(goods2level);
    }

    public static void main(String[] args) {
        if (args.length >= 1) {
            handleSpamFiles(args);
        } else {
            System.err.println("Usage: " + SpamHandler.class.getSimpleName() + " resultFile...");
        }
    }
}
