package com.xiaonei.commons.interceptors.injection.messages;

/**
 * {@link PageMessage}
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */

public class PageMessage {

	private String type;
	private String name;
	private String defaultMessage;

	public PageMessage() {
	}

	public PageMessage(String type, String name, String content) {
		this.type = type;
		this.name = name;
		this.defaultMessage = content;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public void setDefaultMessage(String defaultMessage) {
		this.defaultMessage = defaultMessage;
	}

	public String getDefaultMessage() {
		return defaultMessage;
	}

}
