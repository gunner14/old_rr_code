package com.xiaonei.page.xoa.domain;

/**
 * 检查的结果类，用于返回给调用antispam者
 * 
 * @version 1.0.0 from 2009年4月
 */
public class CheckResultView {

	/**
	 * 判断类型
	 */
	private int flag;

	/**
	 * 原文
	 */
	private String preContent;

	/**
	 * 安全过滤后的内容
	 */
	private String safeContent;

	/**
	 * 高亮提示信息，给后台使用
	 */
	private String highLightContent;

	/**
	 * 先审后发
	 */
	private boolean auditFirst = false;

	/**
	 * 不发新鲜事
	 */
	private boolean notNotify = false;

	/**
	 * 广告
	 */
	private boolean advertisement = false;

	/**
	 * 是否超时后的结果
	 */
	private boolean timeout = false;

	/**
	 * 封禁类型
	 */
	private int infoType = 0;
	
	/**
	 * 提示信息
	 */
	private String showMessage = DEFAULT_MESSAGE;

	public final static String PROHIBITED_MESSAGE = "发表失败，信息包含不合适内容，请检查";

	public final static String AUDIT_BEFORE_SEND_MESSAGE = "您的内容审核通过后会自动发表，无须重复提交";

	public final static String ACTIVITY_FAST_MESSAGE = "您的操作过于频繁，请稍后再试";

	public final static String ACTIVITY_SIMILAR_MESSAGE = "请您不要频繁发布相同内容";

	public final static String DEFAULT_MESSAGE = "发布成功";

	/**
	 * 
	 * @Title: isTimeout
	 * @Description: 是否超时的结果
	 * @return boolean
	 */
	public boolean isTimeout() {
		return timeout;
	}

	/**
	 * 是否是超时后的结果
	 * 
	 * @param timeout
	 */
	public void setTimeout(boolean timeout) {
		this.timeout = timeout;
	}

	/**
	 * 
	 * @Title: setInfoType
	 * @description 设置过滤的信息类型政治、色情等
	 * @param infoType
	 * @return void
	 */
	public void setInfoType(int infoType) {
		this.infoType = infoType;
	}

	/**
	 * @title:getInfoType
	 * @description 获取过滤内容的类型，政治、色情、重复发送、过快发送等{@link InfoType}
	 * @return int
	 */
	public int getInfoType() {
		return infoType;
	}

	/**
	 * 
	 * @Title: getFlag
	 * @Description: 获取过滤结果标记
	 * @return int
	 */
	public int getFlag() {
		return flag;
	}

	/**
	 * 
	 * @Title: getSafeContent
	 * @Description: 获取安全过滤后的文本内容，不推荐使用!
	 *               推荐根据业务需求，获取不同的接口如：getSafeRichContent/getSafePureContent
	 *               /getSafeJsonPureContent
	 * @return String
	 */
	public String getSafeContent() {
		return safeContent;
	}

	/**
	 * 
	 * @Title: getHighLightContent
	 * @Description: 获取标记的高亮内容
	 * @return String
	 */
	public String getHighLightContent() {
		return highLightContent;
	}

	/**
	 * 
	 * @Title: setNotNotify
	 * @Description: 设置是否不法新鲜事
	 * @param noNotify
	 * @return void
	 */
	public void setNotNotify(boolean noNotify) {
		this.notNotify = noNotify;
	}

	/**
	 * 
	 * @Title: isNotNotify
	 * @Description: 检测是否发新鲜事
	 * @return boolean
	 */
	public boolean isNotNotify() {
		return notNotify;
	}

	/**
	 * 
	 * @Title: isAdvertisement
	 * @Description: 检测是否是广告
	 * @return boolean
	 */
	public boolean isAdvertisement() {
		return advertisement;
	}

	/**
	 * 
	 * @Title: setAuditFirst
	 * @Description: 设置是否先审核后发布
	 * @param auditFirst
	 * @return void
	 */
	public void setAuditFirst(boolean auditFirst) {
		this.auditFirst = auditFirst;
	}

	/**
	 * 
	 * @Title: isAuditFirst
	 * @Description: 检测是否先审核后发布
	 * @return boolean
	 */
	public boolean isAuditFirst() {
		return auditFirst;
	}

	/** 安全 */
	public static final int SAFE = 0;

	/** 不发新鲜事 */
	public static final int CONTENT_NOT_NOTIFY = 15;

	/** 需要人工审核 */
	public static final int CONTENT_NEED_AUDIT = 20;

	/** 需要人工审核并且不发新鲜事 */
	public static final int AUDIT_AND_NOT_NOTIFY = 22;

	/** 先审后发 */
	public static final int AUDIT_BEFORE_SEND = 23;

	/** 违禁，放入黑洞 */
	public static final int BLACK_HOLE = 29;

	/** 违禁，不能发布 */
	public static final int PROHIBITED = 30;

	/**
	 * 系统错误
	 */
	public static final int SYSTEM_ERROR = 100;

	/**
	 * 需要真实名字
	 */
	public static final int NEED_REAL_NAME = 110;

	/**
	 * 可能是错误的名字
	 */
	public static final int MAY_BE_FALSE_NAME = 115;

	/**
	 * 是钓鱼网页
	 */
	public static final int PHISHING_PAGE = 120;

	public static final int CONTAIN_VICIOUSURL = 130;

	public static final int PAGE_CONTAIN64 = 140;

	public static final int REGISTER_SAFE = 0;

	public static final int REGISTER_SPAM_IP = 11;

	public static final int REGISTER_SPAM_PHONE = 12;

	public static final int REGISTER_SPAM_EMAIL = 13;

	public static final int REGISTER_SPAM_SIMILAR = 14;

	public String getPreContent() {
		return preContent;
	}

	public void setPreContent(String preContent) {
		this.preContent = preContent;
	}

	public void setFlag(int flag) {
		this.flag = flag;
	}

	public void setSafeContent(String safeContent) {
		this.safeContent = safeContent;
	}

	public void setHighLightContent(String highLightContent) {
		this.highLightContent = highLightContent;
	}

	public void setAdvertisement(boolean advertisement) {
		this.advertisement = advertisement;
	}
	/**
	 * 显示提示信息
	 * @return
	 */
	public String getShowMessage() {
		return showMessage;
	}

	public void setShowMessage(String showMessage) {
		this.showMessage = showMessage;
	}
	
	

}
