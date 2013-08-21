package com.xiaonei.reg.guide.flows.oldactionsutil;

import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * ActionForward.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-8-27 
 */
public class ActionForward {

    private String toUrl = "@";
    private boolean isRedirect = false;

    public ActionForward(String string) {
        this.toUrl = string;
    }

    public ActionForward(String string, boolean isRedirect) {
        this(string);
        this.isRedirect = isRedirect;
    }

    public String toString() {
        String ret = toUrl;
        if(isRedirect){
            ret = "r:"+toUrl;
        }
        else{
            ret = "f:"+toUrl;
        }
        GuideLogger.printLog(" -> "+toUrl);
        return ret;
    }
}
