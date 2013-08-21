package com.renren.sns.wiki.enums;

/**
 * Wiki状态枚举类。这里列举出了一个Wiki所有可能的状态
 * 
 * @author yi.li@renren-inc.com since 2012-7-3
 * 
 */
public enum WikiStateType {
    // 暂定于10以下的状态为可访问状态，10以上的均为不可访问状态

    OPEN(0, "开放"), //  
    PAUSE(1, "暂停"), //  
    CLOSE(10, "关闭"), //  
    FORBIDDEN(11, "封禁"); //

    private int state;

    private String description;

    private WikiStateType(int state, String description) {
        this.state = state;
        this.description = description;
    }

    public int getState() {
        return state;
    }

    public String getDescription() {
        return description;
    }

    public static WikiStateType getStateType(int state) {
        WikiStateType[] values = values();
        for (WikiStateType value : values) {
            if (state == value.getState()) {
                return value;
            }
        }
        return null;
    }
}
