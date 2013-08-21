
<%@page import="com.xiaonei.platform.component.doing.home.DoingHome"%>
<%@page import="com.xiaonei.platform.component.doing.model.DoingUBB"%>
<%@page import="java.util.List"%>
<%@page import="java.lang.reflect.Field"%>
<%@page import="com.xiaonei.platform.component.doing.helper.impl.DoingReplaceHelperImpl"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%>
<%@page import="com.xiaonei.reg.guide.util.GuideUtil"%>
<%@page import="com.xiaonei.platform.core.model.UserName"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%><%@page import="com.xiaonei.xce.passport.TicketWebImpl"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@page import="com.xiaonei.xce.userright.UserRightInfo"%>
<%@page import="org.tuckey.web.filters.urlrewrite.UrlRewriteFilter"%>
<%@page import="com.xiaonei.reg.common.users.UserAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.BaseThreadUtil"%>
<%@page import="com.xiaonei.reg.register.dao.RegMobileWebInfoMapDAO"%>
<%@page import="com.dodoyo.opt.DodoyoResource"%>
<%@page import="com.xiaonei.reg.common.constants.IRegFromSource"%>
<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="com.xiaonei.reg.register.logic.pretreatment.*"%>
<%@ page import="com.dodoyo.login.logic.*"%>
<%@ page import="java.util.Date"%>
<%
int uid = 339488833;

DoingReplaceHelperImpl dr = new DoingReplaceHelperImpl();

//Field f = DoingReplaceHelperImpl.class.getField("allDuList");
Field[] fs = DoingReplaceHelperImpl.class.getDeclaredFields();
for(Field f : fs){
out.println("f:"+f.getName()+"<br />");
    if(f.getName().startsWith("allDu")){
        f.setAccessible(true);
List<DoingUBB> dul = (List<DoingUBB>)f.get(dr);

out.println("dul:"+dul+"<br />");
        
    }
}

List<DoingUBB> ubbList = DoingHome.getInstance().getDoingUBB();
out.println("ubbList:"+ubbList+"<br />");

%>