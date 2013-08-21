package com.xiaonei.platform.core.model;

import java.io.Serializable;

/**
 * Api_key
 * 
 * @author fusong.li@opi-corp.com 2008-3-27 下午04:37:37
 */
public class ApiKey implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 7765840080147297814L;

	private int appId;

	private String apiKey;

	public String getApiKey() {
		return apiKey;
	}

	public void setApiKey(String apiKey) {
		this.apiKey = apiKey;
	}

	public int getAppId() {
		return appId;
	}

	public void setAppId(int appId) {
		this.appId = appId;
	}

}
