package com.xiaonei.tools.ice.client.ice;

public class EncapsulationException extends MarshalException {
	public EncapsulationException() {
		super();
	}

	public EncapsulationException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::EncapsulationException";
	}
}
