package com.dodoyo.invite.httpservice.excep;

public class MailInviteException extends ServiceException {
    /**
	 * 
	 */
	private static final long serialVersionUID = 6283974618385497962L;

	public MailInviteException(String message) {
        super(message);
    }

    public MailInviteException() {
        super();
    }

    public MailInviteException(String message, Throwable cause) {
        super(message, cause);
    }

    public MailInviteException(Throwable cause) {
        super(cause);
    }
}