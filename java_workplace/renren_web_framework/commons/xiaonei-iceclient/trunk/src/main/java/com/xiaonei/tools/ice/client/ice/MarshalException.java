package com.xiaonei.tools.ice.client.ice;

public class MarshalException extends ProtocolException {
	private static final long serialVersionUID = 3494833330947576813L;

	public MarshalException() {
		super();
	}

	public MarshalException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::MarshalException";
	}
}
