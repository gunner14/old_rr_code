package com.xiaonei.platform.core.image;

import java.util.Map;

public class ImageStorageModel {

	public static final int STATUS_OK = 0;
	public static final int STATUS_SYSTEM_BUSY = 1;
	public static final int STATUS_ALREADY_EXIST = 2;
	public static final int STATUS_FAILED = 3;

	public static final String KEY_SIZE_LARGE = "large";
	public static final String KEY_SIZE_MAIN = "main";
	public static final String KEY_SIZE_HEAD = "head";
	public static final String KEY_SIZE_TINY = "tiny";

	private Map<String, String> map;

	private int status;

	public ImageStorageModel(int status) {
		this.status = status;
	}

	public ImageStorageModel(int status, Map<String, String> map) {
		this.status = status;
		this.map = map;
	}

	public Map<String, String> getMap() {
		return map;
	}

	public void setMap(Map<String, String> map) {
		this.map = map;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

}
