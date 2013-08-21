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
请输入查询数量限制:<input name="id" value="" />

<input type="submit" />
<input type="hidden" name="run" value="true">
<%
if(!Globals.DEBUG_ENV){
    out.println("");
    return;
}
%>
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
	out.println("请输入数量限制!");
	return;
}
List<Integer> list = RecallBonusInviteLogic.getInstance().getAllInviteeId(nid,nid);
int index  = 0;
for(int i : list){
	out.println((index++)+":"+i+":"+"<a href=\"/views/activity/recallbonus/d_rc_link.jsp?id="+i+""+"&run=true\">"+"看Link"+"</a>"+"<br />");
}
%>
