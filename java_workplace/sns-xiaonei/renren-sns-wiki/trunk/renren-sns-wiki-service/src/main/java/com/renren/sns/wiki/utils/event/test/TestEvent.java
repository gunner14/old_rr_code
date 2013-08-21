package com.renren.sns.wiki.utils.event.test;

import com.renren.sns.wiki.utils.event.Event;

/**
 * @author yi.li@renren-inc.com since 2012-3-30
 *
 */
public class TestEvent implements Event {

    private static final long serialVersionUID = 237228291954241285L;

    private String msg;

    public TestEvent(String msg) {
        this.msg = msg;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

}
