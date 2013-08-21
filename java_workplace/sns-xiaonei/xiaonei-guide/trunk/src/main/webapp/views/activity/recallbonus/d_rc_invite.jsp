<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel"%>


<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAccountLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountRecordModel"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusInviteLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusInviteModel"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><form action="">
<%@ include file="d_rc_head.inc"%>
邀请人id（即邀请别人来的人）:<input name="id" value="" />

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
<table>
<tr><td>类型说明：</td><td></td></tr>
<tr><td>接受邀请</td><td>1</td></tr>
<tr><td>拒绝邀请</td><td>2</td></tr>
<tr><td>默认写入 </td><td>0</td></tr>
</table>
</form>
    
<table>
<tr>
    <td>id</td>
    <td>邀请人</td>
    <td>被邀请人</td>
    <td>邀请时间</td>
    <td>接受标志</td>
    <td>接受时间</td>
</tr>
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
out.println(" host:"+host.getId()+" name:"+host.getName());
List<RecallBonusInviteModel> list = RecallBonusInviteLogic.getInstance().getByInviterId(host);

for(RecallBonusInviteModel m : list){
%>
<tr>
    <td><%=m.getId() %></td>
    <td><%=m.getInviter_id() %></td>
    <td><%=m.getInvitee_id() %></td>
    <td><%=m.getInvite_time() %></td>
    <td><%=m.getAccept_flag() %></td>
    <td><%=m.getAccept_time() %></td>
</tr>
<%	
}
%>
</table>