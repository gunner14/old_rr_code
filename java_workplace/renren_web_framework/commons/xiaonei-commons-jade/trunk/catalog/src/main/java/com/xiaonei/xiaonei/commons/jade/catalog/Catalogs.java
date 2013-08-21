/*
 * OPI, all rights reserved.
 */
package com.xiaonei.xiaonei.commons.jade.catalog;

import com.xiaonei.xce.XceAdapter;

/**
 * 定义 {@link Catalogs} 用于在: @Dao(catalog="...") 属性中表示一个数据源。
 * 
 * @author han.liao
 */
public interface Catalogs {

    public static final String ACCOUNT = XceAdapter.DB_ACCOUNT;

    // 主页源 (小白、启容删)
    //public static final String LATEST_LOGIN_FRIENDS = XceAdapter.DB_LATEST_LOGIN_FRIENDS;

    // 活动用库(活动终于有自己的库啦～)
    public static final String ACTIVITY = XceAdapter.DB_ACTIVITY;

    /** 审核内容 */
    public static final String ADMIN_CONTENT = XceAdapter.DB_AUDIT_CONTENT;

    /** 审核人 */
    public static final String ADMIN_USER = XceAdapter.DB_AUDIT_USER;

    public static final String ALBUM = XceAdapter.DB_ALBUM;

    public static final String ALBUM_INFO = XceAdapter.DB_ALBUM_INFO;

    public static final String ALBUM_FRIEND = XceAdapter.DB_ALBUM_FRIEND;

    /** app */
    public static final String APP = XceAdapter.DB_APP;

    public static final String APP_LOG = XceAdapter.DB_APP_LOG;

    public static final String APP_USER = XceAdapter.DB_APP_USER;

    public static final String API_PAY = XceAdapter.DB_API_PAY;

    public static final String APPS_TYCOON = XceAdapter.DB_TYCOON;

    public static final String GODFATHER_INFO = XceAdapter.DB_GODFATHER;

    public static final String APP_USER_MENU = XceAdapter.DB_APP_USER_MENU;

    // 以下两个是 App 留言版和粉丝
    public static final String APP_COMMENTS = XceAdapter.DB_APP_COMMENTS;

    public static final String APP_FANS = XceAdapter.DB_APP_FANS;

    /** API Connect */
    public static final String APP_CONNECT_HASH = XceAdapter.DB_APP_CONNECT;// 散表

    public static final String APP_CONNECT = XceAdapter.DB_APP_CONNECT_EMAIL;

    /** APP 新增 2009-12-21 */
    public static final String APP_BBS = XceAdapter.DB_APP_BBS;

    public static final String APP_WORKFLOW = XceAdapter.DB_APP_WORKFLOW;

    /** 日志散表 */
    public static final String BLOG = XceAdapter.DB_BLOG;

    public static final String BLOG_INFO = XceAdapter.DB_BLOG_INFO;

    public static final String BLOG_STAT = XceAdapter.DB_BLOG_STAT;

    public static final String BROWSE = XceAdapter.DB_BROWSE;

    public static final String CARPARK = XceAdapter.DB_CARPARK;

    public static final String CARPARK_GAME = XceAdapter.DB_CARPARKGAME;

    public static final String CARPARK_LOG = XceAdapter.DB_CARPARK_LOG;

    /** 班级散表 */
    public static final String CLASS = XceAdapter.DB_CLASS;

    public static final String CLASS_INFO = XceAdapter.DB_CLASS_INFO;

    public static final String COMMON = XceAdapter.DB_COMMON;

    public static final String COUNT = XceAdapter.DB_COUNT;

    // 状态用
    public static final String DOING = XceAdapter.DB_DOING;

    public static final String DOING_INFO = XceAdapter.DB_BIZ_DOING_INFO;

    public static final String FRIEND_DOING = XceAdapter.DB_BIZ_FRIEND_DOING;

    // 活动(Event)
    public static final String EVENT = XceAdapter.DB_EVENT;

    public static final String FRIEND_DIST = XceAdapter.DB_FRIEND_DIST;

    public static final String GAOKAO = XceAdapter.DB_GAOKAO;

    /** 礼物 非散表 */
    public static final String GIFT_INFO = XceAdapter.DB_GIFT;

    // 将poke、going等表单移出来
    public static final String GOING_INFO = XceAdapter.DB_GOING_INFO;

    /** 留言散表 */
    public static final String GOSSIP = XceAdapter.DB_GOSSIP;

    public static final String GOSSIP_INFO = XceAdapter.DB_GOSSIP_INFO;

    // public static final String TRIBE = XceAdapter.DB_TRIBE;
    /** 群组散表 */
    public static final String GROUP = XceAdapter.DB_GROUP;

    public static final String GROUP_INFO = XceAdapter.DB_GROUP_INFO;

    public static final String GROUP_MEMBER = XceAdapter.DB_GROUP_MEMBER;

    public static final String GUIDE = XceAdapter.DB_GUIDE;

    public static final String IDSEQUENCE = XceAdapter.DB_IDSEQUENCE;

    // 存放即时mail
    public static final String IMMEDIATE_EMAIL = XceAdapter.DB_EMAIL;

    public static final String IMMSG = XceAdapter.DB_IMMSG;

    /* 了若指掌 */
    public static final String KNOWABC_INFO = XceAdapter.DB_KNOWABC_INFO;

    public static final String KNOWABC_NEWS = XceAdapter.DB_KNOWABC_NEWS;

    public static final String KNOWABC_SUB = XceAdapter.DB_KNOWABC_SUB;

    public static final String KNOWABC_INVESTIGATION = XceAdapter.DB_KNOWABC_INVESTIGATION;

    public static final String KNOWABC_SEND_INV = XceAdapter.DB_KNOWABC_SEND_INV;

    public static final String KNOWABC_VOTING_LOG = XceAdapter.DB_KNOWABC_VOTING_LOG;

    public static final String LOGGING = XceAdapter.DB_LOGGING;

    /* 蛊惑仔 */
    public static final String MOBWAR_INFO = XceAdapter.DB_MOBWAR;

    public static final String MOBWAR_NEWS = XceAdapter.DB_MOBWAR_NEWS;

    public static final String MOBWAR_FIGHT = XceAdapter.DB_MOBWAR_FIGHT;

    public static final String MOBWAR_REL = XceAdapter.DB_MOBWAR_REL;

    // public static final String MOBWAR_LIST = XceAdapter.DB_MOBWAR_LIST;

    // for wap
    public static final String MOBILE_STAT = XceAdapter.DB_MOBILE_STAT;

    public static final String MAIL_MMS = XceAdapter.DB_MAIL_MMS;

    public static final String WAP_PAY = XceAdapter.DB_WAP_PAY;

    public static final String WAP_REGISTER = XceAdapter.DB_WAP_REGISTER;

    /* mini city */
    public static final String MINICITY_INFO = XceAdapter.DB_MINICITY;

    /** market 非散表 */
    public static final String MARKET_INFO = XceAdapter.DB_MARKET;

    // public static final String ADMIN_ = XceAdapter.DB_ADMIN;

    public static final String MEMBER_GROUP = XceAdapter.DB_MEMBER_GROUP;

    /** 站内信散表 */
    public static final String MESSAGE = XceAdapter.DB_MESSAGE;

    public static final String MESSAGE_INFO = XceAdapter.DB_MESSAGE_INFO;

    public static final String MESSAGE_SESSION = XceAdapter.DB_MESSAGE_SESSION;

    /** 影评 */
    public static final String MOVIE_INFO = XceAdapter.DB_LOGGING;

    public static final String NOTIFY = XceAdapter.DB_MESSAGE_NOTIFY;

    public static final String NOTIFY_CONFIG = XceAdapter.DB_NOTIFY_CONFIG;

    /**
     * 请求中心
     */
    public static final String REQUEST_CENTER = XceAdapter.DB_REQUEST_CENTER;

    public static final String REQUEST_CENTER_APP = XceAdapter.DB_REQUEST_CENTER_APP;

    public static final String REQUEST_CENTER_SYSTEM = XceAdapter.DB_REQUEST_CENTER_SYSTEM;

    // 紧急审核专用(如台独，藏独)
    public static final String POLICE = XceAdapter.DB_POLICE;

    public static final String POP_USER = XceAdapter.DB_POP_USER;

    public static final String RELATION = XceAdapter.DB_RELATION;

    public static final String RELATION_INFO = XceAdapter.DB_RELATION_INFO;

    public static final String RELATION_BLOCK = XceAdapter.DB_RELATION_BLOCK;

    // 好友描述申请的综述表
    public static final String REQUEST_FRIEND_MEMO = XceAdapter.DB_REQUEST_FRIEND_MEMO;

    /** 搜索用的 */
    public static final String SEARCH = XceAdapter.DB_SEARCH;

    public static final String SELF_MESSAGE = XceAdapter.DB_SELF_MESSAGE;

    public static final String SHARE = XceAdapter.DB_SHARE;

    public static final String STAT = XceAdapter.DB_STAT;

    public static final String USER_APP = XceAdapter.DB_USER_APP;

    public static final String USER_COMMON = XceAdapter.DB_USER_COMMON;

    public static final String USER_CONFIG = XceAdapter.DB_USER_CONFIG;

    public static final String USER_DESC = XceAdapter.DB_USER_DESC;

    public static final String USER_MSN = XceAdapter.DB_USER_MSN;

    public static final String USER_SCORE = XceAdapter.DB_USER_SCORE;

    public static final String USER_SCORE_INFO = XceAdapter.DB_USER_SCORE_INFO;

    public static final String USER_VIEW_COUNT = XceAdapter.DB_USER_VIEW_COUNT;

    public static final String USER_NETWORK = XceAdapter.DB_USER_NETWORK;

    public static final String VAMPIRE = XceAdapter.DB_VAMPIRE;

    /** 做邮件打开和回点统计的数据源 */
    public static final String EMAIL_STAT = XceAdapter.DB_EMAIL_STAT;

    public static final String INVITE_LETTER = XceAdapter.DB_INVITE_LETTER;

    public static final String OP_LETTER = XceAdapter.DB_OP_LETTER;

    public static final String ACTIVATION_LETTER = XceAdapter.DB_ACTIVATION_LETTER;

    public static final String USER_TIME_EMAIL = XceAdapter.DB_USER_TIME_EMAIL;

    /** 统计好友申请历史记录的 */
    public static final String FRIEND_REQUEST = XceAdapter.DB_FRIEND_REQUEST;

    /** 注册邀请 */
    public static final String REG_INVITE = XceAdapter.DB_REG_INVITE;

    /** 注册邀请,开心从天津倒过来的数据供合并使用 是原开心的注册邀请 */
    public static final String KX_STAT = XceAdapter.DB_KX_STAT;

    public static final String KX_REG_INVITE = XceAdapter.DB_KX_REG_INVITE;

    public static final String INVITE_URL_PARAS = XceAdapter.DB_INVITE_URL_PARAS;

    public static final String KX_INVITE_URL_PARAS = XceAdapter.DB_KX_INVITE_URL_PARAS;

    /** PAGE用到的数据源 */
    public static final String PAGE = XceAdapter.DB_PAGE;

    public static final String PAGE_FANS = XceAdapter.DB_PAGE_FANS;

    public static final String FANS_PAGES = XceAdapter.DB_FANS_PAGES;

    public static final String PAGE_STAT = XceAdapter.DB_USER_PAGE_STAT;

    public static final String PAGE_SCORE = XceAdapter.DB_PAGE_SCORE;

    /** follow用到的数据源 */
    public static final String FOLLOW = XceAdapter.DB_FOLLOW;

    /** wap游戏用到的数据源 */
    public static final String MOBILE_GAME = XceAdapter.DB_MOBILE_GAME;

    /** wap游戏航海用到的数据源 */
    public static final String VOYAGE = XceAdapter.DB_VOYAGE;

    /** 常志昂在用：跑马甲的 */
    public static final String NEWBIE_PROFILE = XceAdapter.DB_NEWBIE_PROFILE;

    /** 安全中心 */
    public static final String SAFE_CENTER = XceAdapter.DB_SAFE_CENTER;

    /** 手机客户端申请通讯录备份数据表 */
    public static final String MOBILE_CLIENT = XceAdapter.DB_MOBILE_CLIENT;

    /** 热点分享 UGC */
    public static final String SHARE_INFO = XceAdapter.DB_SHARE_INFO;

    /** SNS部门在用，用户个人信息中的个人爱好 */
    public static final String USER_FOND = XceAdapter.DB_USER_FOND;// 用户个人信息中的个人爱好 user_fond

    /** UGC 问答 */
    public static final String QA_COMMON = XceAdapter.DB_QA_COMMON;

    /** Ugc专用id sequence */
    public static final String IDSEQ_UGC = XceAdapter.DB_IDSEQ_UGC;

    /** 高中项目数据库 */
    public static final String HIGH_SCHOOL_PAGE = XceAdapter.DB_HIGH_SCHOOL_PAGE;

    /** 手机农场数据库 */
    public static final String MOBILE_FARM = XceAdapter.DB_MOBILE_FARM;

    /** 积分系统新增 */
    public static final String USER_SCORE_LOG = XceAdapter.DB_USER_SCORE_LOG;

    public static final String USER_SCORE_TOTAL = XceAdapter.DB_USER_SCORE_TOTAL;

    public static final String USER_SCORE_TOTAL_INFO = XceAdapter.DB_USER_SCORE_TOTAL_INFO;

    public static final String USER_SCORE_AWARD_NEW = XceAdapter.DB_USER_SCORE_AWARD_NEW;

    /** 公共主页Page徽章系统 */
    public static final String PAGE_BIZ = XceAdapter.DB_PAGE_BIZ;

    /** SEO 项目新增 */
    public static final String SEO_COMPANY = XceAdapter.DB_SEO_COMPANY;

    public static final String PASSPORT_WAP = XceAdapter.DB_PASSPORT_WAP;

    /** 相册圈人数据源 */
    public static final String PAGE_RING = XceAdapter.DB_PAGE_RING;

    public static final String UGC_EVENT = XceAdapter.DB_UGC_EVENT;

    public static final String UGC_EVENT_INFO = XceAdapter.DB_UGC_EVENT_INFO;

    /** 游戏后台 */
    public static final String GAME_CENTER = XceAdapter.DB_GAME_CENTER;

    public static final String FINANCE_STATS = XceAdapter.DB_FINANCE_STATS;

    /** 安全中心窗项目 */
    public static final String SAFECENTER_CHUANG = XceAdapter.DB_SAFECENTER_CHUANG;

    public static final String FRIENDFINDER_USERLOGIN = XceAdapter.DB_FRIENDFINDER_USERLOGIN;
}
