package com.renren.ad.sns.services.avenue;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.data.SimpleUserReputation;
import com.renren.ad.sns.data.Subtags;
import com.renren.ad.sns.services.avenue.AvenueIndex.GoodsData;
import com.renren.ad.sns.services.avenue.protocol.IndexType;
import com.renren.ad.sns.services.avenue.score.DefaultGoodsScorer;
import com.renren.ad.sns.services.avenue.score.GoodsScoreParam;
import com.renren.ad.util.container.Tuple;
import com.xiaonei.xce.buddyadapter.buddybyidcache.BuddyByIdCacheAdapter;

/**
 * simple memory index builder for avenue , steps are follow : <br/>
 * <ul>
 * <li>1. load related data [goods , spammer , reputation]</li><br/>
 * <li>2. compute reputation</li>
 * </ul>
 * 
 * @author xiong.hua refactored by xiang.zhang
 */
public class AvenueBuilder {

	private DataHammer hammer;

	private DefaultGoodsScorer scorer;

	private Map<Long, Double> reputations;

	private List<Tuple<GoodsMeta, Double>> candidates;

	private GoodsScoreParam sortParameter;

	private Map<IndexType, GoodsScoreParam> sortParameterMap;

	private static final Log LOG = LogFactory.getLog(AvenueBuilder.class);

	/**
	 * Build the full index
	 * 
	 * @return
	 * @throws Exception
	 */
	public Map<IndexType, AvenueIndex> createIndex() throws Exception {

		long start = System.currentTimeMillis();
		Map<IndexType, AvenueIndex> result = new HashMap<IndexType, AvenueIndex>();


		// 1.
		// 从DB中重新加载信息：Goods/Spammer/Reputation/(Un)Credible/numRenRenFriend/VUser/GoodsScoreParameter
		hammer.assemble();

		// 获取不同索引类型对应的排序参数
		sortParameterMap = hammer.getGoodsScoreParam();
		long version=System.currentTimeMillis();
		for (IndexType idxType : sortParameterMap.keySet()) {
			LOG.info("start creating " + idxType + " index ...");

			sortParameter = sortParameterMap.get(idxType);

			// 2. 信誉值的归一化, 设置商品的可信度
			caculateReputations(hammer.getAvaliableReputations());
			setCredible();

			// 3. 计算真实喜欢数
			caculateRealLikeCountAndReputation(hammer.getAvaliableGoods());

			// 4. 商品初排
			sortGoods(hammer.getAvaliableGoods());

			// 5. 同一个用户多个商品的打散
			reorder();

			// 6. 零喜欢商品的打散
			if (sortParameter.isOpenZeroSort()) {
				zeroSort();
			}

			if (LOG.isDebugEnabled()) {
				int i = 0;
				for (Tuple<GoodsMeta, Double> tuple : candidates) {
					LOG.debug("score:" + tuple.left.getGoodsId() + ","
							+ tuple.right + "," + i);
					i++;
				}
			}

			// 7. 建立索引
			AvenueIndex index = doCreateIndex(version);
			result.put(idxType, index);
			LOG.info("finished creating " + idxType + " index, using "
					+ (System.currentTimeMillis() - start) / 1000 + " s");
		}

		return result;
	}

	/**
	 * 设置用户的可信等级
	 * 
	 * @param goodss
	 * @param credibleUsers
	 *            可信名单
	 * @param uncredibleUsers
	 *            不可信名单
	 */
	private void setCredible() {
		Map<Long, GoodsMeta> goodss = hammer.getAvaliableGoods();
		Map<Long, String> credibleUsers = hammer.getCredibleUsers();
		Map<Long, String> uncredibleUsers = hammer.getUncredibleUsers();

		Iterator<GoodsMeta> it = goodss.values().iterator();
		while (it.hasNext()) {
			GoodsMeta goodsMeta = it.next();
			if (credibleUsers.containsKey(goodsMeta.getOwnerId())) {
				goodsMeta.setCredible(1);
			}
			if (uncredibleUsers.containsKey(goodsMeta.getOwnerId())) {
				goodsMeta.setCredible(-1);
			}
		}

	}

	/**
	 * calculate real like count
	 * 
	 * @param goodss
	 * @param repus
	 */
	private void caculateRealLikeCountAndReputation(Map<Long, GoodsMeta> goodss) {
	    /* 用户信誉度划分界限 */
	    final double firstMargin = 15.001;
	    final double secondMargin = 15.1;
	    final double thirdMargin = 15.47;
	    final double fourthMargin = 17;
	    final double fifthMargin = 40;
	    
		Iterator<Entry<Long, GoodsMeta>> it = goodss.entrySet().iterator();
		while (it.hasNext()) {
			Entry<Long, GoodsMeta> entry = it.next();
			GoodsMeta goodsMeta = entry.getValue();
			List<Long> likeUserList = goodsMeta.getUserLikeList();
			if (likeUserList == null) {
				setReputation(goodsMeta);
				continue;
			}
			List<Integer> ids = new ArrayList<Integer>();
			for (Long userId : likeUserList) {
				ids.add(userId.intValue());
			}
			Map<Integer, Integer> id2friendCount = this.hammer.isTest == true ? new HashMap<Integer, Integer>()
					: BuddyByIdCacheAdapter.getInstance().getFriendCounts(ids);
			LOG.debug("friend count is " + id2friendCount.size() + " "
					+ goodsMeta.getGoodsId());

			double value = 0;
			double starWeight = 1.0;
			Set<Long> spamLikeUsers=this.hammer.getSpamLikeUsers();
			for (Long userId : likeUserList) {
				double score = sortParameter.getDefaultRealLikeCount();
				if (id2friendCount.containsKey(userId.intValue())) {
					int count = id2friendCount.get(userId.intValue());
					if (count > sortParameter.getDefaultFriendCount()) {
						score = 0.9;
					}
					LOG.debug("friend count " + goodsMeta.getGoodsId() + " "
							+ count + " " + userId);
				}

				double reputationScore = 0;
				if (reputations.containsKey(userId)) {
					reputationScore = reputations.get(userId) * 100;
				}else{
				    reputationScore = sortParameter.getDefaultReputationValue();
				}
				
				if (reputationScore < firstMargin) {
                    score = sortParameter.getRealLikeFirstWeigth();
                } else if (reputationScore >= firstMargin && reputationScore < secondMargin) {
                    score = sortParameter.getRealLikeSecondWeigth();
                } else if (reputationScore >= secondMargin && reputationScore < thirdMargin) {
                    score = sortParameter.getRealLikeThirdWeigth();
                } else if (reputationScore >= thirdMargin && reputationScore < fourthMargin) {
                    score = sortParameter.getRealLikeFourthWeigth();
                } else if (reputationScore >= fourthMargin && reputationScore < fifthMargin) {
                    score = sortParameter.getRealLikeFifthWeigth();
                } else {
                    score = 1;
                }
				if(spamLikeUsers.contains(userId)){
					score=0;//刷喜欢用户
				}
				if (score < 1) {
					LOG.debug(userId + " " + score + " " + likeUserList.size()
							+ " " + goodsMeta.getGoodsId());
				}

				// 若白名单用户喜欢该商品，则真实喜欢数加倍
				if (sortParameter.getWhiteList() != null
						&& sortParameter.getWhiteList().contains(userId)) {
					starWeight = 2.0;
				} else {
					starWeight = 1.0;
				}

				value += score * starWeight;
			}
			int realLikeCount = (int) Math.rint(value);
			// System.out.println("recaculate real like count. the goodsId is "+goodsMeta.getGoodsId()+" .the orig like count is "+
			// goodsMeta.getLikeCount()+" the real like count is "+realLikeCount);
			if (goodsMeta.getLikeCount() > realLikeCount) {
				LOG.debug("recaculate real like count. the goodsId is "
						+ goodsMeta.getGoodsId() + " .the orig like count is "
						+ goodsMeta.getLikeCount() + " the real like count is "
						+ realLikeCount);
			}

			goodsMeta.setRealLikeCount(realLikeCount);
			setReputation(goodsMeta);
		}
	}

	private void setReputation(GoodsMeta goodsMeta) {
		if (this.reputations.containsKey(goodsMeta.getOwnerId())) {
			double score = this.reputations.get(goodsMeta.getOwnerId());
			goodsMeta.setReputation(score);
		} else {
			goodsMeta.setReputation(sortParameter.getDefaultReputationValue());
			goodsMeta.setDefaultReputation(true);
		}
	}

	// create index helper method
	protected AvenueIndex doCreateIndex(long version) throws SQLException,
			FileNotFoundException, IOException {
		if (null == candidates || candidates.size() == 0) {
			LOG.error("failed to load goods from hammer");
			return null;
		}

		long start = System.currentTimeMillis();
		LOG.info("start building targeting index in memory");
		AvenueIndex index = new AvenueIndex();
		// ensure index knows number of valid documents
		index.ensureCapacity(candidates.size());
		// add document one by one
		int docId = 0;
		for (Tuple<GoodsMeta, Double> candidate : candidates) {
			GoodsMeta goods = candidate.left;
			if (goods.getGoodsId() == 1206230838013874817l
					|| goods.getGoodsId() == 1206231307495134054l) {
				LOG.info("test end end:" + goods.getGoodsId() + "\t"
						+ candidate.right + ", user parameters is "
						+ sortParameter.toString());
			}
			update(docId, index, candidate.left, candidate.right);
			docId++;
		}
		setSubtagList(index, hammer.getSubtags());
		// compact index
		index.compact();
		long elapse = System.currentTimeMillis() - start;
		index.setVersion(version);
		LOG.info("build index of " + candidates.size() + " goods, takes "
				+ elapse / 1000 + "s" + ", version=" + version);

		return index;
	}

	private void setSubtagList(AvenueIndex index, List<Subtags> subtags) {
	    if (subtags==null) {
	        LOG.error("subtagses is null");
            return;
        }
        for (Subtags s : subtags) {
            if (s == null) {
                LOG.error("subtags is null");
                continue;
            }
            long tagId = s.getTagId();
            TargetingFeature feature = new TargetingFeature(FeatureType.Tag, tagId);
            FeaturePostingList featureList = new FeaturePostingList();
            for (long subtagId : s.getSubTags()) {
                FeaturePostingList list = index.get(new TargetingFeature(FeatureType.Tag, subtagId));
                if (list != null) {
                    featureList.addAll(list);
                }
            }
            // 去重
            List<DocPosting> docs = FeaturePostingList.merge(featureList, null);
            
            featureList.clear();
            featureList.addAll(docs);
            
            LOG.info("feature:" + feature + "; list.size:" + featureList.size());
            index.put(feature, featureList);
        }
    }

    public void update(int docId, AvenueIndex index, GoodsMeta goods,
			double score) {
		GoodsFeatureTokenizer tokenizer = new GoodsFeatureTokenizer(goods);
		GoodsData goodsData = new GoodsData(goods.getGoodsId(), goods
				.getOwnerId(), score);
		while (tokenizer.hasNext()) {
			TargetingFeature feature = tokenizer.nextFeature();
			index.addGoods(docId, goodsData, feature);
		}
	}

	/**
	 * caculate reputations for goods owner
	 * 
	 * @param repus
	 */
	private void caculateReputations(Map<Long, SimpleUserReputation> repus) {
		reputations = new HashMap<Long, Double>();
		for (Map.Entry<Long, SimpleUserReputation> entry : repus.entrySet()) {
			try {
				double repu = entry.getValue().getScore() * 0.01;
				reputations.put(entry.getKey(), repu);
			} catch (Exception e) {
				LOG.error(e, e);
			}
		}
	}

	/**
	 * order the goods meta by score
	 * 
	 * @param goodss
	 */
	/*
	 * private void order(Map<Long, GoodsMeta> goodss) { candidates = new
	 * ArrayList<Tuple<GoodsMeta, Double>>();
	 * 
	 * // init scorer scorer.init(spammers, 10, avenueSortParameter); int
	 * defaultFriendCount = avenueSortParameter.getDefaultFriendCount();
	 * Map<Long, String> uncredibleUsers = hammer.getUncredibleUsers();
	 * Map<Long, Double> delta = new HashMap<Long, Double>(); for
	 * (Map.Entry<Long, GoodsMeta> entry : goodss.entrySet()) { double point =
	 * scorer.score(entry.getValue()); GoodsMeta goods = entry.getValue(); if
	 * (goods.getRealLikeCount() == 0) { point -=
	 * avenueSortParameter.getZeroWeigth(); } // FIXME: shall do it in scorer
	 * int fs = 0; int owner = new
	 * Long(entry.getValue().getOwnerId()).intValue(); if
	 * (id2fs.containsKey(owner)) { fs = id2fs.get(owner); } if (fs <
	 * defaultFriendCount) { point +=
	 * avenueSortParameter.getLessFriendThanValue();
	 * LOG.debug(entry.getValue().getGoodsId() + "\t" + point +
	 * ", user parameters is " + avenueSortParameter.toString()); }
	 * 
	 * Tuple<GoodsMeta, Double> tuple = new Tuple<GoodsMeta, Double>(entry
	 * .getValue(), point); candidates.add(tuple); }
	 * Collections.sort(candidates, this.new DescTupleComparator()); }
	 */

	private void sortGoods(Map<Long, GoodsMeta> goodss) {
		candidates = new ArrayList<Tuple<GoodsMeta, Double>>();
		scorer.setSpammers(hammer.getAvaliableSpammers());
		scorer.setScoreParam(sortParameter);
		scorer.setId2friendCount2(hammer.getId2fs());
		scorer.setStarUsers(hammer.getStarUsers());
		scorer.setGoodsHidden(hammer.getGoodsHidden());
		scorer.setOperationUsers(hammer.getOperationUsers());
		for (Map.Entry<Long, GoodsMeta> entry : goodss.entrySet()) {
			double point = scorer.score(entry.getValue());

			Tuple<GoodsMeta, Double> tuple = new Tuple<GoodsMeta, Double>(entry
					.getValue(), point);
			candidates.add(tuple);
		}
		Collections.sort(candidates, this.new DescTupleComparator());
	}

	/**
	 * reorder by user avoid too many goods from the same user
	 */
	private void reorder() {
		if (candidates != null) {
			double delta = sortParameter.getRepeatGoodsFromSameUserWeight();
			Map<Long, Double> deltas = new HashMap<Long, Double>();
			for (Tuple<GoodsMeta, Double> tuple : candidates) {
				long key = tuple.left.getOwnerId();
				double minus = 0;
				if (deltas.containsKey(key)) {
					minus = deltas.get(key);
				}
				tuple.right += minus;
				deltas.put(key, minus - delta);
				GoodsMeta goods = tuple.left;
			}
			Collections.sort(candidates, this.new DescTupleComparator());
			for (Tuple<GoodsMeta, Double> tuple : candidates) {
				GoodsMeta goods = tuple.left;
			}
		}
	}

	private void zeroSort() {
		List<Tuple<GoodsMeta, Double>> zoreList = new ArrayList<Tuple<GoodsMeta, Double>>();
		List<Integer> indexes = new ArrayList<Integer>();
		int idx = 0;
		// pick zore like candidate
		double top = candidates.size();
		Map<Long, Double> delta = new HashMap<Long, Double>();
		int limit = 200;
		// Map<Long, String> credibleUsers=hammer.getCredibleUsers();
		Map<Long, String> uncredibleUsers = hammer.getUncredibleUsers();
		for (Tuple<GoodsMeta, Double> candidate : candidates) {
			GoodsMeta meta = candidate.left;
			// LOG.info("ALL ===== " + candidate.left.getGoodsId() + "\t" +
			// candidate.right);
			if (meta != null) {
				if (meta.getRealLikeCount() == 0) {
					double point = candidate.right;
					long oid = meta.getOwnerId();
					if (reputations.containsKey(oid)
							&& !uncredibleUsers.containsKey(oid)) {
						double value = 0;
						if (delta.containsKey(oid)) {
							value = delta.get(oid);
						}
						double mdelta = reputations.get(oid)
								* sortParameter.getZeroCorrectionWeigh()
								- value;
						point += mdelta > 0 ? mdelta : 0;
						if (meta.getCredible() == 1) {
							point += sortParameter.getCrediblePoint();
							// point+=avenueSortParameter.getZeroWeigth();
						}
						value += sortParameter
								.getRepeatGoodsFromSameUserWeight();
						delta.put(oid, value);
						LOG.debug(meta.getGoodsId() + " zore sort point "
								+ point + " orig point " + candidate.right);
						candidate.right = point;
					}
					// if( --limit > 0 ) LOG.info("ZERO ===== " +
					// candidate.left.getGoodsId() + "\t" + point);
					zoreList.add(new Tuple<GoodsMeta, Double>(meta, point));
					indexes.add(idx);
				}
			}
			// candidate.right = top - idx;
			idx++;
		}
		// reorder zore like candidate
		Collections.sort(zoreList, this.new DescTupleComparator());

		idx = 0;
		limit = sortParameter.getZeroSortCount();
		double value = 0;
		for (int i = 0; i < zoreList.size(); i++) {
			Tuple<GoodsMeta, Double> zore = zoreList.get(i);
			if (i < indexes.size()) {
				idx = indexes.get(i);
				if (--limit > 0) {
					zore.right += sortParameter.getZeroWeigth();
					zore.right -= value;
					value += sortParameter.getZeroDecrementWeight();
					LOG.debug(zore.left.getGoodsId()
							+ "zore limit  sort point " + zore.right);
					candidates.set(idx, zore);
					// LOG.info("AZERO ===== " + zore.left.getGoodsId() + "\t"
					// + zore.right + "\t" + idx);
				}
			} else {
				LOG.error("Can not happen , but happend ...");
			}
		}
		Collections.sort(candidates, this.new DescTupleComparator());
	}

	public void setHammer(DataHammer hammer) {
		this.hammer = hammer;
	}

	public void setScorer(DefaultGoodsScorer scorer) {
		this.scorer = scorer;
	}

	class DescTupleComparator implements Comparator<Tuple<GoodsMeta, Double>> {

		static final double EPS = 1.0e-10;

		@Override
		public int compare(Tuple<GoodsMeta, Double> t1,
				Tuple<GoodsMeta, Double> t2) {
			double delta = t1.right - t2.right;
			return Math.abs(delta) < EPS ? 0 : delta > 0 ? -1 : 1;
		}
	}

	class AscTupleComparator implements Comparator<Tuple<GoodsMeta, Double>> {

		static final double EPS = 1.0e-10;

		@Override
		public int compare(Tuple<GoodsMeta, Double> t1,
				Tuple<GoodsMeta, Double> t2) {
			double delta = t1.right - t2.right;
			return Math.abs(delta) < EPS ? 0 : delta < 0 ? -1 : 1;
		}
	}

	public DataHammer getHammer() {
		return hammer;
	}

	public static void main(String[] args) {
		List<Tuple<GoodsMeta, Double>> ls = new ArrayList<Tuple<GoodsMeta, Double>>();
		Tuple<GoodsMeta, Double> t1 = new Tuple<GoodsMeta, Double>(null, 3.0);
		Tuple<GoodsMeta, Double> t2 = new Tuple<GoodsMeta, Double>(null, 2.0);
		ls.add(t1);
		ls.add(t2);
		AvenueBuilder builder = new AvenueBuilder();
		Collections.sort(ls, builder.new DescTupleComparator());
		for (Tuple<GoodsMeta, Double> t : ls) {
			System.out.println(t.right);
		}
	}
}
