package com.xiaonei.platform.core.domain;

import java.io.Serializable;
/**
 * domain_config 表对应的model对象
 */
public class DomainName implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = 568250375654986912L;
	private String domainName;
	private String domainUrl;
	public String getDomainName() {
		return domainName;
	}
	public void setDomainName(String domainName) {
		this.domainName = domainName;
	}
	public String getDomainUrl() {
		return domainUrl;
	}
	public void setDomainUrl(String domainUrl) {
		this.domainUrl = domainUrl;
	}
	
	/*
	 * 先注释掉，如果有代码依赖这里，就会报编译错误
	 * 
	
	@Deprecated
	public String getName() {
		return domainName;
	}
	@Deprecated
	public void setName(String name) {
		this.domainName = name;
	}
	@Deprecated
	public String getUrl() {
		return domainUrl;
	}
	@Deprecated
	public void setUrl(String url) {
		this.domainUrl = url;
	}*/
}
