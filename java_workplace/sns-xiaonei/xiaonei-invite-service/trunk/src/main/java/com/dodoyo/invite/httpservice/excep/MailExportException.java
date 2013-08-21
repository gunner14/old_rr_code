package com.dodoyo.invite.httpservice.excep;

public class MailExportException extends ServiceException {
    /**
	 * 
	 */
	private static final long serialVersionUID = -5338667589795851068L;

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