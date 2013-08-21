<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.logic.DogBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.model.DogBonusObjectModel"%>


<%@page import="com.xiaonei.reg.activity.dogbonus.logic.DogBonusAccountLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.model.DogBonusAccountRecordModel"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.logic.DogBonusInviteLogic"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><form action="">
<%@ include file="d_dog_head.inc"%>

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
int current = DogBonusAccountLogic.getInstance().getAccountCurrent(host);
out.println(" host:"+host.getId()+" name:"+host.getName()+" current:"+current);
String link = DogBonusInviteLogic.getInstance().getInviteeLink(host.getId(),Globals.urlGuide.toString());
out.println("link:<a href="+link+">"+link+"</a>");
%>
