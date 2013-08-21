package com.xiaonei.tools.ice.client.ice;

public class NegativeSizeException extends MarshalException {
	public NegativeSizeException() {
		super();
	}

	public NegativeSizeException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::NegativeSizeException";
	}
}
