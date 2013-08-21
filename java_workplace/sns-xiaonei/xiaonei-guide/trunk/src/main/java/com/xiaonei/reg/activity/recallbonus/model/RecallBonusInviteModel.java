package com.xiaonei.reg.activity.recallbonus.model;

public class RecallBonusInviteModel {
	private int id;
	private int inviter_id;
	private int invitee_id;
	private String invite_time;
	private int accept_flag;
	private String accept_time;
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public int getInviter_id() {
		return inviter_id;
	}
	public void setInviter_id(int inviter_id) {
		this.inviter_id = inviter_id;
	}
	public int getInvitee_id() {
		return invitee_id;
	}
	public void setInvitee_id(int invitee_id) {
		this.invitee_id = invitee_id;
	}
	public String getInvite_time() {
		return invite_time;
	}
	public void setInvite_time(String invite_time) {
		this.invite_time = invite_time;
	}
	public int getAccept_flag() {
		return accept_flag;
	}
	public void setAccept_flag(int accept_flag) {
		this.accept_flag = accept_flag;
	}
	public String getAccept_time() {
		return accept_time;
	}
	public void setAccept_time(String accept_time) {
		this.accept_time = accept_time;
	}
	
	
}
