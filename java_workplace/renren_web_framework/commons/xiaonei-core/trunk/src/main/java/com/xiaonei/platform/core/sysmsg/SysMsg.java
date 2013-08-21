package com.xiaonei.platform.core.sysmsg;

/**
 * @author Zhang Tielei
 * 存放系统消息（出错或警告信息）的对象
 *
 */
public class SysMsg {
	private String msg;
	private int msgType;
	
	public String getMsg() {
		return msg;
	}
	public void setMsg(String msg) {
		this.msg = msg;
	}
	public int getMsgType() {
		return msgType;
	}
	public void setMsgType(int msgType) {
		this.msgType = msgType;
	}

}
