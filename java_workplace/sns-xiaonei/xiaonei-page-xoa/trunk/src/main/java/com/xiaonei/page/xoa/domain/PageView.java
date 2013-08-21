package com.xiaonei.page.xoa.domain;

import java.sql.Timestamp;
import java.util.Date;

public class PageView {
	/** 开启 */
	public static final int STATE_OPEN = 0;

	/** 关闭 */
	public static final int STATE_CLOSE = 1;

	/** 隐藏 */
	public static final int STATE_HIDE = 2;

	/** 默认的粉丝数无上限 */
	public static final int DEFAULT_FANS_COUNT_LIMIT = -1;

	private Integer id;
	/**
	 * page名称
	 */
	private String name;

	private String alias;

	/**
	 * 自我介绍
	 */
	private String description;

	/**
	 * 创建者id
	 */
	private Integer ownerId;

	/**
	 * 年龄限制
	 */
	private Integer ageRestriction;

	/**
	 * 当前状态 ［新建｜不公布|仅fans可见｜所有人可见］
	 */
	private Integer state;   //当前为情侣空间时： 0：正常开启的， 1：被超管封禁（或者仅主人可见），6：注销PAGE---情侣不能操作，如再开启需经过双方都同意，7: 被屏蔽的  8：好友可见，9：共同好友可见

	/**
	 * 一级分类
	 */
	private Integer type;

	private String typeString = "Famous";

	/**
	 * 二级分类
	 */
	private Integer subType;

	private String subTypeString = "SuperStar";

	/**
	 * 创建时间
	 */
	private Date createTime;

	/**
	 * page的可见度
	 */
	private String visibility;

	private int fansCount;

	// -----------------huqi add----------------

	private boolean hasIncrement = false;

	private int counta = 0;

	private int incrementNumHigh = 0;

	private int incrementNumMid = 0;

	private int incrementNumLow = 0;

	private String tinyUrl;

	private String headUrl;

	private String mainUrl;

	private String largeUrl;

	private boolean checked;

	private Timestamp apply_time;// pageAuditTime表中读取的审核申请时间, 不能在外部直接被调用

	private int level;// //pageAuditTime中的排序等级,不能再外部直接被调用

	private int oriType;// 最顶级的分类[page、zhaopin、app?]

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAlias() {
		return alias;
	}

	public void setAlias(String alias) {
		this.alias = alias;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public Integer getOwnerId() {
		return ownerId;
	}

	public void setOwnerId(Integer ownerId) {
		this.ownerId = ownerId;
	}

	public Integer getAgeRestriction() {
		return ageRestriction;
	}

	public void setAgeRestriction(Integer ageRestriction) {
		this.ageRestriction = ageRestriction;
	}

	public Integer getState() {
		return state;
	}

	public void setState(Integer state) {
		this.state = state;
	}

	public Integer getType() {
		return type;
	}

	public void setType(Integer type) {
		this.type = type;
	}

	public String getTypeString() {
		return typeString;
	}

	public void setTypeString(String typeString) {
		this.typeString = typeString;
	}

	public Integer getSubType() {
		return subType;
	}

	public void setSubType(Integer subType) {
		this.subType = subType;
	}

	public String getSubTypeString() {
		return subTypeString;
	}

	public void setSubTypeString(String subTypeString) {
		this.subTypeString = subTypeString;
	}

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public String getVisibility() {
		return visibility;
	}

	public void setVisibility(String visibility) {
		this.visibility = visibility;
	}

	public int getFansCount() {
		return fansCount;
	}

	public void setFansCount(int fansCount) {
		this.fansCount = fansCount;
	}

	public boolean isHasIncrement() {
		return hasIncrement;
	}

	public void setHasIncrement(boolean hasIncrement) {
		this.hasIncrement = hasIncrement;
	}

	public int getCounta() {
		return counta;
	}

	public void setCounta(int counta) {
		this.counta = counta;
	}

	public int getIncrementNumHigh() {
		return incrementNumHigh;
	}

	public void setIncrementNumHigh(int incrementNumHigh) {
		this.incrementNumHigh = incrementNumHigh;
	}

	public int getIncrementNumMid() {
		return incrementNumMid;
	}

	public void setIncrementNumMid(int incrementNumMid) {
		this.incrementNumMid = incrementNumMid;
	}

	public int getIncrementNumLow() {
		return incrementNumLow;
	}

	public void setIncrementNumLow(int incrementNumLow) {
		this.incrementNumLow = incrementNumLow;
	}

	public String getTinyUrl() {
		return tinyUrl;
	}

	public void setTinyUrl(String tinyUrl) {
		this.tinyUrl = tinyUrl;
	}

	public String getHeadUrl() {
		return headUrl;
	}

	public void setHeadUrl(String headUrl) {
		this.headUrl = headUrl;
	}

	public String getMainUrl() {
		return mainUrl;
	}

	public void setMainUrl(String mainUrl) {
		this.mainUrl = mainUrl;
	}

	public String getLargeUrl() {
		return largeUrl;
	}

	public void setLargeUrl(String largeUrl) {
		this.largeUrl = largeUrl;
	}

	public boolean isChecked() {
		return checked;
	}

	public void setChecked(boolean checked) {
		this.checked = checked;
	}

	public Timestamp getApply_time() {
		return apply_time;
	}

	public void setApply_time(Timestamp apply_time) {
		this.apply_time = apply_time;
	}

	public int getLevel() {
		return level;
	}

	public void setLevel(int level) {
		this.level = level;
	}

	public int getOriType() {
		return oriType;
	}

	public void setOriType(int oriType) {
		this.oriType = oriType;
	}
	

	
}
