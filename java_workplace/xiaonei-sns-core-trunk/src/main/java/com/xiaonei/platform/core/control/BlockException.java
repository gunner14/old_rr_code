package com.xiaonei.platform.core.control;

public class BlockException extends RuntimeException{
	private static final long serialVersionUID = 2326057672786039124L;

	public BlockException() {
		super();
	}

	public BlockException(String message, Throwable cause) {
		super(message, cause);
	}

	public BlockException(String message) {
		super(message);
	}

	public BlockException(Throwable cause) {
		super(cause.getMessage(),cause);
	}

}
