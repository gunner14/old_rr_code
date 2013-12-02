package com.renren.ad.sns.services.avenue;

import java.util.List;

public class GoodsMeta {
	long id;
	private int avenueSts;
	private long goodsId;
	private boolean defaultOwnerPic;
	private int shareCount;
	private int likeCount;
	private long ownerId;
	private int categoryId;
	private int source = 0;
	private String link;
	private List<Long> tagList;
	private List<Long> userLikeList;
	private int realLikeCount;
	private double reputation=0.20;
	private boolean isDefaultReputation=false;
	private int credible=0;//1:可信，0一般，-1不可信。
	private int price;
	public GoodsMeta(long id, int avenueSts, long goodsId,
			boolean defaultOwnerPic, int shareCount, int likeCount,
			long ownerId, int categoryId, int source, String link, int price, 
			List<Long> tagList, List<Long> userLikeList) {
		super();
		this.id = id;
		this.avenueSts = avenueSts;
		this.goodsId = goodsId;
		this.defaultOwnerPic = defaultOwnerPic;
		this.shareCount = shareCount;
		this.likeCount = likeCount;
		this.ownerId = ownerId;
		this.categoryId = categoryId;
		this.source = source;
		this.link = link;
		this.price = price;
		this.tagList = tagList;
		this.userLikeList = userLikeList;
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public int getAvenueSts() {
		return avenueSts;
	}

	public void setAvenueSts(int avenueSts) {
		this.avenueSts = avenueSts;
	}

	public long getGoodsId() {
		return goodsId;
	}

	public void setGoodsId(long goodsId) {
		this.goodsId = goodsId;
	}

	public int getShareCount() {
		return shareCount;
	}

	public void setShareCount(int shareCount) {
		this.shareCount = shareCount;
	}

	public int getLikeCount() {
		return likeCount;
	}

	public void setLikeCount(int likeCount) {
		this.likeCount = likeCount;
	}

	public long getOwnerId() {
		return ownerId;
	}

	public void setOwnerId(long ownerId) {
		this.ownerId = ownerId;
	}

	public int getCategoryId() {
		return categoryId;
	}

	public void setCategoryId(int categoryId) {
		this.categoryId = categoryId;
	}

	public int getSource() {
		return source;
	}

	public void setSource(int source) {
		this.source = source;
	}

	public String getLink() {
		return link;
	}

	public void setLink(String link) {
		this.link = link;
	}

	public List<Long> getTagList() {
		return tagList;
	}

	public void setTagList(List<Long> tagList) {
		this.tagList = tagList;
	}

	public boolean isDefaultOwnerPic() {
		return defaultOwnerPic;
	}

	public void setDefaultOwnerPic(boolean defaultOwnerPic) {
		this.defaultOwnerPic = defaultOwnerPic;
	}

	public List<Long> getUserLikeList() {
		return userLikeList;
	}

	public void setUserLikeList(List<Long> userLikeList) {
		this.userLikeList = userLikeList;
	}

	public int getRealLikeCount() {
		return realLikeCount;
	}

	public void setRealLikeCount(int realLikeCount) {
		this.realLikeCount = realLikeCount;
	}

	public double getReputation() {
		return reputation;
	}

	public void setReputation(double reputation) {
		this.reputation = reputation;
	}

	public int getCredible() {
		return credible;
	}

	public void setCredible(int credible) {
		this.credible = credible;
	}

	public boolean isDefaultReputation() {
		return isDefaultReputation;
	}

	public void setDefaultReputation(boolean isDefaultReputation) {
		this.isDefaultReputation = isDefaultReputation;
	}

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    @Override
    public String toString() {
        return "GoodsMeta [avenueSts=" + avenueSts + ", categoryId="
                + categoryId + ", credible=" + credible + ", defaultOwnerPic="
                + defaultOwnerPic + ", goodsId=" + goodsId + ", id=" + id
                + ", isDefaultReputation=" + isDefaultReputation
                + ", likeCount=" + likeCount + ", link=" + link + ", ownerId="
                + ownerId + ", price=" + price + ", realLikeCount="
                + realLikeCount + ", reputation=" + reputation
                + ", shareCount=" + shareCount + ", source=" + source
                + ", tagList=" + tagList + ", userLikeList=" + userLikeList
                + "]";
    }

}
