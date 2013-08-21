package com.xiaonei.reg.usertrace.model;

public class RegOprDefine {

    //1.打开注册页面的动作
    //默认的情况
    public static final int DEFAULT_XN_REG = 100001;
    //第一次访问
    public static final int FIRSR_VISIT_XN_REG = 100002;
    //普通注册流程
    //	public static final int NORMAL_VISIT_XN_REG 		= 100003;
    public static final int NORMAL_VISIT_XN_REG = 106201;
    //普通链接邀请注册
    //	public static final int NORMAL_LINK_INVITE_REG_XN 	= 100004;
    public static final int NORMAL_LINK_INVITE_REG_XN = 106101;
    //普通邮件邀请注册
    //	public static final int NORMAL_EMAIL_INVITE_REG_XN 	= 100005;
    public static final int NORMAL_EMAIL_INVITE_REG_XN = 106103;
    //APP链接邀请注册
    //	public static final int APP_LINK_INVITE_REG_XN 		= 100006;
    public static final int APP_LINK_INVITE_REG_XN = 106102;
    //APP邮件邀请注册
    //	public static final int APP_EMAIL_INVITE_REG_XN 	= 100007;
    public static final int APP_EMAIL_INVITE_REG_XN = 106104;
    //高中链接邀请
    public static final int HS_LINK_INVITE_REG_XN = 100008;
    //百度注册
    public static final int BAIDU44_REG_XN = 100009;
    //猫游记注册
    public static final int MAOYOU_REG_XN = 100010;
    //三国注册
    public static final int SANGUO_REG_XN = 100011;
    //MSNTAB
    public static final int MSNTAB_VIEW = 100012;
    //完善资料注册
    public static final int FILLINFO_REG_VISIT = 100013;
    //开心开放注册访问
    public static final int KX_OPEN_REG_VISIT = 100014;

    //2.单步注册
    //标准注册
    public static final int STD_REG = 200000;
    //普通邮件邀请
    public static final int REG_EMAIL_INVITE = 200010;
    //单步注册,普通链接邀请
    public static final int REG_LINK_INVITE = 200020;
    //单步注册,APP邮件邀请
    public static final int REG_EMAIL_APP_INVITE = 200030;
    //单步注册,APP链接邀请
    public static final int REG_LINK_APP_INVITE = 200040;
    //单步注册,高中链接邀请
    public static final int REG_LINK_HS_INVITE = 200050;
    //第一次访问注册
    public static final int FRST_VISIT_REG = 200060;
    public static final int MAOYOU_VISIT_REG = 200070;
    public static final int SANGUO_VISIT_REG = 200080;
    public static final int MSN_TAB_REG = 200090;
    //完善资料注册
    public static final int KX_LOGIN_FILLINFO_REG = 200100;
    //开放注册
    public static final int KX_OPEN_REG = 200110;
    /**
     * 手机注册
     */
    public static final int MOBILE_REG = 200130;

    //分2步注册,标准注册
    public static final int STD_REG_STEP_1 = 200001;
    public static final int STD_REG_STEP_2 = 200002;
    //分2步注册,普通邮件邀请
    public static final int REG_EMAIL_INVITE_STEP_1 = 200011;
    public static final int REG_EMAIL_INVITE_STEP_2 = 200012;
    //分2步注册,普通链接邀请
    public static final int REG_LINK_INVITE_STEP_1 = 200021;
    public static final int REG_LINK_INVITE_STEP_2 = 200022;
    //分2步注册,APP邮件邀请
    public static final int REG_EMAIL_APP_INVITE_STEP_1 = 200031;
    public static final int REG_EMAIL_APP_INVITE_STEP_2 = 200032;
    //分2步注册,APP链接邀请
    public static final int REG_LINK_APP_INVITE_STEP_1 = 200041;
    public static final int REG_LINK_APP_INVITE_STEP_2 = 200042;
    //分2步注册,高中链接邀请
    public static final int REG_LINK_HS_INVITE_STEP_1 = 200051;
    public static final int REG_LINK_HS_INVITE_STEP_2 = 200052;
    //分2步注册,第一次访问注册,目前跟个人主页是一种情况
    public static final int FRST_VISIT_REG_STEP_1 = 200061;
    public static final int FRST_VISIT_REG_STEP_2 = 200062;

    //3.成功注册
    //非审核流程,直接成功
    public static final int NORMAL_REG_OK = 240001;
    //ID注册
    public static final int NORMAL_ID_REG_OK = 240007;
    //手机注册
    public static final int NORMAL_MOBILE_REG_OK = 240008;
    
    //进入审核流程，被审核通过
    public static final int CHECK_REG_OK = 240002;
    //邮箱已经被占用
    public static final int CHECK_REG_EMAIL_EXIST = 240003;
    //进入审核流程，但是审核失败
    public static final int CHECK_REG_FAILED = 240004;
    //进入审核流程，填写审核信息
    public static final int FILLINFO_CHECK_REG = 240005;
    /**
     * 手机审核_成功
     */
    public static final int MOBILE_AUDIT_OK = 240006;

    //4.激活
    //普通邮件链接激活
    public static final int NORMAL_MAIL_ACTIVE = 290011;
    //邮件邀请自动激活
    public static final int AUTO_MAIL_ACTIVE = 290021;
    /**
     * XID注册自动激活
     */
    public static final int AUTO_XID_ACTIVE = 290101;
    //通过邮箱向服务器发信激活
    public static final int SEND_MAIL_ACTIVE = 290031;
    /**
     * 手机下行验证码激活
     */
    public static final int DOWN_CODE_ACTIVE = 290041;
    /**
     * 手机上行指令激活
     */
    public static final int UP_CODE_ACTIVE = 290051;

    //5.其他普通动作
    public static final int UPLAOD_HEAD = 250001;
    public static final int UPLAOD_HEAD_SKIP = 250021;
    public static final int UPLAOD_HEAD_AUTO_SKIP = 250031;
    public static final int UPLAOD_HEAD_RESULT = 250011;
    public static final int IMPORT_MSN = 250002;
    public static final int IMPORT_MSN_ADD_FRIEND = 250012;
    public static final int IMPORT_MSN_SKIP = 250022;
    public static final int IMPORT_MSN_CIN = 250003;
    public static final int IMPORT_MSN_CIN_HAVE_FRIEND = 250013;
    public static final int IMPORT_MSN_CIN_ADD_FRIEND = 250014;
    public static final int IMPORT_MSN_CIN_SKIP = 250024;
    public static final int IMPORT_MSN_CIN_SKIP_NOTIN = 250034;
    public static final int IMPORT_MSN_ADDFRIENDS = 250004;
    public static final int IMPORT_MSN_CNOTIN = 250005;
    public static final int IMPORT_MSN_CNOTIN_HAVE_FRI = 250015;
    public static final int IMPORT_MSN_CNOTIN_SKIP = 250026;
    public static final int IMPORT_MSN_CNOTIN_INVITE = 250016;
    public static final int IMPORT_MSN_SENDMAIL = 250006;
    public static final int PREVIEW = 250007;
    public static final int PREVIEW_ADD_FRIEND = 250017;
    public static final int PREVIEW_SKIP = 250027;
    public static final int PREVIEW_AUTO_SKIP = 250037;
    public static final int FILL_INFO = 250008;
    public static final int FILL_INFO_SKIP = 250028;
    public static final int FILL_INFO_SUBMIT = 250018;
    public static final int FILL_INFO_STAGE = 250038;
    public static final int FILL_INFO_STAGE_SUBMIT = 250048;
    public static final int GUIDE = 250009;
    public static final int TEM_NOT_FOSTER = 250019;
    //高中领狗
    public static final int GUIDE_HS_ADD_DOG = 250029;
    public static final int GUIDE_HS_SKIP_FARM = 250059;
    public static final int GUIDE_HS_ADD_FARM = 250049;

    //邀请相关action定义,导入邮箱类
    public static final int INVITE_IMPORT_MAIL_SELECT = 260101;
    public static final int INVITE_IMPORT_MAIL_CONCACT = 260102;
    public static final int INVITE_IMPORT_MAIL_NOCONCACT = 260103;
    public static final int INVITE_IMPORT_MAIL_CONTACT_NOREG = 260104;
    public static final int INVITE_IMPORT_MAIL_CONTACT_REG = 260105;
    public static final int INVITE_IMPORT_MAIL_ADD_FRIEND = 260106;
    public static final int INVITE_IMPORT_MAIL_ADD_FIREND_SKIP = 260107;
    public static final int INVITE_IMPORT_MAIL_INPUT_OTHER_MAIL = 260108;
    public static final int INVITE_IMPORT_MAIL_SELECT_ALL = 260109;
    public static final int INVITE_IMPORT_MAIL_SELECT_PART = 260110;
    public static final int INVITE_IMPORT_MAIL_SEND_SKIP = 260111;
    public static final int INVITE_IMPORT_MAIL_SEND = 260112;
    //导入MSN类
    public static final int INVITE_IMPORT_MSNMAIL_SELECT = 260201;
    public static final int INVITE_IMPORT_MSNMAIL_CONCACT = 260202;
    public static final int INVITE_IMPORT_MSNMAIL_NOCONCACT = 260203;
    public static final int INVITE_IMPORT_MSNMAIL_CONTACT_NOREG = 260204;
    public static final int INVITE_IMPORT_MSNMAIL_CONTACT_REG = 260205;
    public static final int INVITE_IMPORT_MSNMAIL_ADD_FRIEND = 260206;
    public static final int INVITE_IMPORT_MSNMAIL_ADD_FIREND_SKIP = 260207;
    public static final int INVITE_IMPORT_MSNMAIL_INPUT_OTHER_MAIL = 260208;
    public static final int INVITE_IMPORT_MSNMAIL_SELECT_ALL = 260209;
    public static final int INVITE_IMPORT_MSNMAIL_SELECT_PART = 260210;
    public static final int INVITE_IMPORT_MSNMAIL_SEND_SKIP = 260211;
    public static final int INVITE_IMPORT_MSNMAIL_SEND = 260212;
    //导入foxmail类
    public static final int INVITE_IMPORT_FOXMAIL_SELECT = 260301;
    public static final int INVITE_IMPORT_FOXMAIL_CONCACT = 260302;
    public static final int INVITE_IMPORT_FOXMAIL_NOCONCACT = 260303;
    public static final int INVITE_IMPORT_FOXMAIL_CONTACT_NOREG = 260304;
    public static final int INVITE_IMPORT_FOXMAIL_CONTACT_REG = 260305;
    public static final int INVITE_IMPORT_FOXMAIL_ADD_FRIEND = 260306;
    public static final int INVITE_IMPORT_FOXMAIL_ADD_FIREND_SKIP = 260307;
    public static final int INVITE_IMPORT_FOXMAIL_INPUT_OTHER_MAIL = 260308;
    public static final int INVITE_IMPORT_FOXMAIL_SELECT_ALL = 260309;
    public static final int INVITE_IMPORT_FOXMAIL_SELECT_PART = 260310;
    public static final int INVITE_IMPORT_FOXMAIL_SEND_SKIP = 260311;
    public static final int INVITE_IMPORT_FOXMAIL_SEND = 260312;
    //直接发送email邀请类
    public static final int INVITE_DIRECT_SENDMAIL_SELECT = 260501;
    public static final int INVITE_DIRECT_SENDMAIL = 260502;
    //手机邀请类
    public static final int INVITE_MOBILE_SMS = 261101;
    public static final int INVITE_MOBILE_REGUSE = 261102;
    public static final int INVITE_MOBILE_NONE_REGUSE = 261103;

    //开心注册直接导入好友
    public static final int KX_REG_INVITE_FRIENDS = 261201;

    //变量定义
    public static final String ACTION_ID_KEY = "action_id";
    public static final String USER_EMAIL_KEY = "user_email";
    public static final String USER_ID_KEY = "user_id";
    public static final String IP_KEY = "ip_address";
    public static final String XNSESS_ID_KEY = "xnsess_id";
    public static final String USER_STAGE_KEY = "user_stage";
    public static final String DOMAIN_NAME_KEY = "domain_name";
    public static final String REFERER_NAME_KEY = "referer_name";
    public static final String INVITER_ID_KEY = "inviter_id";
    public static final String S1_KEY = "subsource";
    public static final String S2_KEY = "appid";
    public static final String S3_KEY = "s3";
    public static final String S4_KEY = "s4";


    /**
     * 高中狗狗注册ss
     */
    public static final String ssHsDog = "10708";
	public RegOprDefine() {
	}
}
