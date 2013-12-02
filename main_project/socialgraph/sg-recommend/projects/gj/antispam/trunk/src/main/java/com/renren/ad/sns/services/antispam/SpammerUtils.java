package com.renren.ad.sns.services.antispam;

import java.io.FileInputStream;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;

import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.DatastoreImpl;
import com.google.code.morphia.Morphia;
import com.google.code.morphia.query.Query;
import com.google.code.morphia.query.UpdateOperations;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.LikeRecord;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.data.User;

public class SpammerUtils {

    static Datastore datastore;

    static Options opts;

    static SimpleDateFormat format;

    private int defaultFansLimit = 10;

    private static final Log LOG = LogFactory.getLog(SpammerUtils.class);

    static {
        format = new SimpleDateFormat("yyMMddHHmmss");
        Morphia morphia = new Morphia();
        Mongo mongo = null;
        try {
            mongo = new Mongo("db1.jie.d.xiaonei.com", 62166);
        } catch (UnknownHostException e) {
            e.printStackTrace();
            System.exit(1);
        } catch (MongoException e) {
            e.printStackTrace();
            System.exit(1);
        }
        datastore = new DatastoreImpl(morphia, mongo, "sns_online", "gj", "dec2011online4nb"
                .toCharArray());
        opts = new Options();
        opts.addOption("h", false, "print help for this application");
        opts.addOption("u", false, "with users's model");
        opts.addOption("g", false, "with goods's model");
        opts.addOption("l", false, "with like record's model");
        opts
                .addOption(
                        "n",
                        true,
                        "fans count up bound , if the user's fans count below or eqauls the limit , he/she wr regarded as a spammer , the default value is 10");
        opts.addOption("f", true, "the file cantainers goods' id or users' id");
    }

    public SpammerUtils(int defaultFansLimit) {
        super();
        this.defaultFansLimit = defaultFansLimit;
    }

    public SpammerUtils() {
        super();
    }

    /**
     * 商品被刷喜欢,处理方式为：1）将ownerId加入Spammer表中；2）置商品的商街状态为-1
     * 
     * @param file
     * @return
     */
    public boolean fromGids(String file) {
        boolean res = true;
        try {
            Scanner cin = new Scanner(new FileInputStream(file));
            while (cin.hasNext()) {
                long gid = cin.nextLong();
                Goods goods = datastore.find(Goods.class, "goodsId", gid).retrievedFields(true,
                        "goodsId", "ownerId").get();
                if (goods != null) {
                    String msg = "handle goods(" + gid + "):";
                    if (!check(goods.getOwnerId())) {
                        Spammer spammer = new Spammer(goods.getOwnerId(),
                                format.format(new Date()), -1l, "like spam");
                        datastore.save(spammer);
                        msg += "insert ownerId(" + goods.getOwnerId() + ") into " + spammer + "; ";
                    } else {
                        msg += "ownerId(" + goods.getOwnerId()
                                + ") pass check, not insert into Spammer; ";
                    }
                    if (banGoods(gid)) {
                        msg += "set goods avenueStatus to -1.";
                    }
                    LOG.info(msg);
                } else {
                    LOG.error("No such goods:" + gid);
                }
            }
            cin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public boolean fromUids(String file) {
        boolean res = true;
        try {
            Scanner cin = new Scanner(new FileInputStream(file));
            while (cin.hasNext()) {
                long uid = cin.nextLong();
                String msg = "handle user(" + uid + "):";
                if (!check(uid)) {
                    Spammer spammer = new Spammer(uid, format.format(new Date()), -1l, "like spam");
                    datastore.save(spammer);
                    msg += "insert into " + spammer;
                } else {
                    msg += "user pass check, not insert into Spammer.";
                }
                LOG.info(msg);
            }
            cin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public boolean fromUidAndGids(String file) {
        boolean res = true;
        try {
            Scanner cin = new Scanner(new FileInputStream(file));
            while (cin.hasNext()) {
                long uid = cin.nextLong();
                long gid = cin.nextLong();
                delUserLikeForGoods(uid, gid);
                delUserLikeForUser(uid, gid);
                delLikeRecord(uid, gid);
                System.out.println("done for uid=" + uid + " gid =" + gid);
            }
            cin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    private boolean delLikeRecord(long uid, long gid) {
        boolean res = false;
        try {
            Query<LikeRecord> updateQuery = datastore.createQuery(LikeRecord.class).field("userId")
                    .equal(uid).field("goodsId").equal(gid);
            datastore.findAndDelete(updateQuery);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    private boolean delUserLikeForGoods(long userId, long goodsId) {
        boolean res = false;
        try {
            Query<Goods> updateQuery = datastore.createQuery(Goods.class).field(newField(goodsId))
                    .equal(goodsId);
            UpdateOperations<Goods> ops = datastore.createUpdateOperations(Goods.class).removeAll(
                    "userLikeList", userId).dec("likeCount");
            datastore.update(updateQuery, ops);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    private boolean delUserLikeForUser(long userId, long goodsId) {
        boolean flag = false;
        try {
            Query<User> query = datastore.createQuery(User.class).field("userId").equal(userId);
            UpdateOperations<User> ops = datastore.createUpdateOperations(User.class).removeAll(
                    "likeList", goodsId);
            datastore.update(query, ops);
            flag = true;
        } catch (MongoException e) {
            e.printStackTrace();
        }
        return flag;
    }

    private String newField(long entityId) {
        String ids = String.valueOf(entityId);
        if (ids.length() == 19) return "goodsId";
        else return "entityId";
    }

    private boolean check(long uid) {
        boolean res = true;
        try {
            User user = datastore.find(User.class).filter("userId", uid).retrievedFields(true,
                    "userAbsInfo.fansCount", "userAbsInfo.picList").get();
            int cnt = user.getUserAbsInfo().getFansCount();
            System.out.println(uid + " : " + cnt + " / " + defaultFansLimit);
            if (cnt <= defaultFansLimit) {
                res = false;
            }
            String pic = user.getUserAbsInfo().getLargeHead();
            System.out.println(uid + " : " + pic);
            if (pic == null || pic.endsWith("_main.gif")) {
                res = false;
            }
        } catch (Exception e) {
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

    public static void main(String[] args) {
        BasicParser parser = new BasicParser();
        try {
            CommandLine cl = parser.parse(opts, args);
            if (cl.hasOption("h")) {
                HelpFormatter hf = new HelpFormatter();
                hf.printHelp("Spammer Utils", opts);
            }
            int cnt = 10;
            if (cl.hasOption("n")) {
                String num = cl.getOptionValue("n");
                cnt = Integer.parseInt(num);
            }

            if (cl.hasOption("f")) {
                String file = cl.getOptionValue("f");
                System.out.println(file);
                SpammerUtils spammerUtils;
                if (cnt >= 0) {
                    spammerUtils = new SpammerUtils(cnt);
                } else {
                    spammerUtils = new SpammerUtils();
                }
                if (cl.hasOption("g")) {
                    spammerUtils.fromGids(file);
                } else if (cl.hasOption("u")) {
                    spammerUtils.fromUids(file);
                } else if (cl.hasOption("l")) {
                    spammerUtils.fromUidAndGids(file);
                }
            } else {
                System.out.println("must specify the file containes user ids or goods ids ");
            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }
}
