package com.renren.sns.wiki.exception;

/**
 * 自定义的WIKI异常类型
 * 
 * @author yi.li@renren-inc.com since 2012-3-26
 * 
 */
public enum WikiExceptionType {
    LOGIN_REQUIRED(10, "您还未登录，请登录之后再进行操作"),
    SERVER_ERROR(11, "服务端错误"), //controller里最外层catch抛出的异常,根据code可以快速定位异常的是controller抛出的
    PARAMS_ERROR(12, "传入参数错误"), //提交到服务端参数错误返回这个异常
    
    UNKNOWN_EXCEPTION(100, "未知错误"),
    WIKI_NOT_EXISTS(101, "您所查看的Wiki不存在"),
    WIKI_EXISTS(102, "WIKI已存在"),
    WIKI_CLOSE(103, "WIKI已关闭"),
    WIKI_FORBIDDEN(104, "WIKI已被封禁"),
    
    FEED_NOT_EXISTS(120, "FEED不存在"),
    
    USER_ALREADY_FOLLOWING(150, "您已经喜欢了该wiki"),
    USER_NOT_FOLLOWING(151, "您还未喜欢该wiki"),
    LIKE_DETAIL_NOT_FOUND(152, "获取喜欢信息失败"),
    
    SEARCH_NULL_KEYWORDS(160, "输入的查询条件为空"),
    
    WIKI_INFO_PROPERTY_LIST_NULL(200, "propertyList is NULL"),
    WIKI_UPDATE_VALUATION_EXCEPTION(210, "更新wiki评分失败"),
    
    WIKI_USERRANK_NOT_FOUND(250, "没有找到对应的积分排名"),

    PROHIBITED(300, "违禁词"),
    
    URL_NOT_FOUND(404, "请求不存在");

    private int code; // 异常码

    private String displayMsg; // 异常显示信息，标准是可以直接用于前端显示

    private WikiExceptionType(int code, String dispalyMsg) {
        this.code = code;
        this.displayMsg = dispalyMsg;
    }

    public int getCode() {
        return code;
    }

    public String getDisplayMsg() {
        return displayMsg;
    }
}
