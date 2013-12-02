package com.renren.ad.sns.services.avenue;

import java.util.ArrayList;
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
import com.renren.ad.sns.data.CommonList;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.GoodsHidden;
import com.renren.ad.sns.data.SimpleUserReputation;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.data.Subtags;
import com.renren.ad.sns.data.TopicList;
import com.renren.ad.sns.data.VUser;
import com.renren.ad.sns.services.avenue.filter.GoodsCandidateFilter;
import com.renren.ad.sns.services.avenue.protocol.IndexType;
import com.renren.ad.sns.services.avenue.score.GoodsScoreParam;
import com.renren.ad.sns.util.Constant;
import com.xiaonei.xce.buddyadapter.buddybyidcache.BuddyByIdCacheAdapter;

/**
 * load data
 * 
 * @author xiong.hua
 */
public class DataHammer {

    private Datastore datastore;

    private List<GoodsCandidateFilter> goodsFilters;

    // private ReputationBuilder builder;

    private static final Log LOG = LogFactory.getLog(DataHammer.class);

    public boolean isTest = false;

    public List<Long> testGoodsIds = new ArrayList<Long>();

    // private AvenueSortParameter avenueSortParameter = new
    // AvenueSortParameter();

    private Map<IndexType, GoodsScoreParam> goodsScoreParam = new HashMap<IndexType, GoodsScoreParam>();

    private Map<String, Double> parameters = new HashMap<String, Double>();

    private Map<Long, GoodsMeta> avaliableGoods;

    private Map<Long, Spammer> avaliableSpammers;

    private Map<Long, SimpleUserReputation> avaliableReputations;

    private Map<Long, String> credibleUsers = new HashMap<Long, String>();

    private Map<Long, String> uncredibleUsers = new HashMap<Long, String>();

    private Map<Integer, Integer> id2fs = new HashMap<Integer, Integer>();// key:userId,value:人人好友个数

    private Set<Long> starUsers = new HashSet<Long>(); // 逛街达人
    
    private Set<Long> spamLikeUsers = new HashSet<Long>(); // 刷喜欢的用户.这些用户的喜欢会被计为0
    
    private Set<Long> operationUsers= new HashSet<Long>(); // 运营人员的数据，在紧急情况下，可以把他们的商品进行展示
    
    private List<Subtags> subtags = new ArrayList<Subtags>(); // 商街页子标签
    public Set<Long> getSpamLikeUsers() {
		return spamLikeUsers;
	}

	private Set<Long> goodsHidden=new HashSet<Long>();
    private static final int GOODS_MAX_SIZE = 30 * 10000/*300000*/;
    private static final int GET_GOODS_MAX_TIME = 5 * 60 * 1000/*5min*/;

    /**
     * 加载这批数据对应的人人好友个数
     * 
     * @param goodss
     */
    private void loadRenrenFriendCount(Map<Long, GoodsMeta> goodss) {
        List<Integer> ids = new ArrayList<Integer>();
        for (Map.Entry<Long, GoodsMeta> entry : goodss.entrySet()) {
            ids.add(new Long(entry.getValue().getOwnerId()).intValue());
        }
        Map<Integer, Integer> id2fs = isTest ? new HashMap<Integer, Integer>()
                : BuddyByIdCacheAdapter.getInstance().getFriendCounts(ids);
        this.id2fs = id2fs;
    }

    /**
     * 加载商品排序的参数
     */
    private void loadGoodsScoreParam() {
        long beg = System.currentTimeMillis();
        LOG.info("loading goods score parameter ...");
        goodsScoreParam.clear();

        List<GoodsScoreParam> params = datastore.createQuery(GoodsScoreParam.class).asList();
        for (GoodsScoreParam param : params) {
            goodsScoreParam.put(param.getType(), param);
        }

        long time = System.currentTimeMillis() - beg;
        LOG.info("loaded  goods score parameter, elapsed " + time + " ms!");
    }

    /**
     * 参数可配置，配置排序的各个参数
     */
    /*
     * private void loadAvenueSortParameter() { long beg =
     * System.currentTimeMillis(); LOG.info("loading sort parameter ...");
     * Query<AvenueSortParameter> query =
     * datastore.createQuery(AvenueSortParameter.class); AvenueSortParameter
     * avenueSortParameter = query.get(); this.avenueSortParameter =
     * avenueSortParameter; long time = System.currentTimeMillis() - beg;
     * LOG.info("loaded  sort parameter, elapsed " + time + " ms!"); }
     */

    /**
     * 由于还没有建立统一的用户白名单机制，暂时用话题的白名单代替。后续用真正的用户名单替换
     */
    private void loadCredibleUsers() {
        long beg = System.currentTimeMillis();
        LOG.info("loading credible users ...");
        Query<TopicList> query = datastore.createQuery(TopicList.class);
        List<TopicList> list = query.asList();
        if (list == null || list.size() == 0) {
            LOG.info("loaded 0 credible Users ");
            return;
        }
        Map<Long, String> credibleUsersLocal = new HashMap<Long, String>();
        Map<Long, String> uncredibleUsersLocal = new HashMap<Long, String>();
        for (TopicList topicList : list) {
            if (topicList.getType() == 1) {
                credibleUsersLocal.put(topicList.getUserId(), topicList.getGrade());
            } else if (topicList.getType() == -1 && "GRADE_FIVE".equals(topicList.getGrade())) {
                uncredibleUsersLocal.put(topicList.getUserId(), topicList.getGrade());
            }
        }
        this.credibleUsers = credibleUsersLocal;
        this.uncredibleUsers = uncredibleUsersLocal;
        long time = System.currentTimeMillis() - beg;
        LOG.info("loaded " + credibleUsers.size() + " credible  over , elapsed " + time + " ms!");
    }
 
    private void loadSpammer() {
        long beg = System.currentTimeMillis();
        LOG.info("loading spammers ...");
        avaliableSpammers = new HashMap<Long, Spammer>();
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
    }

    private void loadTestGoods(List<Long> goodsIds) {
        avaliableGoods = new HashMap<Long, GoodsMeta>();
        List<Goods> allGoods = datastore
                .find(Goods.class)
                .field("avenueSts")
                .greaterThanOrEq(Constant.AVENUE_STS_NORMAL)
                .retrievedFields(true, "goodsId", "link", "likeCount", "ownerId", "shareCount",
                        "tagList", "categoryId", "avenueSts", "source", "ownerPic", "userLikeList", "name", "desc", "price")
                .field("goodsId").hasAnyOf(goodsIds).order("-goodsId").limit(GOODS_MAX_SIZE)
                .asList();
        if (goodsFilters != null) {
        	for(GoodsCandidateFilter goodsFilter: this.goodsFilters){
        		 allGoods = goodsFilter.filter(allGoods,this);
        	}
           
        } 
        processGoods(allGoods);
    }
    
    private void loadSubtags(){
        LOG.info("loading subtags data ...");
        long beg = System.currentTimeMillis();
        subtags = datastore.createQuery(Subtags.class).asList();
        LOG.info("loaded " + subtags.size() + " subtags over, elapsed " + (System.currentTimeMillis()-beg)+" ms.");
    }
    private void loadGoods() {
        LOG.info("loading goods data ...");
        long beg = System.currentTimeMillis();
        avaliableGoods = new HashMap<Long, GoodsMeta>();
//        List<Goods> allGoods = datastore
//                .find(Goods.class)
//                .field("avenueSts")
//                .greaterThanOrEq(Constant.AVENUE_STS_NORMAL)
//                .retrievedFields(true, "goodsId", "link", "likeCount", "ownerId", "shareCount",
//                        "tagList", "categoryId", "avenueSts", "source", "ownerPic", "userLikeList", "name", "desc", "price")
//                .order("-goodsId").limit(GOODS_MAX_SIZE).asList();

        List<Goods> allGoods = new ArrayList<Goods>();
        allGoods = datastore.find(Goods.class).field("avenueSts").greaterThanOrEq(Constant.AVENUE_STS_NORMAL)
                     .retrievedFields(true, "goodsId", "link", "likeCount", "ownerId", "shareCount",
                    "tagList", "categoryId", "avenueSts", "source", "ownerPic", "userLikeList", "name", "desc", "price")
                     .order("-goodsId").limit(10000).asList();
        while (allGoods.size() < GOODS_MAX_SIZE) {
            LOG.info("get " + allGoods.size() + " goods from database.");
            try {
                List<Goods> part = datastore.find(Goods.class).field("avenueSts").greaterThanOrEq(Constant.AVENUE_STS_NORMAL)
                             .field("goodsId").lessThan(allGoods.get(allGoods.size()-1).getGoodsId())
                             .retrievedFields(true, "goodsId", "link", "likeCount", "ownerId", "shareCount",
                            "tagList", "categoryId", "avenueSts", "source", "ownerPic", "userLikeList", "name", "desc", "price")
                             .order("-goodsId").limit(10000).asList();
                allGoods.addAll(part);
            } catch (Exception e) {
                LOG.error("get goods from database error: " + e.getMessage());
            }
            if (System.currentTimeMillis() - beg > GET_GOODS_MAX_TIME) {
                break;
            }
        }
        long time = System.currentTimeMillis() - beg;
        LOG.info("get " + allGoods.size() + " goods from database over , elapsed " + time + " ms!");
        if (goodsFilters != null) {
        	for(GoodsCandidateFilter goodsFilter: this.goodsFilters){
        		 allGoods = goodsFilter.filter(allGoods,this);
        	}
        }  
        processGoods(allGoods);
        time = System.currentTimeMillis() - beg;
        LOG.info("loaded " + avaliableGoods.size() + " goods over , elapsed " + time + " ms!");
    }

    private void processGoods(List<Goods> allGoods) {
        Collections.sort(allGoods, new LinkComparator());
        String last = "";
        String link = last;
        int like = 0;
        int total = 0;
        Goods gg = null;
        for (Goods g : allGoods) {
            link = g.getLink();
            if (last.compareTo(link) == 0) {
                if (g.getLikeCount() > like) {
                    like = g.getLikeCount();
                    gg = g;
                    total++;
                }
            } else {
                if (last.length() > 0) {
                    GoodsMeta meta = new GoodsMeta(total, gg.getAvenueSts(), gg.getGoodsId(),
                            isDefaultPic(gg.getOwnerPic()), gg.getShareCount(), like,
                            gg.getOwnerId(), gg.getCategoryId(), gg.getSource(), last, gg.getPrice(),
                            gg.getTagList(), gg.getUserLikeList());
                    avaliableGoods.put(gg.getGoodsId(), meta);
                }
                like = g.getLikeCount();
                gg = g;
                total++;
            }
            last = link;
        }
        GoodsMeta meta = new GoodsMeta(total, gg.getAvenueSts(), gg.getGoodsId(),
                isDefaultPic(gg.getOwnerPic()), gg.getShareCount(), like, gg.getOwnerId(),
                gg.getCategoryId(), gg.getSource(), last, gg.getPrice(), gg.getTagList(), gg.getUserLikeList());
        avaliableGoods.put(gg.getGoodsId(), meta);
    }

    private void loadReputations() {
        long beg = System.currentTimeMillis();
        // avaliableReputations =
        // builder.getReputations();//以前的需要从文件加载。本次的通过数据库加载
        List<SimpleUserReputation> reputations = datastore.find(SimpleUserReputation.class)
                .retrievedFields(true, "userId", "score").asList();
        Map<Long, SimpleUserReputation> map = new HashMap<Long, SimpleUserReputation>();
        for (SimpleUserReputation reputation : reputations) {
            map.put(reputation.getUserId(), reputation);
        }
        this.avaliableReputations = map;
        long time = System.currentTimeMillis() - beg;
        LOG.info("loaded " + avaliableReputations.size() + " reputations over , elapsed " + time
                + " ms");
    }

    /**
     * 加载逛街达人数据
     */
    private void loadStarUsers() {
        long beginTime = System.currentTimeMillis();

        List<VUser> stars = datastore.find(VUser.class).retrievedFields(true, "userId").asList();
        if (stars != null && stars.size() > 0) {
            starUsers.clear();
            for (VUser user : stars) {
                starUsers.add(user.getUserId());
            }
            LOG.info("Loaded " + starUsers.size() + " star users, elapse "
                    + (System.currentTimeMillis() - beginTime) + " ms.");
        } else {
            LOG.warn("Fail to load star users from DB.");
        }
    }

    public void assemble() {
        // loadAvenueSortParameter();
        loadCredibleUsers();
        loadSpammer();
        loadSubtags();
        loadSpamLikeUsers();
        loadOperationUsers();
        if (!isTest) {
            loadGoods();
        } else {
            loadTestGoods(testGoodsIds);
        }
        loadReputations();
        loadRenrenFriendCount(this.avaliableGoods);

        loadGoodsScoreParam();
        loadStarUsers();
        loadHiddenGoods();
    }
    private void loadHiddenGoods(){
    	long beginTime=System.currentTimeMillis();
    	List<GoodsHidden> list=this.datastore.createQuery(GoodsHidden.class).retrievedFields(true, "goodsId").limit(80000).asList();
    	for(GoodsHidden goodsHidden:list){
    		this.goodsHidden.add(goodsHidden.getGoodsId());
    	}
    	 LOG.info("Loaded " + spamLikeUsers.size() + " goods hidden, elapse "
                 + (System.currentTimeMillis() - beginTime) + " ms.");
    }
    private void loadSpamLikeUsers(){
    	long beginTime=System.currentTimeMillis();
    	CommonList list=this.datastore.createQuery(CommonList.class).filter("name", "SPAM_LIKE").filter("type", 0).get();
    	spamLikeUsers=list.getUserIds();
    	 LOG.info("Loaded " + spamLikeUsers.size() + " spam like users, elapse "
                 + (System.currentTimeMillis() - beginTime) + " ms.");
    }
    private void loadOperationUsers(){
    	long beginTime=System.currentTimeMillis();
    	CommonList list=this.datastore.createQuery(CommonList.class).filter("name", "OPERATION_USER").filter("type", 0).get();
    	this.operationUsers=list.getUserIds();
    	 LOG.info("Loaded " + operationUsers.size() + " operation users, elapse "
                 + (System.currentTimeMillis() - beginTime) + " ms.");
    }
    public Datastore getDatastore() {
        return datastore;
    }

    public void setDatastore(Datastore datastore) {
        this.datastore = datastore;
    }

    public Map<Long, GoodsMeta> getAvaliableGoods() {
        return avaliableGoods;
    }

    public Map<Long, Spammer> getAvaliableSpammers() {
        return avaliableSpammers;
    }

    public Map<Long, SimpleUserReputation> getAvaliableReputations() {
        return avaliableReputations;
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
            return link1.compareTo(link2);
        }
    }

    public Map<Long, String> getCredibleUsers() {
        return credibleUsers;
    }

    public Map<Long, String> getUncredibleUsers() {
        return uncredibleUsers;
    }

    /*
     * public AvenueSortParameter getAvenueSortParameter() { return
     * avenueSortParameter; }
     */

    public Set<Long> getGoodsHidden() {
		return goodsHidden;
	}

	public Map<Integer, Integer> getId2fs() {
        return id2fs;
    }

    private boolean isDefaultPic(String pic) {
        return pic == null || pic.endsWith("men_main.gif") || pic.endsWith("men_tiny.gif");
    }

    public Map<IndexType, GoodsScoreParam> getGoodsScoreParam() {
        return goodsScoreParam;
    }

    public Set<Long> getStarUsers() {
        return starUsers;
    }

	public void setGoodsFilters(List<GoodsCandidateFilter> goodsFilters) {
		this.goodsFilters = goodsFilters;
	}

    public void setSubtags(List<Subtags> subtags) {
        this.subtags = subtags;
    }

    public List<Subtags> getSubtags() {
        return subtags;
    }

	public Set<Long> getOperationUsers() {
		return operationUsers;
	}



}
