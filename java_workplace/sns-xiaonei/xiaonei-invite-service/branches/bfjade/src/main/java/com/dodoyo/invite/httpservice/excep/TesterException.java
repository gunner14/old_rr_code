package com.dodoyo.invite.httpservice.excep;

public class TesterException extends ServiceException {
    public TesterException(String message) {
        super(message);
    }

    public TesterException() {
        super();
    }

    public TesterException(String message, Throwable cause) {
        super(message, cause);
    }

    public TesterException(Throwable cause) {
        super(cause);
    }

}