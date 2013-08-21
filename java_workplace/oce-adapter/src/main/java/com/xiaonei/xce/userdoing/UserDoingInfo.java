package com.xiaonei.xce.userdoing;

import java.util.Date;

import mop.hi.oce.domain.Markable;
import xce.userbase.UserDoingData;
import xce.util.tools.DateFormatter;
import com.renren.tripod.proto.UserBaseProto.PbUserDoing;

public class UserDoingInfo extends Markable {

	public static final String ID = "ID";
	public static final String DOING = "DOING";
	public static final String DOINGTIME = "DOINGTIME";

	protected int id;
	protected String doing;
	protected Date doingTime;

	public UserDoingInfo parse(UserDoingData source) {
		this.id = source.id;
		this.doing=source.doing;
		this.doingTime = new Date(source.doingTime * 1000l);
		return this;
	}

	public UserDoingInfo parse(PbUserDoing source) {
		this.id = (int)source.getId();
		this.doing = source.getDoing();
		this.doingTime = new Date(source.getDoingtime() * 1000l);
		return this;
	}

	public String toString() {
		String result = "";
		result += "UserDoingInfo:[";
		result += "id:" + id;
		result += ",doing:" + doing;
		result += ",doingTime:" + doingTime;
		result += "]";
		return result;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
		mark(UserDoingInfo.ID,String.valueOf(id));
	}

	public String getDoing() {
		return doing;
	}

	public void setDoing(String doing) {
		this.doing = doing;
		mark(UserDoingInfo.DOING,doing);
	}

	public Date getDoingTime() {
		return doingTime;
	}

	public void setDoingTime(Date doingTime) {
		this.doingTime = doingTime;
		if (doingTime==null||doingTime.getTime() == 0) {
			return;
		}
		mark(UserDoingInfo.DOINGTIME, DateFormatter.format(doingTime));
	}

}
