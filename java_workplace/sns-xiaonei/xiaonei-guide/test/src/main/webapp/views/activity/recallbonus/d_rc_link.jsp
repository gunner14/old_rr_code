<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel"%>


<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAccountLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountRecordModel"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusInviteLogic"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><form action="">
<%@ include file="d_rc_head.inc"%>

<%
if(!Globals.DEBUG_ENV){
    out.println("");
    return;
}
%>
被邀请人id（即会收到邀请链接的人）:<input name="id" value="" />

<input type="submit" />
<input type="hidden" name="run" value="true">

<%
if(!"true".equals(request.getParameter("run"))){
    return;
}
%>
</form>

<%

String id = request.getParameter("id");
int nid = 0;

try{
	nid = Integer.parseInt(id);
}
catch(Exception e){
	
}

if(nid == 0){
	out.println("请输入id!");
	return;
}
User host = SnsAdapterFactory.getUserAdapter().get(nid);
int current = RecallBonusAccountLogic.getInstance().getAccountCurrent(host);
out.println(" host:"+host.getId()+" name:"+host.getName()+" current:"+current);
String link = RecallBonusInviteLogic.getInstance().getInviteeLink(host.getId(),Globals.urlGuide.toString());
out.println("link:<a href="+link+">"+link+"</a>");
%>
