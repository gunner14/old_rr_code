package com.renren.ad.sns.services.uranus;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.ThreadPoolExecutor.AbortPolicy;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.query.Query;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.SimpleUserReputation;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.util.FileWatchdog;

/**
 * a simple candidate picker user with goods category
 * 
 * @author socialgraph
 * 
 *         modify history: 1) 2012/07/24 a) 将最近10天内发布的商品的tag信息预先载入内存，定期更新；
 *         b) 由于商街展示的是最近的30w商品，但推荐的商品可能不在其中，故增加一个cache，缓存这些商品的tag信息。 c)
 *         生成商品候选集的过程简化，改为”获取到tag对应的商品后，直接并入最终的候选商品中“
 * 
 */
public class CategoryCandidatePicker implements ICandidatePicker {

    private static final Log LOG = LogFactory.getLog(CategoryCandidatePicker.class);

    private static final int CANDIDATE_LIST_MAX_SIZE = 2000;

    /* for simplicity , we preserve double size */
    private static final int CANDIDATE_MIN_SIZE = 24 * 3;

    private static final int CANDIDATE_MAX_SIZE = 200;

    private List<Candidate> hotGoods;

    private static final int HOT_GOODS_SIZE = CANDIDATE_MIN_SIZE * 2;

    public static final double HOT_GOODS_MIN_WEIGHT = 3.07;

    public static final double HOT_GOODS_MAX_WEIGHT = 3.08;

    private final static int SG_LEN = 7;

    private Datastore datastore;

    private Map<Long, CandidateList> tag2Candidate;

    private FileWatchdog goodsDataWatchdog;

    private String dataPath;

    /* 商品到tag的map */
    private Map<Long, List<Long>> goods2tagList;

    private Map<Long, List<Long>> goods2tagListCache;

    private Map<Long, Integer> goods2category;

    private static final long GOODS2TAGLIST_CACHE_MAX_SIZE = 10 * 10000;

    private ExecutorService executor;

    public static final long QUERY_DB_TIMEOUT = 100;

    public static final long TAG_2_CANDIDATE_TIMEOUT = 100;

    /* ThreadPoolExecutor parameters */
    public static final int TPE_CORE_SIZE = 80;

    public static final int TPE_MAX_SIZE = 100;

    public static final int TPE_QUEUE_SIZE = 1;

    public static final long TPE_KEEP_ALIVE_TIME = 60;

    /* lock for object which will be updated periodically */
    public final Object hotGoodsLock = new byte[0];

    public final Object tag2CandidateLock = new byte[0];

    public final Object goods2tagListLock = new byte[0];

    public final Object goods2tagListCacheLock = new byte[0];

    public final Object goods2categoryLock = new byte[0];

    public final Object userReputationLock = new byte[0];

    private ITagList2CandidatePicker[] taglist2candidatePicker;

    private Map<Integer, Float> userReputaion;

    public CategoryCandidatePicker(Datastore datastore,
            ITagList2CandidatePicker[] taglist2candidatePicker) {
        super();
        this.datastore = datastore;
        this.taglist2candidatePicker = taglist2candidatePicker;
        /* 当并行任务总数超过105时，直接丢弃新提交的任务，保护系统资源 */
        executor = new ThreadPoolExecutor(TPE_CORE_SIZE, TPE_MAX_SIZE, TPE_KEEP_ALIVE_TIME,
                TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(TPE_QUEUE_SIZE),
                new AbortPolicy());
    }

    public void init() {
        hotGoods = new ArrayList<Candidate>();
        tag2Candidate = new HashMap<Long, CandidateList>();
        goods2tagList = new HashMap<Long, List<Long>>();
        goods2tagListCache = new HashMap<Long, List<Long>>();
        goods2category = new HashMap<Long, Integer>();

        userReputaion = new HashMap<Integer, Float>();
        loadUserReputation();
        Thread refreshUserRepuThread = new Thread(new Runnable() {

            @Override
            public void run() {
                try {
                    Thread.sleep(60 * 60 * 1000);
                    loadUserReputation();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

        });
        refreshUserRepuThread.setDaemon(true);
        refreshUserRepuThread.start();

        goodsDataWatchdog = new FileWatchdog(dataPath + File.separator + "goods.dat") {

            @Override
            protected void doOnChange() {
                updateCandidate(this.filename);
            }

        };
        goodsDataWatchdog.setDelay(5 * 60 * 1000);
        goodsDataWatchdog.start();
    }

    private void loadUserReputation() {
        List<SimpleUserReputation> repu = datastore.find(SimpleUserReputation.class)
                .retrievedFields(true, "userId", "score").asList();
        Map<Integer, Float> tmpUserReputation = new HashMap<Integer, Float>();
        if (repu != null) {
            for (SimpleUserReputation sur : repu) {
                tmpUserReputation.put((int) sur.getUserId(), (float) sur.getScore());
            }

            synchronized (userReputationLock) {
                this.userReputaion = tmpUserReputation;
            }
            LOG.info("Updated user reputation, size:" + userReputaion.size());
        } else {
            LOG.warn("Can not load user reputation from DB.");
        }
    }

    class QueryGoodsInfoTask implements Callable<Goods> {

        private long goodsId;

        public QueryGoodsInfoTask(long gid) {
            goodsId = gid;
        }

        @Override
        public Goods call() throws Exception {
            Goods goods = null;
            goods = datastore.find(Goods.class, "goodsId", goodsId).retrievedFields(true,
                    "goodsId", "tagList").get();
            return goods;
        }
    }

    class TagList2CandidateTask implements Callable<List<Candidate>> {

        private long gid;

        private List<Long> tagList;

        public TagList2CandidateTask(long gid, List<Long> tagList) {
            this.gid = gid;
            this.tagList = tagList;
        }

        @Override
        public List<Candidate> call() throws Exception {
            List<Candidate> candidates = new ArrayList<Candidate>();
            final Candidate excludeCandidate = new Candidate(gid, 0);
            synchronized (tag2CandidateLock) {
                for (Long tag : tagList) {
                    if (tag2Candidate.containsKey(tag)) {
                        CandidateList clist = tag2Candidate.get(tag);
                        candidates = CandidateList.mergeAndExclude(candidates, clist,
                                excludeCandidate, CANDIDATE_MAX_SIZE);
                        if (candidates.size() >= CANDIDATE_MAX_SIZE) {
                            break;
                        }
                    }
                }
            }

            return candidates;
        }

    }

    @Override
    /**
     * Choose candidates which has at least one same tag with the current goods. 
     * @param uid
     * @param gid
     * @return a list of goods
     */
    public List<Candidate> pick(long uid, long gid) {
        List<Candidate> candidates = new ArrayList<Candidate>();
        long begin = System.currentTimeMillis();

        try {
            List<Long> tagList = null;
            if (goods2tagList.containsKey(gid)) {
                synchronized (goods2tagListLock) {
                    if (goods2tagList.containsKey(gid)) {
                        tagList = goods2tagList.get(gid);
                    }
                }
            } else if (goods2tagListCache.containsKey(gid)) {
                synchronized (goods2tagListCacheLock) {
                    if (goods2tagListCache.containsKey(gid)) {
                        tagList = goods2tagListCache.get(gid);
                    }
                }
            } else {
                long dbBegin = System.currentTimeMillis();
                Goods goods = null;
                try {
                    Future<Goods> future = executor.submit(new QueryGoodsInfoTask(gid));
                    goods = future.get(QUERY_DB_TIMEOUT, TimeUnit.MILLISECONDS);
                } catch (TimeoutException te) {
                    LOG.error("qurey goods info timeout.");
                } catch (RejectedExecutionException re) {
                    LOG.error("qurey goods info reject.");
                } catch (Exception e) {
                    LOG.error(e.getMessage(), e);
                }

                if (goods != null && goods.getTagList() != null && goods.getTagList().size() > 0) {
                    tagList = goods.getTagList();
                    addToCache(gid, tagList);
                }
                LOG.debug("query db used:" + (System.currentTimeMillis() - dbBegin));
            }
            LOG.debug("1. pick taglist used:" + (System.currentTimeMillis() - begin));

            long tagBegin = System.currentTimeMillis();
            if (tagList != null && tagList.size() > 0) {
                synchronized (tag2CandidateLock) {
                    synchronized (goods2categoryLock) {
                        for (ITagList2CandidatePicker picker : taglist2candidatePicker) {
                            //candidates.addAll(picker.pick(gid, tagList, tag2Candidate));
                            candidates.addAll(picker.pick(gid, tagList, tag2Candidate,
                                    goods2category));
                        }
                    }
                }
            }

            LOG.debug("2. tag2candidate used:" + (System.currentTimeMillis() - tagBegin));
        } catch (Exception e) {
            LOG.error(e.getMessage(), e);
        } finally {
            if (candidates.size() < CANDIDATE_MIN_SIZE) {
                // try to padding the left empty candidates with hot goods if any
                synchronized (hotGoodsLock) {
                    for (int i = candidates.size(), j = 0; i < CANDIDATE_MIN_SIZE
                            && j < hotGoods.size();) {
                        Candidate c = hotGoods.get(j);
                        if (!candidates.contains(c)) {
                            candidates.add(c);
                            i++;
                            j++;
                        } else {
                            j++;
                        }
                    }
                }
            }
        }

        LOG.debug("total used:" + (System.currentTimeMillis() - begin));
        return candidates;
    }

    private void addToCache(long gid, List<Long> tagList) {
        if (goods2tagListCache.size() <= GOODS2TAGLIST_CACHE_MAX_SIZE) {
            synchronized (goods2tagListCacheLock) {
                goods2tagListCache.put(gid, tagList);
            }
        }
    }

    /**
     * 获取热门商品作为候选商品
     * 
     * @return
     */
    public List<Candidate> pickDefault(long uid, long gid) {
        List<Candidate> result = new ArrayList<Candidate>();
        synchronized (hotGoodsLock) {
            if (hotGoods != null && hotGoods.size() > 0) {
                for (int i = 0; i < hotGoods.size() && i < CANDIDATE_MIN_SIZE; i++) {
                    result.add(hotGoods.get(i));
                }
            }
        }

        return result;
    }

    private void updateCandidate(String filename) {
        LOG.info("begin to update candidate ...");
        long t_beg = System.currentTimeMillis();
        Map<Long, Spammer> spammers = loadSpammer();
        List<Goods> newGoods = loadGoods(filename);

        Map<Long, CandidateList> tmpTag2Candidate = new HashMap<Long, CandidateList>();
        Map<Long, List<Long>> tmpGoods2tagList = new HashMap<Long, List<Long>>();
        Map<Long, Integer> tmpGoods2category = new HashMap<Long, Integer>();

        for (Goods g : newGoods) {
            long ownerId = g.getOwnerId();
            long goodsId = g.getGoodsId();
            double weight = score(g);
            List<Long> tags = g.getTagList();
            int catID = g.getCategoryId();

            if (spammers.containsKey(ownerId)
                    || g.getLink().startsWith(CandidatesFilterUtil.BAN_URL)) {
                continue;
            }

            tmpGoods2tagList.put(goodsId, tags);

            Candidate candidate = new Candidate(g.getGoodsId(), weight,
                    CandidateType.CategoryCandidatePicker);
            for (Long tag : tags) {
                CandidateList candidateList = null;
                if (tmpTag2Candidate.containsKey(tag)) {
                    candidateList = tmpTag2Candidate.get(tag);
                } else {
                    candidateList = new CandidateList(CANDIDATE_LIST_MAX_SIZE);
                }
                candidateList.add(candidate);
                tmpTag2Candidate.put(tag, candidateList);
            }

            tmpGoods2category.put(goodsId, catID);
        }

        if (tmpTag2Candidate.size() > 0) {
            synchronized (tag2CandidateLock) {
                tag2Candidate = tmpTag2Candidate;
            }
            LOG.info("update candidate over , tag2Candidate size = " + tag2Candidate.size()
                    + ", elapse " + (System.currentTimeMillis() - t_beg) + " ms!");
        }
        if (tmpGoods2tagList.size() > 0) {
            synchronized (goods2tagListLock) {
                goods2tagList = tmpGoods2tagList;
            }
            LOG.info("update goods2tagList over , size = " + goods2tagList.size() + ", elapse "
                    + (System.currentTimeMillis() - t_beg) + " ms!");
        }

        if (tmpGoods2category.size() > 0) {
            synchronized (goods2categoryLock) {
                goods2category = tmpGoods2category;
            }
            LOG.info("update goods2category over , size = " + goods2category.size() + ", elapse "
                    + (System.currentTimeMillis() - t_beg) + " ms!");
        }

        if (newGoods != null) {
            // update the hot goods
            updateHotGoods(newGoods, spammers);
        }
    }

    private void updateHotGoods(List<Goods> goods, final Map<Long, Spammer> spammers) {
        LOG.info("Begin to update hot goods list...");
        List<Candidate> newHotGoods = new ArrayList<Candidate>();
        /*
         * sort the goods list according to the following rules: 1) if two goods
         * are both published by spammers, they are regards as equal (bad). 2)
         * if one goods is published by a spammer, the other is a normal goods,
         * then normal goods is smaller then spam goods. 3) if two goods are
         * both normal, then compare them by the 'score(Goods)' method.
         */
        Collections.sort(goods, new Comparator<Goods>() {

            @Override
            public int compare(Goods o1, Goods o2) {
                if (spammers.containsKey(o1.getOwnerId()) && spammers.containsKey(o2.getOwnerId())) {
                    return 0;
                } else if (spammers.containsKey(o1.getOwnerId())
                        && !spammers.containsKey(o2.getOwnerId())) {
                    return 1;
                } else if (!spammers.containsKey(o1.getOwnerId())
                        && spammers.containsKey(o2.getOwnerId())) {
                    return -1;
                } else {
                    double scoreO1 = score(o1);
                    double scoreO2 = score(o2);
                    if (scoreO1 > scoreO2) {
                        return -1;
                    } else if (scoreO1 < scoreO2) {
                        return 1;
                    } else {
                        return 0;
                    }
                }
            }
        });

        double minWeight = Double.MAX_VALUE;
        double maxWeight = Double.MIN_VALUE;
        for (int i = 0; i < HOT_GOODS_SIZE && i < goods.size(); i++) {
            long gid = goods.get(i).getGoodsId();
            double weight = score(goods.get(i));
            newHotGoods.add(new Candidate(gid, weight, CandidateType.HotGoods));
            if (weight > maxWeight) {
                maxWeight = weight;
            } else if (weight < minWeight) {
                minWeight = weight;
            }
        }

        if (hotGoods == null || newHotGoods.size() == HOT_GOODS_SIZE) {
            rescoreHotGoodsWeight(newHotGoods, minWeight, maxWeight);
            synchronized (hotGoodsLock) {
                hotGoods = newHotGoods;
            }
            LOG.info("Finish updating the hot goods list, size:" + hotGoods.size());
        } else {
            // if we can not get enough hot goods, just use the previous one
            LOG.warn("Can not get enough hot goods. expected size:" + HOT_GOODS_SIZE
                    + ", actual size:" + newHotGoods.size());
        }
    }

    private void rescoreHotGoodsWeight(List<Candidate> candidates, double min, double max) {
        final double NEW_RANGE = HOT_GOODS_MAX_WEIGHT - HOT_GOODS_MIN_WEIGHT;
        final double OLD_RANGE = max - min;
        for (Candidate c : candidates) {
            double newWeight = (HOT_GOODS_MIN_WEIGHT + NEW_RANGE * (c.getWeight() - min)
                    / OLD_RANGE);
            c.setWeight(newWeight);
        }
    }

    private Map<Long, Spammer> loadSpammer() {
        long beg = System.currentTimeMillis();
        LOG.info("loading spammers ...");
        Map<Long, Spammer> avaliableSpammers = new HashMap<Long, Spammer>();
        Query<Spammer> query = datastore.createQuery(Spammer.class);
        query.or(query.criteria("expiredTime").greaterThanOrEq(System.currentTimeMillis()), query
                .criteria("expiredTime").lessThanOrEq(0));
        List<Spammer> spammers = query.asList();
        if (spammers != null && spammers.size() > 0) {
            for (Spammer spammer : spammers) {
                LOG.debug(spammer);
                avaliableSpammers.put(spammer.getUid(), spammer);
            }
        }
        long time = System.currentTimeMillis() - beg;
        LOG.info("loaded " + avaliableSpammers.size() + " spammer over , elapsed " + time + " ms!");
        return avaliableSpammers;
    }

    private List<Goods> loadGoods(String filename) {
        long beg = System.currentTimeMillis();
        List<Goods> all = new ArrayList<Goods>();
        Scanner cin;
        try {
            cin = new Scanner(new FileInputStream(filename));
            while (cin.hasNext()) {
                String line = cin.nextLine();
                try {
                    String[] tmps = line.split(",");
                    Goods goods = new Goods();
                    if (tmps.length >= SG_LEN) {
                        goods.setGoodsId(Long.parseLong(tmps[0]));
                        goods.setOwnerId(Long.parseLong(tmps[1]));
                        goods.setCategoryId(Integer.parseInt(tmps[2]));
                        goods.setPrice(Integer.parseInt(tmps[3]));
                        goods.setShareCount(Integer.parseInt(tmps[4]));
                        goods.setLikeCount(Integer.parseInt(tmps[6]));
                        goods.setLink(tmps[7]);
                        // parse tag list
                        List<Long> tags = new ArrayList<Long>();
                        if (tmps.length == SG_LEN + 2) {
                            String stags = tmps[tmps.length - 1];
                            String[] ttags = stags.split(":");
                            for (String stag : ttags) {
                                try {
                                    long tag = Long.parseLong(stag);
                                    tags.add(tag);
                                } catch (Exception e) {
                                    LOG.error("parse tag list error " + stags, e);
                                }
                            }
                        }
                        goods.setTagList(tags);
                        all.add(goods);
                    }
                } catch (Exception e) {
                    LOG.error("Parse U2C Exception : " + line, e);
                }
            }
            cin.close();
        } catch (FileNotFoundException e) {
            LOG.error("load new goods.dat error!", e);
        }
        Collections.sort(all, new LinkComparator());
        String last = "";
        String link = last;
        List<Goods> res = new ArrayList<Goods>();
        for (Goods g : all) {
            link = g.getLink();
            if (last.compareTo(link) != 0) {
                res.add(g);
            }
            last = link;
        }
        long time = System.currentTimeMillis() - beg;
        LOG.info("loaded " + res.size() + " goods over , elapsed " + time + " ms!");
        return res;
    }

    private float normBiggerBetter(float raw, float maxVal) {
        return maxVal * raw / (1 + raw);
    }

    private float normSmallerBetter(float raw, float maxVal) {
        return maxVal / (1 + raw);
    }

    private double score(Goods goods) {

        if (goods.getSource() == 1) {
            return 0;
        }

        final int repuWeight = 60;
        final int likeCntWeight = 30;
        final int shareCntWeight = 5;
        final int timeWeight = 5;
        int ownerID = (int) goods.getOwnerId();
        float repuScore = 0.0f;
        synchronized (userReputationLock) {
            if (userReputaion.containsKey(new Long(ownerID))) {
                repuScore = normBiggerBetter(userReputaion.get(new Long(ownerID)), repuWeight);
            }
        }
        float likeCntScore = normBiggerBetter(goods.getLikeCount(), likeCntWeight);
        float shareCntScore = normBiggerBetter(goods.getShareCount(), shareCntWeight);
        int deltaTime = (int) (System.currentTimeMillis() - getDate(goods.getGoodsId()).getTime())
                / (1000 * 60 * 60);
        float timeScore = normSmallerBetter(deltaTime, timeWeight);
        return repuScore + likeCntScore + shareCntScore + timeScore;
    }

    public static Date getDate(long goodsID) {
        DateFormat df = new SimpleDateFormat("yyMMddHHmmss");
        String dateStr = new String(goodsID + "").substring(0, 12);
        Date date = null;
        try {
            date = df.parse(dateStr);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return date;
    }

    class LinkComparator implements Comparator<Goods> {

        @Override
        public int compare(Goods g1, Goods g2) {
            String link1 = g1.getLink();
            if (link1 == null) {
                link1 = "";
                g1.setLink("");
            }
            String link2 = g2.getLink();
            if (link2 == null) {
                link2 = "";
                g2.setLink("");
            }

            int res = link1.compareTo(link2);
            if (res == 0) {
                res = g1.getGoodsId() - g2.getGoodsId() > 0 ? 1 : -1;
            }
            return res;
        }
    }

    public void setDataPath(String dataPath) {
        this.dataPath = dataPath;
    }

    public Map<Long, CandidateList> getTag2Candidate() {
        return tag2Candidate;
    }

    public FileWatchdog getGoodsDataWatchdog() {
        return goodsDataWatchdog;
    }

    public List<Candidate> getHotGoods() {
        return hotGoods;
    }

    public static void main(String[] args) {
        long gid = 1202281440571267925L;
        System.out.println(getDate(gid));
    }
}
