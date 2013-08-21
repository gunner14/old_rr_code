<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@page import="com.xiaonei.sns.platform.core.opt.base.CookieManager"%>
<%@page import="com.xiaonei.xce.passport.PassportAdapterFactory"%>
<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<%
    response.setHeader(
            "P3P",
            "CP=\"NON DSP COR CURa ADMa DEVa TAIa PSAa PSDa IVAa IVDa CONa HISa TELa OTPa OUR UNRa IND UNI COM NAV INT DEM CNT PRE LOC\"");
    String p = CookieManager.getInstance().getCookie(request, "p");
    if (null != p) {
        int id = PassportAdapterFactory.getWebLogin().ticket2Id(p);
        if (id > 0) {
            String t = PassportManager.getInstance().createTicket(id, request);
            if (t != null) {
                CookieManager.getInstance().saveCookie(response, "t", t, -1, "/",
                        ".renren.com; p3p=1");
            }
        }
    }
%>
