package com.xiaonei.platform.core.model;

import java.io.Serializable;

import com.xiaonei.antispam.validator.HtmlPure;

public class SubMenu implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private int id;
	
	private int appId;
	
	@HtmlPure
	private String menuName;
	
	@HtmlPure
	private String menuUri;
	
	private int orderNum;
	
	private int enable;
	
	private int removeFlag;
	
	private int newFlag;
	
	private int countFlag;
	
	private int newWindowFlag;
	
	private int currentFrameFlag;
	
	@HtmlPure
	private String operationName;
	
	@HtmlPure
	private String operationUri;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getAppId() {
		return appId;
	}

	public void setAppId(int appId) {
		this.appId = appId;
	}

	public String getMenuName() {
		return menuName;
	}

	public void setMenuName(String menuName) {
		this.menuName = menuName;
	}

	public String getMenuUri() {
		return menuUri;
	}

	public void setMenuUri(String menuUri) {
		this.menuUri = menuUri;
	}

	public int getOrderNum() {
		return orderNum;
	}

	public void setOrderNum(int orderNum) {
		this.orderNum = orderNum;
	}

	public int getEnable() {
		return enable;
	}

	public void setEnable(int enable) {
		this.enable = enable;
	}

	public int getRemoveFlag() {
		return removeFlag;
	}

	public void setRemoveFlag(int removeFlag) {
		this.removeFlag = removeFlag;
	}

	public int getNewFlag() {
		return newFlag;
	}

	public void setNewFlag(int newFlag) {
		this.newFlag = newFlag;
	}

	public int getCountFlag() {
		return countFlag;
	}

	public void setCountFlag(int countFlag) {
		this.countFlag = countFlag;
	}

	public String getOperationName() {
		return operationName;
	}

	public void setOperationName(String operationName) {
		this.operationName = operationName;
	}

	public String getOperationUri() {
		return operationUri;
	}

	public void setOperationUri(String operationUri) {
		this.operationUri = operationUri;
	}

	public int getNewWindowFlag() {
		return newWindowFlag;
	}

	public void setNewWindowFlag(int newWindowFlag) {
		this.newWindowFlag = newWindowFlag;
	}

	public int getCurrentFrameFlag() {
		return currentFrameFlag;
	}

	public void setCurrentFrameFlag(int currentFrameFlag) {
		this.currentFrameFlag = currentFrameFlag;
	}
	
}
