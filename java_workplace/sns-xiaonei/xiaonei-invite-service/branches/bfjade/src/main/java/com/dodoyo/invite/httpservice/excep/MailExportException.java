package com.dodoyo.invite.httpservice.excep;

public class MailExportException extends ServiceException {
    public MailExportException(String message) {
        super(message);
    }

    public MailExportException() {
        super();
    }

    public MailExportException(String message, Throwable cause) {
        super(message, cause);
    }

    public MailExportException(Throwable cause) {
        super(cause);
    }
}