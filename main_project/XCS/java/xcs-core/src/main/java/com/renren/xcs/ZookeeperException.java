package com.renren.xcs;

import com.renren.ad.datacenter.find.core.FindException;

public class ZookeeperException extends XcsException {

	public static enum Code {
		NOAUTH, CONNECTIONLOSS, NONODE, OTHER
	}

	private static final long serialVersionUID = 110436013251266311L;

	private Code code;

	public Code getCode() {
		return code;
	}

	public ZookeeperException(Code code, String message, Exception e) {
		super(message, e);
		this.code = code;
	}

	public ZookeeperException(Code code, String message) {
		super(message);
		this.code = code;
	}

	public static ZookeeperException valueOf(Exception e) {
		ZookeeperException ze;
		if (e instanceof FindException) {
			FindException fe = (FindException) e;
			com.renren.ad.datacenter.find.core.FindException.Code code = fe.getCode();
			switch (code) {
			case DISCONNECTED:
				ze = new ZookeeperException(Code.CONNECTIONLOSS, e.getMessage(), e);
				break;
			case NOAUTH:
				ze = new ZookeeperException(Code.NOAUTH, e.getMessage(), e);
				break;
			case NONODE:
				ze = new ZookeeperException(Code.NONODE, e.getMessage(), e);
				break;
			default:
				ze = new ZookeeperException(Code.OTHER, e.getMessage(), e);
				break;
			}
		} else {
			ze = new ZookeeperException(Code.OTHER, e.getMessage(), e);
		}
		
		return ze;
	}
}
