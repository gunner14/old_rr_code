<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel"%>


<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAccountLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountRecordModel"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><form action="">
<%@ include file="d_rc_head.inc"%>
id:<input name="id" value="" />

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
<table>
<tr><td>类型说明：</td><td></td></tr>
<tr><td>日登录</td><td>1</td></tr>
<tr><td>邀请别人</td><td>2</td></tr>
<tr><td>别人接受了我的邀请 </td><td>3</td></tr>
<tr><td>花钱了</td><td>4</td></tr>
</table>
<table>
<tr>
    <td>id</td>
    <td>用户id</td>
    <td>增量</td>
    <td>时间</td>
    <td>类型</td>
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
int current = RecallBonusAccountLogic.getInstance().getAccountCurrent(host);
out.println(" host:"+host.getId()+" name:"+host.getName()+" current:"+current);
List<RecallBonusAccountRecordModel> list = RecallBonusAccountLogic.getInstance().get(host);

for(RecallBonusAccountRecordModel m : list){
%>
<tr>
    <td><%=m.getId() %></td>
    <td><%=m.getUid() %></td>
    <td><%=m.getAct_amount() %></td>
    <td><%=m.getAct_time() %></td>
    <td><%=m.getAct_type() %></td>
</tr>
<%	
}
%>
</table>