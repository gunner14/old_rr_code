package com.renren.ad.sns.services.uranus;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.query.Query;
import com.renren.ad.sns.data.CommonList;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.util.Constant;

public class DefaultCandidatesFilter implements CandidatesFilter {

    protected static final Log LOG = LogFactory.getLog(DefaultCandidatesFilter.class);

    public static int MAX_GOODS_SIZE = 30 * 10000;

    public static long UPDATE_GOODS_INFO_INTERVAL = 30 * 60 * 1000;

    public static long UPDATE_SPAMMER_INTERVAL = 3 * 60 * 1000;

    private Datastore datastore;

    /* 最近发布的商品信息 */
    private Map<Long, GoodsInfo> goodsInfo = new HashMap<Long, GoodsInfo>();

    /* Spammer 集合 */
    private Set<Long> spammer = new HashSet<Long>();

    /* Spammer发布的商品集合 */
    public Set<Long> spamGoods = new HashSet<Long>();

    private Thread updateGoodsThread = new Thread() {

        @Override
        public void run() {
            while (true) {
                try {
                    Thread.sleep(UPDATE_GOODS_INFO_INTERVAL);
                    loadGoodsInfo();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    };

    private Thread updateSpammerThread = new Thread() {

        @Override
        public void run() {
            while (true) {
                try {
                    Thread.sleep(UPDATE_SPAMMER_INTERVAL);
                    loadSpammer();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    };

    static class GoodsInfo {

        public long goodsId;

        public long ownerId;

        public String link;

        public int picCount;

        public GoodsInfo(long goodsId, long ownerId, String link, int picCount) {
            super();
            this.goodsId = goodsId;
            this.ownerId = ownerId;
            this.link = link/*CandidatesFilterUtil.getShortLink(link)*/;
            this.picCount = picCount;
        }

        public static GoodsInfo fromGoods(Goods goods) {
            return new GoodsInfo(goods.getGoodsId(), goods.getOwnerId(), goods.getLink(), goods
                    .getPicLinkList() == null ? 0 : goods.getPicLinkList().size());
        }
    }

    public DefaultCandidatesFilter(Datastore datastore) {
        this.datastore = datastore;
        loadSpammer();
        loadGoodsInfo();
        updateGoodsThread.setDaemon(true);
        updateGoodsThread.start();
        updateSpammerThread.setDaemon(true);
        updateSpammerThread.start();
    }

    /**
     * 加载Spammer以及Spammer所发布的商品
     */
    public void loadSpammer() {
        Query<Spammer> query = datastore.createQuery(Spammer.class);
        query.or(query.criteria("expiredTime").greaterThanOrEq(System.currentTimeMillis()), query
                .criteria("expiredTime").lessThanOrEq(0));
        List<Spammer> result = query.retrievedFields(true, "_id").asList();
        Set<Long> tmpSpammerIds = new HashSet<Long>();
        if (result != null && result.size() > 0) {
            for (Spammer s : result) {
                tmpSpammerIds.add(s.getUid());
            }
        } else {
            LOG.warn("Fail to load spammer from Spammer collection.");
        }
        
        tmpSpammerIds.addAll(loadSpammerInCommonList());
        
        if(tmpSpammerIds.size() > 0){
            spammer = tmpSpammerIds;
            LOG.info("Updated spammer:" + spammer.size());
        }
    }

    private Set<Long> loadSpammerInCommonList() {
        Set<Long> spamList = new HashSet<Long>(); 
        CommonList result = datastore.find(CommonList.class).filter("name", "SPAM_LIKE")
                .retrievedFields(true, "userIds").asList().get(0);
        if(result != null && result.getUserIds().size() > 0){
            spamList.addAll(result.getUserIds());
            LOG.info(String.format("Load %s spammer from CommonList.", spamList.size()));
        }
        return spamList;
    }

    private void appendGoodsInfo(Map<Long, GoodsInfo> data, List<Goods> goodsInfo) {
        for (Goods goods : goodsInfo) {
            GoodsInfo gi = new GoodsInfo(goods.getGoodsId(), goods.getOwnerId(), goods.getLink(),
                    goods.getPicLinkList() == null ? 0 : goods.getPicLinkList().size());
            data.put(goods.getGoodsId(), gi);
        }
    }

    
    /**
     * 加载最新的商品信息
     */
    public void loadGoodsInfo() {
        Map<Long, GoodsInfo> tmpGoodsInfo = new HashMap<Long, GoodsInfo>();
        int batchSize = 10000;
        long loadedSize = 0;
        long lastGoodsId = Long.MAX_VALUE;
        if (loadedSize < MAX_GOODS_SIZE) {
            List<Goods> goodsMeta = datastore.find(Goods.class).filter("goodsId <", lastGoodsId)
                    .filter("avenueSts >=", Constant.AVENUE_STS_NORMAL).retrievedFields(true,
                            "goodsId", "link", "picLinkList", "ownerId").order("-goodsId").limit(
                            batchSize).asList();
            while (goodsMeta != null && goodsMeta.size() > 0 && loadedSize < MAX_GOODS_SIZE) {
                appendGoodsInfo(tmpGoodsInfo, goodsMeta);
                LOG.info("Loaded " + (loadedSize += goodsMeta.size()));
                lastGoodsId = goodsMeta.get(goodsMeta.size() - 1).getGoodsId();
                goodsMeta = datastore.find(Goods.class).filter("goodsId <", lastGoodsId).filter(
                        "avenueSts >=", Constant.AVENUE_STS_NORMAL).retrievedFields(true,
                        "goodsId", "link", "picLinkList", "ownerId").order("-goodsId").limit(
                        batchSize).asList();
            }
        }

        if (tmpGoodsInfo != null && tmpGoodsInfo.size() > 0) {
            goodsInfo = tmpGoodsInfo;
            LOG.info("Updated goods info:" + goodsInfo.size());
        } else {
            LOG.warn("Fail to load goods info from DB.");
        }
    }

    /* for testing purpose only */
    public void addGoodsInfo(GoodsInfo gi) {
        goodsInfo.put(gi.goodsId, gi);
    }

    /* for testing purpose only */
    public void addSpamGoods(long gid) {
        spamGoods.add(gid);
    }

    /* for testing purpose only */
    public void addSpammer(long uid) {
        spammer.add(uid);
    }

    @Override
    /**
     * 滤掉满足以下任一条件的候选商品：
     * 1)goodsId == gid
     * 2)goodsId相同的商品
     * 3)goods.ownerId in spammerIdList
     * 4)goods.ownerId == uid
     * 5)goods.link相同的商品
     * 6)不含有效商品图片的商品
     * 7)link以http://s.click.taobao.com开头的商品
     *
     */
    public List<Candidate> filter(List<Candidate> candidates, long uid, long gid) {
        List<Candidate> result = new ArrayList<Candidate>();
        // filter out goodsId same with gid(1) and duplicate goodsId(2)
        Set<Long> uniqGoodsId = new HashSet<Long>();
        for (Candidate c : candidates) {
            if (c.getGid() != gid && !uniqGoodsId.contains(c.getGid())) {
                result.add(c);
                uniqGoodsId.add(c.getGid());
            }
        }

        // get goods data, and filter out goods meeting condition(3),(4),(5),(6),(7)
        List<Candidate> tmp = new ArrayList<Candidate>();
        Set<String> uniqLink = new HashSet<String>();
        for (Candidate c : result) {
            if (goodsInfo.containsKey(c.getGid())) {
                GoodsInfo gi = goodsInfo.get(c.getGid());
                if (gi.ownerId != uid && !spammer.contains(gi.ownerId)
                        && !uniqLink.contains(gi.link) && gi.picCount > 0
                        && !gi.link.startsWith(CandidatesFilterUtil.BAN_URL)) { //(3),(4),(5),(6),(7)
                    tmp.add(c);
                    uniqLink.add(gi.link);
                }
            } else if (!spamGoods.contains(c.getGid())) { //(3)
                tmp.add(c);
            }
        }
        result = tmp;

        Collections.sort(result, Collections.reverseOrder());
        return result;
    }

    public Set<Long> getSpammer() {
        return spammer;
    }

    public Datastore getDatastore() {
        return datastore;
    }

}
