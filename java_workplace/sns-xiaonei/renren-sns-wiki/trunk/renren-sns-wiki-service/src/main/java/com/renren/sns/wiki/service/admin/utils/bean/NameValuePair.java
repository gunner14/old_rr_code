package com.renren.sns.wiki.service.admin.utils.bean;

/**
 * httpToolkit提交post使用
 * @author weiwei.wang@renren-inc.com since 2012-8-8
 *
 */
public class NameValuePair {
	
	private String name;
	private String value;
	
	public NameValuePair(String name, String value) {
		this.name = name;
		this.value = value;
	}
	public String getName() {
		return name;
	}
	public String getValue() {
		return value;
	}
}
