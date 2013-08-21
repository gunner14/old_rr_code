package com.xiaonei.platform.scheduleTask;
import java.util.Map;

public class HttpClientType {
	String url;

	String methodtype;

	String encoding;

	String ip;

	String port;

	Map parameters;

	public String getEncoding() {
		return encoding;
	}

	public void setEncoding(String encoding) {
		this.encoding = encoding;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public String getMethodtype() {
		return methodtype;
	}

	public void setMethodtype(String methodtype) {
		this.methodtype = methodtype;
	}

	public Map getParameters() {
		return parameters;
	}

	public void setParameters(Map parameters) {
		this.parameters = parameters;
	}

	public String getPort() {
		return port;
	}

	public void setPort(String port) {
		this.port = port;
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

}
