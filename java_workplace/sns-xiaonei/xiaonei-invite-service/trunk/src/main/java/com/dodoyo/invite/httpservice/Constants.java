package com.dodoyo.invite.httpservice;

public class Constants {
    /**
     * for link invite
     */
    public static final String SERVICE_ACTION_LINK = "link";

    /**
     * for all link invite
     */
    public static final String SERVICE_ACTION_LINK_ALL = "link-all";

    /**
     * for batch link invite
     */
    public static final String SERVICE_ACTION_LINK_BATCH = "link-batch";

    /**
     * for email direct invite
     */
    public static final String SERVICE_ACTION_EMAIL = "email";

    /**
     * for page using, send request first
     */
    public static final String SERVICE_ACTION_EXPORT_REQUEST = "export-request";

    /**
     * for page using, query list latter
     */
    public static final String SERVICE_ACTION_EXPORT_QUERY = "export-query";

    /**
     * for mail export contacts
     */
    public static final String SERVICE_ACTION_EXPORT_CALLBACK = "export-callback";

    /**
     * for mail export contacts and send
     */
    public static final String SERVICE_ACTION_EXPORT_INVITE_CALLBACK = "export-invite-callback";

    /**
     * service status open or not
     */
    public static final String SERVICE_STATUS_OPEN = "on";

    /**
     * invite result codes define
     */
    public static final int INVITE_RESULT_SUCCESS = 0;

    public static final int INVITE_RESULT_EXCP = 1;

    public static final int INVITE_RESULT_INVALID = 2;

    public static final int INVITE_RESULT_UNAUTHED = 3;

    public static final int INVITE_RESULT_INVALID_CLIENT = 4;

    public static final int INVITE_RESULT_NOT_AVAILABLE = 1000;
    /**
     * authentication related
     */

    public static final String TOKEN_KEY = "xninvitekey";

    public static final int TOKEN_EXPIRE_SENCONDS = 3;

    public static final int TOKEN_RESULT_OK = 0;

    public static final int TOKEN_RESULT_INVALID = 1;

    public static final int TOKEN_RESULT_EXPIRED = 2;

    public static final int TOKEN_RESULT_USED = 3;


}