package com.xiaonei.tools.ice.client.ice;

public class UnsupportedEncodingException extends ProtocolException {
	public UnsupportedEncodingException() {
		super();
	}

	public UnsupportedEncodingException(String reason, int badMajor,
			int badMinor, int major, int minor) {
		super(reason);
		this.badMajor = badMajor;
		this.badMinor = badMinor;
		this.major = major;
		this.minor = minor;
	}

	public String ice_name() {
		return "Ice::UnsupportedEncodingException";
	}

	public int badMajor;

	public int badMinor;

	public int major;

	public int minor;
}
