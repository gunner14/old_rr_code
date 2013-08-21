package com.xiaonei.tools.monitor.entity;

public class IMEntity {

	private long id;

	private String type;

	private long costTime;

	private String urlOrHost;

	private String happenTime;

	private String IP = "undefined";
	private String remoteHost;

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public long getCostTime() {
		return costTime;
	}

	public void setCostTime(long costTime) {
		this.costTime = costTime;
	}

	public String getUrlOrHost() {
		return urlOrHost;
	}

	public void setUrlOrHost(String urlOrHost) {
		this.urlOrHost = urlOrHost;
	}

	public String getHappenTime() {
		return happenTime;
	}

	public void setHappenTime(String happenTime) {
		this.happenTime = happenTime;
	}

	public String getIP() {
		return IP;
	}

	public void setIP(String iP) {
		IP = iP;
	}

	public String getRemoteHost() {
		return remoteHost;
	}

	public void setRemoteHost(String remoteHost) {
		this.remoteHost = remoteHost;
	}

}
