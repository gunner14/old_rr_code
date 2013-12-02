package com.renren.ad.sns.services.avenue;

import com.google.code.morphia.annotations.Entity;

@Entity
public class AvenueSortParameter {

	//private double likeWeight = 1;// 喜欢的权重
	//private double fwdWeight = 0.05;// 转发的权重
	private double crediblePoint = 0.0004;// 在白名单中的用户加权
	//private double taobaoWeight = -1;// 淘宝分享的商品降权
	//private double defaultOwnerPic = -1;// 没有投降的降权
	//private double uncrediblePoint = -1;// 在黑名单中的用户降权
	//private double lessFriendThanValue = -1;// 好友数下于n值得降权
	private double defaultRealLikeCount = 0.3;// 如果没有在Reputation中的默认实际喜欢数
	private double realLikeCountCorrectionCount = 0.3;// 对于Reputation中的权重值做修正
	private double realLikeFirstWeigth = 0.1;
	private double realLikeSecondWeigth = 0.2;
	private double realLikeThreeWeigth = 0.4;
	private double realLikeForthWeigth = 0.6;
	private double realLikeFiveWeigth = 0.8;
	private double repeatGoodsFromSameUserWeight=5;//一个用户分享多个商品，每个商品会逐渐减权
	private double zeroCorrectionWeigh=0.0001;//0排序的修正值
	private int defaultFriendCount=15;//如果小于15个好友，则将权
	private boolean openZeroSort=false;//是否启用o排序
	//private double reputationCorrectionValue=0.88;//Reputation的修正值，即（原值+修正值）*修正参
	//private double reputationCorrectionParameter=0.005;//Reputation的修正值
	private double defaultReputationValue=0.2;//如果reputation表中没有数据的默认数据
	private boolean openCredibleForNotLikeSort=false;//是否对非0喜欢排序启用白名单加权功能
	private double zeroWeigth=0.001;//对0喜欢的降权
	private int zeroSortCount=20;//0排序位置
	private double zeroDecrementWeight=0.0001;//每个0排序递减的权重
	
	private double timeWeight = 0.0001; //时间权重
	
	

	public double getCrediblePoint() {
		return crediblePoint;
	}

	public double getDefaultReputationValue() {
		return defaultReputationValue;
	}

	public double getDefaultRealLikeCount() {
		return defaultRealLikeCount;
	}

	public void setDefaultRealLikeCount(double defaultRealLikeCount) {
		this.defaultRealLikeCount = defaultRealLikeCount;
	}

	public double getRealLikeCountCorrectionCount() {
		return realLikeCountCorrectionCount;
	}

	public void setRealLikeCountCorrectionCount(
			double realLikeCountCorrectionCount) {
		this.realLikeCountCorrectionCount = realLikeCountCorrectionCount;
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

	public double getRealLikeThreeWeigth() {
		return realLikeThreeWeigth;
	}

	public void setRealLikeThreeWeigth(double realLikeThreeWeigth) {
		this.realLikeThreeWeigth = realLikeThreeWeigth;
	}

	public double getRealLikeForthWeigth() {
		return realLikeForthWeigth;
	}

	public void setRealLikeForthWeigth(double realLikeForthWeigth) {
		this.realLikeForthWeigth = realLikeForthWeigth;
	}

	public double getRealLikeFiveWeigth() {
		return realLikeFiveWeigth;
	}

	public void setRealLikeFiveWeigth(double realLikeFiveWeigth) {
		this.realLikeFiveWeigth = realLikeFiveWeigth;
	}

	public double getRepeatGoodsFromSameUserWeight() {
		return repeatGoodsFromSameUserWeight;
	}

	public void setRepeatGoodsFromSameUserWeight(
			double repeatGoodsFromSameUserWeight) {
		this.repeatGoodsFromSameUserWeight = repeatGoodsFromSameUserWeight;
	}

	public double getZeroCorrectionWeigh() {
		return zeroCorrectionWeigh;
	}

	public void setZeroCorrectionWeigh(double zeroCorrectionWeigh) {
		this.zeroCorrectionWeigh = zeroCorrectionWeigh;
	}

	public int getDefaultFriendCount() {
		return defaultFriendCount;
	}

	public void setDefaultFriendCount(int defaultFriendCount) {
		this.defaultFriendCount = defaultFriendCount;
	}

	public boolean isOpenZeroSort() {
		return openZeroSort;
	}

	public void setOpenZeroSort(boolean openZeroSort) {
		this.openZeroSort = openZeroSort;
	}


	public boolean isOpenCredibleForNotLikeSort() {
		return openCredibleForNotLikeSort;
	}

	public void setOpenCredibleForNotLikeSort(boolean openCredibleForNotLikeSort) {
		this.openCredibleForNotLikeSort = openCredibleForNotLikeSort;
	}

	public int getZeroSortCount() {
		return zeroSortCount;
	}

	public void setZeroSortCount(int zeroSortCount) {
		this.zeroSortCount = zeroSortCount;
	}

	public double getZeroDecrementWeight() {
		return zeroDecrementWeight;
	}

	public void setZeroDecrementWeight(double zeroDecrementWeight) {
		this.zeroDecrementWeight = zeroDecrementWeight;
	}

//	@Override
//    public String toString() {
//        return "AvenueSortParameter [likeWeight=" + likeWeight + ", fwdWeight=" + fwdWeight
//                + ", crediblePoint=" + crediblePoint + ", taobaoWeight=" + taobaoWeight
//                + ", defaultOwnerPic=" + defaultOwnerPic + ", uncrediblePoint=" + uncrediblePoint
//                + ", lessFriendThanValue=" + lessFriendThanValue + ", defaultRealLikeCount="
//                + defaultRealLikeCount + ", realLikeCountCorrectionCount="
//                + realLikeCountCorrectionCount + ", realLikeFirstWeigth=" + realLikeFirstWeigth
//                + ", realLikeSecondWeigth=" + realLikeSecondWeigth + ", realLikeThreeWeigth="
//                + realLikeThreeWeigth + ", realLikeForthWeigth=" + realLikeForthWeigth
//                + ", realLikeFiveWeigth=" + realLikeFiveWeigth + ", repeatGoodsFromSameUserWeight="
//                + repeatGoodsFromSameUserWeight + ", zeroCorrectionWeigh=" + zeroCorrectionWeigh
//                + ", defaultFriendCount=" + defaultFriendCount + ", openZeroSort=" + openZeroSort
//                + ", reputationCorrectionValue=" + reputationCorrectionValue
//                + ", reputationCorrectionParameter=" + reputationCorrectionParameter
//                + ", defaultReputationValue=" + defaultReputationValue
//                + ", openCredibleForNotLikeSort=" + openCredibleForNotLikeSort + ", zeroWeigth="
//                + zeroWeigth + ", zeroSortCount=" + zeroSortCount + ", zeroDecrementWeight="
//                + zeroDecrementWeight + ", timeWeight=" + timeWeight + "]";
//    }

	public double getZeroWeigth() {
		return zeroWeigth;
	}

	public void setZeroWeigth(double zeroWeigth) {
		this.zeroWeigth = zeroWeigth;
	}
    
    public double getTimeWeight() {
        return timeWeight;
    }
    
    public void setTimeWeight(double timeWeight) {
        this.timeWeight = timeWeight;
    }

}
