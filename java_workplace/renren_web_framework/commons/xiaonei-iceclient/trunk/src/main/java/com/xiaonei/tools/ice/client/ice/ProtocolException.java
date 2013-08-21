package com.xiaonei.tools.ice.client.ice;

public class ProtocolException extends LocalException {

	private static final long serialVersionUID = 8597348685134161589L;

	public ProtocolException() {
	}

	public ProtocolException(String reason) {
		this.reason = reason;
	}

	public String ice_name() {
		return "Ice::ProtocolException";
	}

	public String reason;
}
