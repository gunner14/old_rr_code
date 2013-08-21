package com.xiaonei.platform.taglib.helper;

import javax.servlet.jsp.PageContext;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 抽取原来的<%@ include file="/inc/googleAnalytics.inc" %> 逻辑
 */

public class GoogleAnalyticsHelper {

    public static String help(PageContext pageContext, User host) {
        StringBuffer sb = new StringBuffer();
        sb.append("<script type=\"text/javascript\" ");
        sb.append("src=\"http://www.google-analytics.com/urchin.js\">");
        sb.append("</script>");
        sb.append("<script type=\"text/javascript\">");
        sb.append("try{");
        sb.append("_uacct = \"UA-877339-3\";");
        sb.append("urchinTracker();");
        sb.append("}catch(e){}");
        String theServerName = pageContext.getRequest().getServerName();
        //		String searchString ="class.xiaonei.com";
        String searchString = OpiConstants.urlClass.toString();
        if (theServerName != null && theServerName.trim().length() != 0
                && theServerName.contains(searchString)) {
            sb.append("try{");
            sb.append("_uff = 0;");
            sb.append("_uacct = \"UA-877339-4\"; ");
            sb.append("urchinTracker();");
            sb.append("}catch(e){}");
        }
        sb.append("try {");
        sb.append("if(GetCookie(\"newbie\") == \"1\") {");
        sb.append("_uff = 0;");
        sb.append("_uacct = \"UA-877339-2\";");
        sb.append("urchinTracker();");
        sb.append("}");
        sb.append("}catch(e){}");
        sb.append("try {");
        sb.append("_uff = 0;");
        sb.append("_uacct = \"UA-371132-13\";");
        //		sb.append("_udn=\"xiaonei.com\";");
        sb.append("_udn=\"").append(OpiConstants.domain).append("\";");
        sb.append("urchinTracker();");
        sb.append("}catch(e){}");
        sb.append("try {");
        sb.append("_uff = 0;");
        sb.append("_uacct = \"UA-877339-6\";");
        sb.append("urchinTracker();");
        sb.append("}catch(e){}");
        if (host != null && host.getStage() == 10) {
            sb.append("try {");
            sb.append("_uff = 0;");
            sb.append("_uacct = \"UA-877339-5\";");
            sb.append("urchinTracker();");
            sb.append("}catch(e){}");
        }
        if (host != null && host.getStage() == 30) {
            sb.append("try {");
            sb.append("_uff = 0;");
            sb.append("_uacct = \"UA-877339-6\";");
            sb.append("urchinTracker();");
            sb.append("}catch(e){}");
        }
        sb.append("</script>");
        return sb.toString();
    }

}
