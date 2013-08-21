package com.xiaonei.platform.core.model;

import java.io.Serializable;

import com.xiaonei.antispam.validator.HtmlPure;

public class ReserveMenu implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private int appId;
	
	private int enable;
	
	private String appName;
	
	private String sideNavUrl;
	
	private String appIcon;
	
	private int removeFlag;
	
	private int newFlag;
	
	private int countFlag;
	
	@HtmlPure
	private String operationName;
	
	@HtmlPure
	private String operationUri;

	private int newWindowFlag;
	
	private int currentFrameFlag;
	
	public ReserveMenu() {
		
	}
	
	public ReserveMenu(int appId) {
		super();
		this.appId = appId;
	}

	public ReserveMenu(int appId, int enable) {
		super();
		this.appId = appId;
		this.enable = enable;
	}

	public int getAppId() {
		return appId;
	}

	public void setAppId(int appId) {
		this.appId = appId;
	}

	public int getEnable() {
		return enable;
	}

	public void setEnable(int enable) {
		this.enable = enable;
	}

	public String getAppName() {
		return appName;
	}

	public void setAppName(String appName) {
		this.appName = appName;
	}

	public String getAppIcon() {
		return appIcon;
	}

	public void setAppIcon(String appIcon) {
		this.appIcon = appIcon;
	}

	public String getSideNavUrl() {
		return sideNavUrl;
	}

	public void setSideNavUrl(String sideNavUrl) {
		this.sideNavUrl = sideNavUrl;
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
