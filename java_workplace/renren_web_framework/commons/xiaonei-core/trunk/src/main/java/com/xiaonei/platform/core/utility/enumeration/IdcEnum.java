package com.xiaonei.platform.core.utility.enumeration;

/**
 * 枚举idc，以此严格控制idc的范围
 * 
 * @author chuanyu.ban@renren-inc.com
 * 
 */
public enum IdcEnum {
	/**
	 * 电信
	 */
	TEL("tel"),
	/**
	 * 联通/网通
	 */
	UNI("uni"),
	/**
	 * 教育网
	 */
	EDU("edu");

	private String idc;

	private IdcEnum(String idc) {
		this.idc = idc;
	}

	public String getIdc() {
		return this.idc;
	}
}
