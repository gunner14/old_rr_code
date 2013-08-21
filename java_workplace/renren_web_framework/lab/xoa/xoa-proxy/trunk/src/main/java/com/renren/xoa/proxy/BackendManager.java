package com.renren.xoa.proxy;

public class BackendManager {

	private static BackendManager instance = new BackendManager();
	
	public static BackendManager getInstance() {
		return instance;
	}
	
	private String backend;
	
	private BackendManager() {
		
	}

	public String getBackend() {
		return backend;
	}

	public void setBackend(String backend) {
		this.backend = backend;
	}
	
}
