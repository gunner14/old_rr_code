package com.kaixin.register.model;

import java.io.Serializable;

public class Invite implements Serializable {

	private static final long serialVersionUID = 132891811313664578L;

	private int userId;

	private String invitecode;

	public String getInvitecode() {
		return invitecode;
	}

	public void setInvitecode(String invitecode) {
		this.invitecode = invitecode;
	}

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

}
