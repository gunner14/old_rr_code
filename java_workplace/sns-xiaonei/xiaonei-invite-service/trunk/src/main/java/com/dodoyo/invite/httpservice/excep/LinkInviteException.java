package com.dodoyo.invite.httpservice.excep;

public class LinkInviteException extends ServiceException {
    /**
	 * 
	 */
	private static final long serialVersionUID = 1647999965875848146L;

	public LinkInviteException(String message) {
        super(message);
    }

    public LinkInviteException() {
        super();
    }

    public LinkInviteException(String message, Throwable cause) {
        super(message, cause);
    }

    public LinkInviteException(Throwable cause) {
        super(cause);
    }
}