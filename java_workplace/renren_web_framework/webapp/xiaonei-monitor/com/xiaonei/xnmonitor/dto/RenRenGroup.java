package com.xiaonei.xnmonitor.dto;

public class RenRenGroup {
	private String groupName;// The Group's short name
	private String groupFullName;// The group's full name
	private String leaderName;// The group's leader's name
	private String leaderEmail;// The group's leader's email
	private String leaderMobile; // The group's leader's mobile
	private String groupEmail;// The group's Email

	public String getGroupName() {
		return groupName;
	}

	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}

	public String getGroupFullName() {
		return groupFullName;
	}

	public void setGroupFullName(String groupFullName) {
		this.groupFullName = groupFullName;
	}

	public String getLeaderName() {
		return leaderName;
	}

	public void setLeaderName(String leaderName) {
		this.leaderName = leaderName;
	}

	public String getLeaderEmail() {
		return leaderEmail;
	}

	public void setLeaderEmail(String leaderEmail) {
		this.leaderEmail = leaderEmail;
	}

	public String getLeaderMobile() {
		return leaderMobile;
	}

	public void setLeaderMobile(String leaderMobile) {
		this.leaderMobile = leaderMobile;
	}

	public String getGroupEmail() {
		return groupEmail;
	}

	public void setGroupEmail(String groupEmail) {
		this.groupEmail = groupEmail;
	}

	// groups | CREATE TABLE `groups` (
	// `id` int(11) NOT NULL auto_increment,
	// `group_name` varchar(100) NOT NULL,
	// `group_ful_name` varchar(500) default NULL,
	// `leader_name` varchar(100) default NULL,
	// `leader_email` varchar(100) default NULL,
	// `group_email` varchar(100) default NULL,
	// `leader_mobile` varchar(20) default NULL,

}
