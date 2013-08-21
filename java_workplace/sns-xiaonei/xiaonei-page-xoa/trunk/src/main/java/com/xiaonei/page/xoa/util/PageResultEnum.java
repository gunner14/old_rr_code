package com.xiaonei.page.xoa.util;

/**
 * 为XOA返回结果提供的统一类型枚举类
 * 
 * @author yi.li@renren-inc.com since 2012-2-28
 * 
 */
public enum PageResultEnum {

    UNKNOWN(-1, "unknown exception"), //建议在返回未知错误时将 错误信息也返给调用方
    SUCC(0, "success"),
    
    // 权限相关
    USER_NOTEXIST(10, "user does not exist"), 
    PAGE_NOTEXIST(11, "page does not exist"), 
    AUTHORITY_ERR(12, "user doesn't have the authority"),

    // PAGE状态相关
    PAGE_CLOSED(20, "page closed"), 
    PAGE_BANED(21, "page forbidden to access"), ;

    private int code;

    private String msg;

    private PageResultEnum(int code, String msg) {
        this.code = code;
        this.msg = msg;
    }

    public int getCode() {
        return code;
    }

    public String getMsg() {
        return msg;
    }

}
