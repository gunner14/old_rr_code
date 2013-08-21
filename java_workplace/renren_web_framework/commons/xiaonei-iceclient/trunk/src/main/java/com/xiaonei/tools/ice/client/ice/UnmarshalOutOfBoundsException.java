package com.xiaonei.tools.ice.client.ice;

public class UnmarshalOutOfBoundsException extends MarshalException {
	private static final long serialVersionUID = 4193546464991350444L;

	public UnmarshalOutOfBoundsException() {
		super();
	}

	public UnmarshalOutOfBoundsException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::UnmarshalOutOfBoundsException";
	}
}
