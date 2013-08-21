package com.renren.sns.wiki.utils.remotelog;

/**
 * 远程log的类型枚举类
 * 
 * @author yi.li@renren-inc.com since 2012-3-20
 * 
 */
public enum RemoteLogType {
    NONE("NONE"), 
    INDEX("INDEX"), 
    LIKE("LIKE"), 
    FEED_LIKE("FEED_LIKE"), 
    FEED_UNLIKE("FEED_UNLIKE"),
    INTEREST("INTEREST"),
    FOLLOW("FOLLOW"),
    CARD_DISPLAY("CARD_DISPLAY"),   // card display
    DISPLAY_FEED_AJAX("DISPLAY_FEED_AJAX"),   // display of hot feed
    FEED_DISPLAY("FEED_DISPLAY"), // display of latest feed
    USER_DISPLAY("USER_DISPLAY"), // display of users
    DISPLAY_USER_AJAX("DISPLAY_USER_AJAX"), // display of users
    CLICK_LOG("CLICK_LOG"),     // log of click
    COMMENT("COMMENT"), 
    HOME("HOME");

    private RemoteLogType(String typeName) {
        this.typeName = typeName;
    }

    private String typeName;

    public String getTypeName() {
        return typeName;
    }
}
