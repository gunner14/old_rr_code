package com.xiaonei.commons.interceptors.injection.messages;

import net.paoding.rose.web.Invocation;

/**
 * {@link PageMessage}
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */

public class PageMessage {

    private static Object[] EMPTY = new Object[0];

    private final Invocation inv;

    private final String type;

    private final String code;

    private String defaultMessage;

    private Object[] args = EMPTY;

    public PageMessage(Invocation inv, String type, String code, String defaultMessage) {
        this.type = type;
        this.code = code;
        this.defaultMessage = defaultMessage;
        this.inv = inv;
    }

    public String getType() {
        return type;
    }

    public String getCode() {
        return code;
    }

    public void setArgs(Object[] args) {
        this.args = args;
    }

    public String getDefaultMessage() {
        return defaultMessage;
    }

    public String getMessage() {
        String text = inv.getApplicationContext().getMessage(code, args, defaultMessage,
                inv.getResponse().getLocale());
        if (text == null) {
            text = defaultMessage = "[" + code + "]";
        }
        return text;
    }

    @Override
    public String toString() {
        return getMessage();
    }

}
