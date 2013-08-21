package com.dodoyo.invite.core;

public class Constants {

    public static final String DEFAULT_APP_ID = "-1";//默认的app普通邀请邀请id

    public static final String RELOAD_TEMPLATE_SUFFIX = "INVITE_EMAIL_SEND";//模板重载的注册id

    //	public static final String WEB_APP_PATH = "/opt/www/";
    public static String WEB_APP_PATH = "/opt/inviteservice_www/";
//    public static String WEB_APP_PATH = "/opt/xiaonei_invite_service/www/";
//	public static final String WEB_APP_PATH = "D:/xiaonei-projects/xiaonei/xiaonei_invite_service/www/";

    //vm模板的读取位置
    public static final String REPLOAD_VELOCITY_PROPERTIES = WEB_APP_PATH + "WEB-INF/conf/velocity_invite.properties";

    public static final String EMAIL_CONFIG_PATH = WEB_APP_PATH + "WEB-INF/conf/email.xml";

    public static final String EMAIL_CONFIG_PATH2 = WEB_APP_PATH + "WEB-INF/conf/email2.xml";

    public static final String LINK_CONFIG_PATH = WEB_APP_PATH + "WEB-INF/conf/invite_link_conf.xml";
    
    public static final String LINK_CONFIG_PATH2 = WEB_APP_PATH + "WEB-INF/conf/invite_link_conf2.xml";

    public static final String RELOAD_VELOCITY_TEMPALATE_XML = WEB_APP_PATH + "WEB-INF/conf/templateConfig_invite.xml";

    public static final String SERVICE_INTERFACE_CONF_PATH = WEB_APP_PATH + "WEB-INF/conf/service_config.properties";

    public static final String EMAIL_CONFIG_FILE_CONTENT = "content";

    public static final String EMAIL_CONFIG_FILE_CONTENT_ID_ATTRIBUTE = "id";

    public static final String VELOCITY_CONFIG_FILE_ID = "id";

    public static final String VELOCITY_CONFIG_FILE_VALUE = "value";

    public static final String EMAIL_CONFIG_FILE_CONTENT_APP_ID_NODE = "appid";

    public static final String EMAIL_CONFIG_FILE_CONTENT_TEMPLATE_NODE = "template";

    public static final String EMAIL_CONFIG_FILE_CONTENT_LINK_NODE = "link";

    public static final String EMAIL_CONFIG_FILE_CONTENT_SOURCE_ID_NODE = "source_id";

    public static final String EMAIL_CONFIG_FILE_CONTENT_BODY_NODE = "body";

    public static final String EMAIL_CONFIG_FILE_CONTENT_TITLE_NODE = "title";


    public static final String EMAIL_CONFIG_FILE_MAIL_DISC = "disc";

    public static final String EMAIL_CONFIG_FILE_MAIL_SENDER = "sender";

    public static final String EMAIL_DEFAULT_TEMPLATE_ID = "22021281";

    public static final String EMAIL_NORMAIL_TEMPLATE_ID_SUFFIX = "220";

    public static final String EMAIL_APP_TEMPLATE_ID_SUFFIX = "230";

    public static final char EMAIL_NORMAIL_SOURCE_ID_INDEX = '2';

    public static final String EMAIL_CONFIG_FILE_RT_NODE = "rt";

    public static final int DEFAULT_INDEX = 0;

    public static final int DEFAULT_INDEX_ONE = 1;

    public static final int VIRTUAL_USER_ID = 700001678;

    public static final String DEFAULT_SOURCE_ID_VALUE = "0";

    public static final String DEFULT_TEMPLATE_FOR_COMMON = "0";

    public static final String DEFULT_TEMPLATE_FOR_APP = "-1";
}
