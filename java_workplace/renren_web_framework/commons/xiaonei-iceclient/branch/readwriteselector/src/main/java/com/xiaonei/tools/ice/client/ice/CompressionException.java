package com.xiaonei.tools.ice.client.ice;

public class CompressionException extends ProtocolException {
	public CompressionException() {
		super();
	}

	public CompressionException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::CompressionException";
	}
}
