package com.xiaonei.platform.core.usercount;

/**
 * @author zhicai.li
 *
 */
public class AppCount {
	private int uid;
	private int appId;
	/**
	 *  该app的业务类型 
	 */
	private int type;
	
	private int count;
	
	public AppCount(int uid,int appId,int type,int count){
		this.uid = uid;
		this.appId = appId;
		this.type = type;
		this.count = count;
	}
	public int getUid() {
		return uid;
	}

	public void setUid(int uid) {
		this.uid = uid;
	}

	public int getAppId() {
		return appId;
	}

	public void setAppId(int appId) {
		this.appId = appId;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public int getCount() {
		return count;
	}

	public void setCount(int count) {
		this.count = count;
	}
}
