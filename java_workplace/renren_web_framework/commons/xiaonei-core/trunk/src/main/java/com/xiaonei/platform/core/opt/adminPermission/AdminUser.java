package com.xiaonei.platform.core.opt.adminPermission;

import java.util.List;

public class AdminUser {
	
	private int id;
	
	private int auth;
	
	private List<int[]> right;

	public int getAuth() {
		return auth;
	}

	public void setAuth(int auth) {
		this.auth = auth;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public List<int[]> getRight() {
		return right;
	}

	public void setRight(List<int[]> right) {
		this.right = right;
	}
}
