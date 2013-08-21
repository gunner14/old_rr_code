package com.xiaonei.tools.ice.client.ice;

public class IllegalMessageSizeException extends ProtocolException {
	public IllegalMessageSizeException() {
		super();
	}

	public IllegalMessageSizeException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::IllegalMessageSizeException";
	}
}
