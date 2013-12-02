package com.xiaonei.xce.ticket;

import mop.hi.oce.domain.Markable;

public class TicketInfo extends Markable {

	private static final String ID = "ID";
	private static final String REALID = "REALID";
	private static final String TYPE = "TYPE";
	private static final String IP = "IP";

	public final static class Type {
		public final static int Web = 0;
		public final static int Wap = 1;
		public final static int PlatForm = 2;
		public final static int Client = 3;
		public final static int MobileClient = 4;
	}

	private long _id;

	private long _realId;

	private int _type;

	private String _ip;

	public long get_id() {
		return _id;
	}

	public void set_id(long id) {
		this._id = id;
		mark(ID, String.valueOf(id));
	}

	public long get_realId() {
		return _realId;
	}

	public void set_realId(long realId) {
		this._realId = realId;
		mark(REALID, String.valueOf(realId));
	}

	public int get_type() {
		return _type;
	}

	public void set_type(int type) {
		this._type = type;
		mark(TYPE, String.valueOf(type));
	}

	public String get_ip() {
		return _ip;
	}

	public void set_ip(String ip) {
		this._ip = ip;
		mark(IP, ip);
	}

	public String toString() {
		String result = "TicketInfo[";
		result += "id:" + _id + ",";
		result += "realId:" + _realId + ",";
		result += "type:" + _type + ",";
		result += "ip:" + _ip + "]";
		return result;
	}

}
