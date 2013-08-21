package com.xiaonei.platform.core.usercount;

/**
 * @author zhicai.li
 *
 */
public class UserCount {
	private int uid;
	/**
	 * 非APP业务的类型  
	 */
	private int type;
	/**
	 * 
	 */
	private int count;
    
	public UserCount(int uid,int type,int count){
		this.uid = uid;
		this.type = type;
		this.count = count;
	}

	public int getUid() {
		return uid;
	}

	public void setUid(int uid) {
		this.uid = uid;
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
