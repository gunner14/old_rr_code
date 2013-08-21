<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel"%>


<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAccountLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountRecordModel"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAwardLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusAwardModel"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><form action="">
<%@ include file="d_rc_head.inc"%>
id:<input name="id" value="<%=request.getParameter("id") %>" />

<input type="submit" />
<input type="hidden" name="run" value="true">
输入1表示查昨天的，输入2表示查前天的…懂了吗？

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
<tr>
    <td>序号</td>
    <td>id</td>
    <td>用户id</td>
    <td>奖品id</td>
    <td>奖品</td>
    <td>获得时间</td>
    <td>领奖时间</td>
</tr>
<%

String id = request.getParameter("id");
int nid = -1;

try{
	nid = Integer.parseInt(id);
}
catch(Exception e){
	
}
if(nid == -1){
	out.println("请输入正整数!");
	return;
}
List<RecallBonusAwardModel> list = null;
if(nid == -1){
    //list = RecallBonusAwardLogic.getInstance().select10(SnsAdapterFactory.getUserAdapter().get(81120));
}
else{
    //User host = SnsAdapterFactory.getUserAdapter().get(nid);
    //out.println(" host:"+host.getId()+" name:"+host.getName());
    list = RecallBonusAwardLogic.getInstance().selectYesterdayAwards(SnsAdapterFactory.getUserAdapter().get(81120),nid);
}
int i = 0;
for(RecallBonusAwardModel m : list){
%>
<tr>
    <td><%=++i %></td>
    <td><%=m.getId() %></td>
    <td><%=m.getUid() %></td>
    <td><%=m.getAward_id() %></td>
    <td><%=m.getAward_remark() %></td>
    <td><%=m.getAward_time() %></td>
    <td><%=m.getAward_get_time() %></td>
</tr>
<%	
}
%>
</table>