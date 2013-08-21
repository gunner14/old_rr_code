package com.xiaonei.xce.username;

import mop.hi.oce.domain.Markable;
import xce.userbase.UserNameData;
import com.renren.tripod.proto.UserBaseProto.PbUserName;

public class UserNameInfo extends Markable {

	public static final String ID = "ID";
	public static final String NAME = "NAME";
	public static final String NICKNAME = "NICKNAME";

	protected int id;
	protected String name;
	protected String nickname;

	public UserNameInfo parse(UserNameData source) {
		id = source.uID;
		name = source.name;
		nickname = source.nickname;
		return this;
	}

	public UserNameInfo parse(PbUserName source) {
		id = (int)source.getId();
		name = source.getName();
		nickname = source.getNickname();
		return this;
	}

	public String toString() {
		String result = "UserNameInfo:[";
		result += "id:" + id;
		result += ", name:" + name;
		result += ", nickname:" + nickname;
		result += "]";
		return result;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
		mark(UserNameInfo.NAME, name);
	}

	public String getNickname() {
		return nickname;
	}

	public void setNickname(String nickname) {
		this.nickname = nickname;
		mark(UserNameInfo.NICKNAME, nickname);
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
		mark(UserNameInfo.ID, String.valueOf(id));
	}
}
