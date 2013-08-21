package com.xiaonei.tools.ice.client.ice;

public class MemoryLimitException extends MarshalException {
	private static final long serialVersionUID = 7537095548636663514L;

	public MemoryLimitException() {
		super();
	}

	public MemoryLimitException(String reason) {
		super(reason);
	}

	public String ice_name() {
		return "Ice::MemoryLimitException";
	}
}
