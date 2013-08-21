package xce.tripod.client;

/**
 * Entry常量表
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public interface EntryType {

    /** 主工程开始 */
    /** */
    Entry BIZ_LOGIN_INPUT = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_LOGIN_INPUT);

    /** */
    Entry BIZ_LOGIN_PAGE_INPROCEEDING = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_LOGIN_PAGE_INPROCEEDING);

    /** */
    Entry BIZ_LOGIN_WEAK_PASSWORD_WARNING = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_LOGIN_WEAK_PASSWORD_WARNING);

    Entry BIZ_PROFILE_TIMELINE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_PROFILE_TIMELINE);

    /** guide 好友原创 */
    Entry BIZ_GUIDE_FRIEND_ORIGINAL = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_GUIDE_FRIEND_ORIGINAL);

    /** 登陆三段式广告 */
    Entry BIZ_LOGIN_STEPS_AD = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_LOGIN_STEPS_AD);

    /** 三段式广告 */
    Entry BIZ_THREE_STEPS_AD = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_THREE_STEPS_AD);

    Entry BIZ_HOME_CMCC_ACT = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_HOME_CMCC_ACT);

    Entry BIZ_HOME_ACTIVITY_ENGINE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_HOME_ACTIVITY_ENGINE);

    /** 主工程结束 */

    /** ugc 分享审核 */
    Entry BIZ_SHARE_SHARE_AUDIT = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_SHARE_AUDIT);

    /** ugc blog 发布日志 */
    Entry BIZ_BLOG_NEW_ENTRY = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_BLOG_NEW_ENTRY);

    /** ugc blog 日志自动保存 */
    Entry BIZ_BLOG_AUTO_SAVE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_BLOG_AUTO_SAVE);

    /** ugc blog 日志 rss 是否导入 */
    Entry BIZ_BLOG_ISRSS = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_BLOG_ISRSS);

    /** 用于首页和个人主页 */
    Entry BIZ_BLOG_HOME = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_BLOG_HOME);

    /** 用于各种列表存储 */
    Entry BIZ_BLOG_LIST = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_BLOG_LIST);

    /** 用于日志相关好友和好友日志 */
    Entry BIZ_BLOG_FRIEND = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_BLOG_FRIEND);

    /** 用于日志评论存储 */
    Entry BIZ_BLOG_COMMENT = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_BLOG_COMMENT);

    /** 日志隐私 */
    Entry BIZ_BLOG_PRIVACY = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_BLOG_PRIVACY);

    /** 状态，热门聚焦 */
    Entry BIZ_STATUS_DOING_HOT = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_STATUS_DOING_HOT);

    /** 状态 */
    Entry BIZ_STATUS_DOING = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_STATUS_DOING);

    /** ugc 状态表情 */
    Entry BIZ_STATUS_DOING_UBB = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_STATUS_DOING_UBB);

    /** social wiki feed */
    Entry BIZ_SOCIAL_WIKI_FEED = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_SOCIAL_WIKI_FEED);

    /** app首页菜单相关需求 */
    Entry BIZ_APP_HOME_MENU = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_APP_HOME_MENU);

    Entry BIZ_APP_USER_RECENT = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_APP_USER_RECENT);

    /** app首页菜单相关需求 */
    Entry BIZ_APP_MENU_COUNTER = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_APP_MENU_COUNTER);

    /** 首页左侧列表的新手引导,用来存储用户是否查看新手引导 */
    Entry BIZ_APP_NEW_GUIDE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_APP_NEW_GUIDE);

    Entry BIZ_APP_EXPIRED_REQ = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_APP_EXPIRED_REQ);

    Entry BIZ_MENU_UGC_MENU = new Entry(NamespaceConstant.WEB_MENU,
            BusinessConstant.BIZ_MENU_UGC_MENU);

    Entry BIZ_MENU_SORT_MENU = new Entry(NamespaceConstant.WEB_MENU,
            BusinessConstant.BIZ_MENU_SORT_MENU);

    Entry BIZ_MENU_TOOL_APP_LIST = new Entry(NamespaceConstant.WEB_MENU,
            BusinessConstant.BIZ_MENU_TOOL_APP_LIST);

    Entry BIZ_MENU_TOOL_APP_WHITE_LIST = new Entry(NamespaceConstant.WEB_MENU,
            BusinessConstant.BIZ_MENU_TOOL_APP_WHITE_LIST);

    /** home页面活动 */
    Entry BIZ_HOME_ACTIVITY = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_HOME_ACTIVITY);

    /** socialgraph推荐 */
    Entry BIZ_SOCIALGRAPH_RCD = new Entry(NamespaceConstant.AD_SOCIALGRAPH,
            BusinessConstant.BIZ_SOCIALGRAPH_RCD);

    Entry BIZ_SOCIALGRAPH_FOFLR = new Entry(NamespaceConstant.AD_SOCIALGRAPH,
            BusinessConstant.BIZ_SOCIALGRAPH_FOFLR);

    /** socialgraph common推荐 */
    Entry BIZ_SOCIALGRAPH_COMMON = new Entry(NamespaceConstant.SOCIALGRAPH_COMMON,
            BusinessConstant.BIZ_SOCIALGRAPH_COMMON);

    Entry BIZ_SOCIALGRAPH_RCDPAGECF = new Entry(NamespaceConstant.SOCIALGRAPH_RCD_PAGE,
            BusinessConstant.BIZ_SOCIALGRAPH_RCDPAGECF);

    Entry BIZ_SOCIALGRAPH_USERPHOTOSTATS = new Entry(NamespaceConstant.SOCIALGRAPH_RCD_PAGE,
            BusinessConstant.BIZ_SOCIALGRAPH_USERPHOTOSTATS);

    /** photo home */
    Entry BIZ_PHOTO_HOME = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_PHOTO_HOME);

    /** photo default */
    Entry BIZ_PHOTO_DEFAULT = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_PHOTO_DEFAULT);

    /** photo model */
    Entry BIZ_PHOTO_MODEL = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_PHOTO_MODEL);

    /** photo list */
    Entry BIZ_PHOTO_LIST = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_PHOTO_LIST);

    /** photo upload */
    Entry BIZ_PHOTO_UPLOAD = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_PHOTO_UPLOAD);

    /** photo activity */
    Entry BIZ_PHOTO_ACTIVITY = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_PHOTO_ACTIVITY);

    /** photo constant */
    Entry BIZ_PHOTO_CONSTANT = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_PHOTO_CONSTANT);

    /** ugc 短链接 */
    Entry BIZ_SHORTURL_SHORTURL = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHORTURL_SHORTURL);

    /** 小站 开始 */
    Entry BIZ_ZHAN_SITE = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_SITE);

    Entry BIZ_ZHAN_ENTRY = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_ENTRY);

    Entry BIZ_ZHAN_ENTRY_COUNT = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ENTRY_COUNT);

    Entry BIZ_ZHAN_ENTRY_FIND = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ENTRY_FIND);

    Entry BIZ_ZHAN_FOLLOW_SITE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_FOLLOW_SITE);

    Entry BIZ_ZHAN_HOME_SITE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_HOME_SITE);

    Entry BIZ_ZHAN_USER_WALL = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_USER_WALL);

    Entry BIZ_ZHAN_HOT_FLAG = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_HOT_FLAG);

    Entry BIZ_ZHAN_GUIDE = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_GUIDE);

    Entry BIZ_ZHAN_ACT_WELCOME = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ACT_WELCOME);

    Entry BIZ_ZHAN_USER_FRIENDS = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_USER_FRIENDS);

    Entry BIZ_ZHAN_USER_TAGS = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_USER_TAGS);

    Entry BIZ_ZHAN_ONE_TAG = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ONE_TAG);

    Entry BIZ_ZHAN_REFERRED_TAG = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_REFERRED_TAG);

    Entry BIZ_ZHAN_TAG_RANK_TREND = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TAG_RANK_TREND);

    Entry BIZ_ZHAN_TAG_TOP_SITE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TAG_TOP_SITE);

    Entry BIZ_ZHAN_SORTED_TOP_SITE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_SORTED_TOP_SITE);

    Entry BIZ_ZHAN_TOP_TAGS = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TOP_TAGS);

    Entry BIZ_ZHAN_NEW_UGCS_INDEX = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_NEW_UGCS_INDEX);

    Entry BIZ_ZHAN_USER_FEED_BACK = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_USER_FEED_BACK);

    Entry BIZ_ZHAN_EDITOR = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_EDITOR);

    Entry BIZ_ZHAN_TAG_POST_NUM = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TAG_POST_NUM);

    Entry BIZ_ZHAN_TOP_ENTRY = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TOP_ENTRY);

    Entry BIZ_ZHAN_OWNER = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_OWNER);

    Entry BIZ_ZHAN_TAG_PHOTO = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TAG_PHOTO);

    Entry BIZ_ZHAN_SITE_RELATIVITY = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_SITE_RELATIVITY);

    Entry BIZ_ZHAN_SITE_TAG_GID_FI = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_SITE_TAG_GID_FI);

    Entry BIZ_ZHAN_SUGGEST_SITE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_SUGGEST_SITE);

    Entry BIZ_ZHAN_SITE_COLLECT_MESSAGE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_SITE_COLLECT_MESSAGE);

    Entry BIZ_ZHAN_GRADUATION_IDS = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_GRADUATION_IDS);

    Entry BIZ_ZHAN_TAG_FEED_GID = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_TAG_FEED_GID);

    Entry BIZ_ZHAN_TAG_V = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_TAG_V);

    Entry BIZ_ZHAN_TAG_ID = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_TAG_ID);

    Entry BIZ_ZHAN_ARCH_FEED = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ARCH_FEED);

    Entry BIZ_ZHAN_ARCH_TAG = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ARCH_TAG);

    Entry BIZ_ZHAN_ARCH_COUNT = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_ARCH_COUNT);

    Entry BIZ_ZHAN_USER_CARD = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_USER_CARD);

    Entry BIZ_ZHAN_REPORT_COMMENT = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_REPORT_COMMENT);

    Entry BIZ_ZHAN_GROUP_GID = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_GROUP_GID);

    Entry BIZ_ZHAN_BR = new Entry(NamespaceConstant.WEB_ZHAN, BusinessConstant.BIZ_ZHAN_BR);

    Entry BIZ_ZHAN_USER_SITE = new Entry(NamespaceConstant.WEB_ZHAN,
            BusinessConstant.BIZ_ZHAN_USER_SITE);

    /** 小站 结束 */

    /** antispam 开始 */
    Entry BIZ_ANTISPAM_ENCRYPT_LOGIN_STRING = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_ANTISPAM_ENCRYPT_LOGIN_STRING);

    Entry BIZ_ANTISPAM_RENREN_AQ = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_ANTISPAM_RENREN_AQ);

    Entry BIZ_ANTISPAM_AD_STATE = new Entry(NamespaceConstant.WEB_DEFAULT,
            BusinessConstant.BIZ_ANTISPAM_AD_STATE);

    /** antispam 结束 */

    Entry BIZ_OLYMPIC_SPREAD = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_OLYMPIC_SPREAD);

    /** UGC share 开始 */
    Entry BIZ_SHARE_ADMIN = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_ADMIN);

    Entry BIZ_SHARE_URL = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_URL);

    Entry BIZ_SHARE_LIST = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_LIST);

    Entry BIZ_SHARE_PUBLISHER = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_PUBLISHER);

    Entry BIZ_SHARE_XOA_URL = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_XOA_URL);

    Entry BIZ_SHARE_COLLECTION_LIST = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_COLLECTION_LIST);

    Entry BIZ_SHARE_SHARE_COMMENT = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_SHARE_COMMENT);

    Entry BIZ_SHARE_SHARE = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_SHARE);

    Entry BIZ_SHARE_FRIEND = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_FRIEND);

    Entry BIZ_SHARE_OLD_SHARE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_OLD_SHARE);

    Entry BIZ_SHARE_USER_SPACE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_USER_SPACE);

    Entry BIZ_SHARE_CONFIG = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_CONFIG);

    Entry BIZ_SHARE_HOT_SHARE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_SHARE_HOT_SHARE);

    Entry BIZ_SHARE_COUNT = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_SHARE_COUNT);

    /** UGC share 结束 */

    /** 新技术中心 开始 */
    /** 光影dv */
    Entry BIZ_NTC_GUANG_YING_DV = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_NTC_GUANG_YING_DV);

    /** K歌 */
    Entry BIZ_NTC_KOK = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_NTC_KOK);

    Entry BIZ_NTC_USER_PHOTO_ONE_DAY_A = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_NTC_USER_PHOTO_ONE_DAY_A);

    Entry BIZ_NTC_USER_PHOTO_ONE_DAY_B = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_NTC_USER_PHOTO_ONE_DAY_B);

    /** 新技术中心 结束 */

    /** UGC like 开始 */
    Entry BIZ_LIKE_CONTENT = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_LIKE_CONTENT);

    /** UGC like 结束 */

    /** Page 开始 */
    Entry BIZ_PAGE_DEFAULT = new Entry(NamespaceConstant.WEB_PAGE,
            BusinessConstant.BIZ_PAGE_DEFAULT);

    Entry BIZ_PAGE_TAG = new Entry(NamespaceConstant.WEB_PAGE, BusinessConstant.BIZ_PAGE_TAG);

    Entry BIZ_PAGE_FANS = new Entry(NamespaceConstant.WEB_PAGE, BusinessConstant.BIZ_PAGE_FANS);

    /** Page 结束 */

    /** XOA 开始 */
    Entry BIZ_XOA_QUOTA = new Entry(NamespaceConstant.WEB_UGC, BusinessConstant.BIZ_XOA_QUOTA);

    Entry BIZ_XOA_QUOTA_CACHE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_XOA_QUOTA_CACHE);

    /** XOA 结束 */

    Entry BIZ_EXPLORE_CACHE = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_EXPLORE_CACHE);
    
    Entry BIZ_YX_JINGYINGHUI_YOGA = new Entry(NamespaceConstant.WEB_UGC,
            BusinessConstant.BIZ_YX_JINGYINGHUI_YOGA);
}
