package com.dodoyo.invite.httpservice.excep;

public class ServiceDBException extends ServiceException {
    public ServiceDBException(String message) {
        super(message);
    }

    public ServiceDBException() {
        super();
    }

    public ServiceDBException(String message, Throwable cause) {
        super(message, cause);
    }

    public ServiceDBException(Throwable cause) {
        super(cause);
    }
}