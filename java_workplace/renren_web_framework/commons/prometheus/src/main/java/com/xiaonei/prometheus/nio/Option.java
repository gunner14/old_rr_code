package com.xiaonei.prometheus.nio;

public class Option {
	private int selectorPoolSize;
	private boolean enableWorkerThread = false;
	private int workerThreadSize = 10;
	public Option(int selectorPoolSize){
		this.selectorPoolSize = selectorPoolSize;
	}
	public int getSelectorPoolSize() {
		return selectorPoolSize;
	}
	public void setSelectorPoolSize(int selectorPoolSize) {
		this.selectorPoolSize = selectorPoolSize;
	}
	public boolean isEnableWorkerThread() {
		return enableWorkerThread;
	}
	public void setEnableWorkerThread(boolean enableWorkerThread) {
		this.enableWorkerThread = enableWorkerThread;
	}
	public int getWorkerThreadSize() {
		return workerThreadSize;
	}
	public void setWorkerThreadSize(int workerThreadSize) {
		this.workerThreadSize = workerThreadSize;
	}
	
}
