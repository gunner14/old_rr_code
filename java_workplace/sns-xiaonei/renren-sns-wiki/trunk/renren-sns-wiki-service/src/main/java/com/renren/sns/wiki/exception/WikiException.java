package com.renren.sns.wiki.exception;

/**
 * wiki 自定义异常
 * 
 * @author yi.li@renren-inc.com since 2012-3-26
 *
 */
public class WikiException extends RuntimeException {

    /**
     * 
     */
    private static final long serialVersionUID = -3573627051227172643L;

    private int code; // 异常码

    private String displayMsg; // 可直接用于前端显示的信息

    private WikiExceptionType type; // 异常类型

    public WikiException(WikiExceptionType type) {
        this(type, "");
    }

    public WikiException(WikiExceptionType type, String msg) {
        super(msg);
        this.code = type.getCode();
        this.displayMsg = type.getDisplayMsg();
        this.type = type;
    }
    
    public WikiException(WikiExceptionType type, Throwable throwable){
        super(throwable);
        this.code = type.getCode();
        this.displayMsg = type.getDisplayMsg();
        this.type = type;
    }

    public int getCode() {
        return code;
    }

    public String getDisplayMsg() {
        return displayMsg;
    }

    public WikiExceptionType getType() {
        return type;
    }
}
