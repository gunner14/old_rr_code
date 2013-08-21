package com.dodoyo.invite.httpservice.excep;

public class LinkInviteException extends ServiceException {
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