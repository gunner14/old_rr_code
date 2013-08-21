package com.xiaonei.page.xoa.domain;


public enum PageXoaErrors {
    NOT_PAGE_ADMIN(0,"不是公共主页的管理员，无权进行此操作"),
    DB_ERROR(1,"数据库错误"),
    PAGE_NOT_EXIST(2,"公共主页不存在"),
    CONTENT_NUL(3,"内容为空"),
    AUDIT_ERROR(4,"审核错误"),
    FORBIDDEN_CONTENT(5,"您发布的信息含有违禁内容"),
    FORBIDDEN_PUBLISH(6,"您无权发布状态"),
    ERROR_WHEN_WRITE_DB(7,"写入db出错"),
    ERROR(8,"操作出错"),
    ERROR_WHEN_INVOKE_XOA(9,"调用第三方XOA服务失败"),
    USER_NOT_EXIST(10,"用户不存在"),
    AUTH_FAILED(11,"判断权限失败"),
    
    SEND_FEED_FAILED(20,"发送新鲜事失败"),
    
    TITLE_NUL(100,"标题为空"),
    
    ERROR_NO_TICKET(103,"");
    
    private int type;
    private String desc;
    
    private PageXoaErrors(int type, String desc) {
        this.type = type;
        this.desc = desc;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }
    
    
}
