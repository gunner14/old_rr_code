package com.dodoyo.invite.httpservice.excep;

import com.dodoyo.invite.core.logic.LogUtils;

public class ExceptionManager {
    private static ExceptionManager em = new ExceptionManager();

    public static ExceptionManager getInstance() {
        return em;
    }

    public void logServiceException(ServiceException se, String message, Throwable cause) {
        LogUtils.logDebug(se.getClass() + "**********"
                + message + "**********" + (cause == null ? "" : cause.getLocalizedMessage()));
    }
}