package com.xiaonei.xce.buddyadapter.journeylogic;

public class JourneyLogicException extends Exception {
	
	private static final long serialVersionUID = 1L;
	public JourneyLogicException(String msg) {
		super(msg);
	}
	public JourneyLogicException(String msg, Throwable t) {
		super(msg, t);
	}
	public JourneyLogicException(Throwable cause) {
		super(cause);
	}
	
}
