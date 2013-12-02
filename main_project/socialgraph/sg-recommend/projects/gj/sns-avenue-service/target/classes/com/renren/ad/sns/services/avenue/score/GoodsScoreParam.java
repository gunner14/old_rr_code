package com.renren.ad.sns.services.avenue.score;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.services.avenue.protocol.IndexType;
import com.renren.ad.sns.services.avenue.util.GJDataStore;

public class GoodsScoreParam {

	/* */
	IndexType type;

	/* 布尔值的阈值，此时x=0,y=0; x=1,y~=1 */
	public static final double BOOL_THRESHOLD = 0.01;

	/* 商品属性的权重、所有者属性的权重 */
	private int wGoodsPublishTime = -10; // 发布时间

	private int wStarUserGoodsPublishTime = -5; // 达人发布时间

	private int wStarUserAdditionalPublishTime = 10;// 对于达人3天内发布的商品的额外加分

	private int goodsPublishTimeOut3day = -5; // 3天之外发布商品的统一降权
	
	private int goodsPublishTimeOut5day = -15; // 5天之外发布商品的统一降权
	
	private int goodsPublishTimeOut7day = -30; // 7天之外发布商品的统一降权
	
	private int hiddenGoodsWeigh = -30; // 隐藏商品降权

	private int wGoodsNumLike = 50; // 真实喜欢数

	public int getHiddenGoodsWeigh() {
		return hiddenGoodsWeigh;
	}

	public void setHiddenGoodsWeigh(int hiddenGoodsWeigh) {
		this.hiddenGoodsWeigh = hiddenGoodsWeigh;
	}

	private int zeroLikeWeight = -10; // 零喜欢降权

	private int wGoodsNumShare = 10; // 转发数

	private int wGoodsTaobaoSrc = -50; // 淘宝来源

	private int wUserIsSpam = -50; // 是否为spammer

	private int wUserDefaultHead = -40; // 默认头像

	private int wUserCredible = 10; // 可信度

	private int wUserReputation = 10; // 信誉值

	private int wUserNumFriend = 5; // 人人好友数

	private int wUserIsStar = 2; // 是否为逛街达人

	/* 属性取值的显著区分阈值，即映射后取值0.9对应的值 */
	public static final double THREE_DAYS = 3 * 24 * 60; // 分钟级别，3天内
	public static final double FIVE_DAYS = 5 * 24 * 60; // 分钟级别，5天内
	public static final double SERVEN_DAYS = 7 * 24 * 60; // 分钟级别，7天内

	private double tGoodsPublishTime = THREE_DAYS; // 分钟级别，3天内

	private double tStarUserGoodsPublishTime = THREE_DAYS;

	private double tStarUserAdditionalPublishTime = THREE_DAYS;

	private double tGoodsNumLike = 100;

	private double tGoodsNumShare = 100;

	private double tGoodsTaobaoSrc = BOOL_THRESHOLD;

	private double tUserIsSpam = BOOL_THRESHOLD;

	private double tUserDefaultHead = BOOL_THRESHOLD;

	private double tUserCredible = BOOL_THRESHOLD;

	private double tUserReputation = 100;

	private double tUserNumFriend = 200;

	private double tUserIsStar = BOOL_THRESHOLD;

	/* 计算真实喜欢数 */
	private double realLikeFirstWeigth = 0.1;

	private double realLikeSecondWeigth = 0.2;

	private double realLikeThirdWeigth = 0.4;

	private double realLikeFourthWeigth = 0.6;

	private double realLikeFifthWeigth = 0.8;

	/* 同一个用户多个商品的打散 */
	private double repeatGoodsFromSameUserWeight = 0.0000075; // 一个用户分享多个商品，每个商品会逐渐减权

	/* 零排序参数 */
	private boolean openZeroSort = true; // 是否启用零排序

	private int zeroSortCount = 20; // 零排序位置

	private double zeroWeigth = 0.000075; // 对零喜欢的降权

	private double zeroDecrementWeight = 0.000003; // 每个零排序递减的权重

	private double zeroCorrectionWeigh = 0.0001; // 零排序的修正值

	/* 其它的排序参数 */
	private double crediblePoint = 0.0000004; // 在白名单中的用户加权

	private double defaultRealLikeCount = 1; // 如果没有在Reputation中的默认实际喜欢数

	private int defaultFriendCount = 15; // 如果小于15个好友，则降权

	private double defaultReputationValue = 0.2; // 如果reputation表中没有数据的默认信誉值
	
	private int isSupportOperationUser=0;//0表示不支持，1表示支持
	
	private double operationUserScore=50;//运营编辑的用户+分

	private List<Long> whiteList = Arrays.asList(378811860L, 385958442L,
			465138349L, 465141034L, 465125711L, 465134973L, 242353786L,
			465142907L, 465139052L, 465124996L, 465208455L, 466339202L,
			465225566L, 465139715L, 465133164L, 465214582L, 465932344L,
			466058140L, 297349927L, 246794585L, 467285462L, 467281247L,
			467285943L, 467303900L, 467304681L);

	/**
	 * 用于最新商品排序的参数
	 * 
	 * @return
	 */
	public static GoodsScoreParam getNewestParam() {
		GoodsScoreParam param = new GoodsScoreParam();
		param.setType(IndexType.NEWEST);
		param.setwGoodsPublishTime(-30);
		return param;
	}

	/**
	 * 用于最热商品排序的参数
	 * 
	 * @return
	 */
	public static GoodsScoreParam getHottestParam() {
		GoodsScoreParam param = new GoodsScoreParam();
		param.setType(IndexType.HOTTEST);
		param.setwGoodsPublishTime(-5);
		return param;
	}

	public int getwGoodsPublishTime() {
		return wGoodsPublishTime;
	}

	public void setwGoodsPublishTime(int wGoodsPublishTime) {
		this.wGoodsPublishTime = wGoodsPublishTime;
	}

	public int getwGoodsNumLike() {
		return wGoodsNumLike;
	}

	public void setwGoodsNumLike(int wGoodsNumLike) {
		this.wGoodsNumLike = wGoodsNumLike;
	}

	public int getwGoodsNumShare() {
		return wGoodsNumShare;
	}

	public void setwGoodsNumShare(int wGoodsNumShare) {
		this.wGoodsNumShare = wGoodsNumShare;
	}

	public int getwGoodsTaobaoSrc() {
		return wGoodsTaobaoSrc;
	}

	public void setwGoodsTaobaoSrc(int wGoodsTaobaoSrc) {
		this.wGoodsTaobaoSrc = wGoodsTaobaoSrc;
	}

	public int getwUserIsSpam() {
		return wUserIsSpam;
	}

	public void setwUserIsSpam(int wUserIsSpam) {
		this.wUserIsSpam = wUserIsSpam;
	}

	public int getwUserDefaultHead() {
		return wUserDefaultHead;
	}

	public void setwUserDefaultHead(int wUserDefaultHead) {
		this.wUserDefaultHead = wUserDefaultHead;
	}

	public int getwUserCredible() {
		return wUserCredible;
	}

	public void setwUserCredible(int wUserCredible) {
		this.wUserCredible = wUserCredible;
	}

	public int getwUserReputation() {
		return wUserReputation;
	}

	public void setwUserReputation(int wUserReputation) {
		this.wUserReputation = wUserReputation;
	}

	public int getwUserNumFriend() {
		return wUserNumFriend;
	}

	public void setwUserNumFriend(int wUserNumFriend) {
		this.wUserNumFriend = wUserNumFriend;
	}

	public double gettGoodsPublishTime() {
		return tGoodsPublishTime;
	}

	public void settGoodsPublishTime(double tGoodsPublishTime) {
		this.tGoodsPublishTime = tGoodsPublishTime;
	}

	public double gettGoodsNumLike() {
		return tGoodsNumLike;
	}

	public void settGoodsNumLike(double tGoodsNumLike) {
		this.tGoodsNumLike = tGoodsNumLike;
	}

	public double gettGoodsNumShare() {
		return tGoodsNumShare;
	}

	public void settGoodsNumShare(double tGoodsNumShare) {
		this.tGoodsNumShare = tGoodsNumShare;
	}

	public double gettGoodsTaobaoSrc() {
		return tGoodsTaobaoSrc;
	}

	public void settGoodsTaobaoSrc(double tGoodsTaobaoSrc) {
		this.tGoodsTaobaoSrc = tGoodsTaobaoSrc;
	}

	public double gettUserIsSpam() {
		return tUserIsSpam;
	}

	public void settUserIsSpam(double tUserIsSpam) {
		this.tUserIsSpam = tUserIsSpam;
	}

	public double gettUserDefaultHead() {
		return tUserDefaultHead;
	}

	public void settUserDefaultHead(double tUserDefaultHead) {
		this.tUserDefaultHead = tUserDefaultHead;
	}

	public double gettUserCredible() {
		return tUserCredible;
	}

	public void settUserCredible(double tUserCredible) {
		this.tUserCredible = tUserCredible;
	}

	public double gettUserReputation() {
		return tUserReputation;
	}

	public void settUserReputation(double tUserReputation) {
		this.tUserReputation = tUserReputation;
	}

	public double gettUserNumFriend() {
		return tUserNumFriend;
	}

	public void settUserNumFriend(double tUserNumFriend) {
		this.tUserNumFriend = tUserNumFriend;
	}

	public int getwUserIsStar() {
		return wUserIsStar;
	}

	public void setwUserIsStar(int wUserIsStar) {
		this.wUserIsStar = wUserIsStar;
	}

	public double gettUserIsStar() {
		return tUserIsStar;
	}

	public void settUserIsStar(double tUserIsStar) {
		this.tUserIsStar = tUserIsStar;
	}

	public double getRealLikeFirstWeigth() {
		return realLikeFirstWeigth;
	}

	public void setRealLikeFirstWeigth(double realLikeFirstWeigth) {
		this.realLikeFirstWeigth = realLikeFirstWeigth;
	}

	public double getRealLikeSecondWeigth() {
		return realLikeSecondWeigth;
	}

	public void setRealLikeSecondWeigth(double realLikeSecondWeigth) {
		this.realLikeSecondWeigth = realLikeSecondWeigth;
	}

	public double getRealLikeThirdWeigth() {
		return realLikeThirdWeigth;
	}

	public void setRealLikeThirdWeigth(double realLikeThirdWeigth) {
		this.realLikeThirdWeigth = realLikeThirdWeigth;
	}

	public double getRealLikeFourthWeigth() {
		return realLikeFourthWeigth;
	}

	public void setRealLikeFourthWeigth(double realLikeFourthWeigth) {
		this.realLikeFourthWeigth = realLikeFourthWeigth;
	}

	public double getRealLikeFifthWeigth() {
		return realLikeFifthWeigth;
	}

	public void setRealLikeFifthWeigth(double realLikeFifthWeigth) {
		this.realLikeFifthWeigth = realLikeFifthWeigth;
	}

	public double getRepeatGoodsFromSameUserWeight() {
		return repeatGoodsFromSameUserWeight;
	}

	public void setRepeatGoodsFromSameUserWeight(
			double repeatGoodsFromSameUserWeight) {
		this.repeatGoodsFromSameUserWeight = repeatGoodsFromSameUserWeight;
	}

	public boolean isOpenZeroSort() {
		return openZeroSort;
	}

	public void setOpenZeroSort(boolean openZeroSort) {
		this.openZeroSort = openZeroSort;
	}

	public int getZeroSortCount() {
		return zeroSortCount;
	}

	public void setZeroSortCount(int zeroSortCount) {
		this.zeroSortCount = zeroSortCount;
	}

	public double getZeroWeigth() {
		return zeroWeigth;
	}

	public void setZeroWeigth(double zeroWeigth) {
		this.zeroWeigth = zeroWeigth;
	}

	public double getZeroDecrementWeight() {
		return zeroDecrementWeight;
	}

	public void setZeroDecrementWeight(double zeroDecrementWeight) {
		this.zeroDecrementWeight = zeroDecrementWeight;
	}

	public double getZeroCorrectionWeigh() {
		return zeroCorrectionWeigh;
	}

	public void setZeroCorrectionWeigh(double zeroCorrectionWeigh) {
		this.zeroCorrectionWeigh = zeroCorrectionWeigh;
	}

	public double getCrediblePoint() {
		return crediblePoint;
	}

	public void setCrediblePoint(double crediblePoint) {
		this.crediblePoint = crediblePoint;
	}

	public double getDefaultRealLikeCount() {
		return defaultRealLikeCount;
	}

	public void setDefaultRealLikeCount(double defaultRealLikeCount) {
		this.defaultRealLikeCount = defaultRealLikeCount;
	}

	public int getDefaultFriendCount() {
		return defaultFriendCount;
	}

	public void setDefaultFriendCount(int defaultFriendCount) {
		this.defaultFriendCount = defaultFriendCount;
	}

	public double getDefaultReputationValue() {
		return defaultReputationValue;
	}

	public void setDefaultReputationValue(double defaultReputationValue) {
		this.defaultReputationValue = defaultReputationValue;
	}

	public int getwStarUserAdditionalPublishTime() {
		return wStarUserAdditionalPublishTime;
	}

	public void setwStarUserAdditionalPublishTime(
			int wStarUserAdditionalPublishTime) {
		this.wStarUserAdditionalPublishTime = wStarUserAdditionalPublishTime;
	}

	public double gettStarUserAdditionalPublishTime() {
		return tStarUserAdditionalPublishTime;
	}

	public void settStarUserAdditionalPublishTime(
			double tStarUserAdditionalPublishTime) {
		this.tStarUserAdditionalPublishTime = tStarUserAdditionalPublishTime;
	}

	public int getwStarUserGoodsPublishTime() {
		return wStarUserGoodsPublishTime;
	}

	public void setwStarUserGoodsPublishTime(int wStarUserGoodsPublishTime) {
		this.wStarUserGoodsPublishTime = wStarUserGoodsPublishTime;
	}

	public double gettStarUserGoodsPublishTime() {
		return tStarUserGoodsPublishTime;
	}

	public void settStarUserGoodsPublishTime(double tStarUserGoodsPublishTime) {
		this.tStarUserGoodsPublishTime = tStarUserGoodsPublishTime;
	}

	public IndexType getType() {
		return type;
	}

	public void setType(IndexType type) {
		this.type = type;
	}

	public List<Long> getWhiteList() {
		return whiteList;
	}

	public void setWhiteList(List<Long> whiteList) {
		this.whiteList = whiteList;
	}

	public int getZeroLikeWeight() {
		return zeroLikeWeight;
	}

	public void setZeroLikeWeight(int zeroLikeWeight) {
		this.zeroLikeWeight = zeroLikeWeight;
	}

	public int getGoodsPublishTimeOut3day() {
		return goodsPublishTimeOut3day;
	}

	public void setGoodsPublishTimeOut3day(int goodsPublishTimeOut3day) {
		this.goodsPublishTimeOut3day = goodsPublishTimeOut3day;
	}

	public int getGoodsPublishTimeOut5day() {
		return goodsPublishTimeOut5day;
	}

	public void setGoodsPublishTimeOut5day(int goodsPublishTimeOut5day) {
		this.goodsPublishTimeOut5day = goodsPublishTimeOut5day;
	}

	public int getGoodsPublishTimeOut7day() {
		return goodsPublishTimeOut7day;
	}

	public void setGoodsPublishTimeOut7day(int goodsPublishTimeOut7day) {
		this.goodsPublishTimeOut7day = goodsPublishTimeOut7day;
	}

	public int getIsSupportOperationUser() {
		return isSupportOperationUser;
	}

	public void setIsSupportOperationUser(int isSupportOperationUser) {
		this.isSupportOperationUser = isSupportOperationUser;
	}

	public double getOperationUserScore() {
		return operationUserScore;
	}

	public void setOperationUserScore(double operationUserScore) {
		this.operationUserScore = operationUserScore;
	}

	public static void main(String[] args) {
		Datastore ds = GJDataStore.getDefaultDataStore();
		ds.save(GoodsScoreParam.getHottestParam());
		ds.save(GoodsScoreParam.getNewestParam());
	}
}
