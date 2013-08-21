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
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.reg.activity.manager.logic.SendAwardLogic"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%><form action="">
<%@ include file="d_rc_head.inc"%>
几天前:<input name="id" value="<%=request.getParameter("id") %>" />
发多少:<input name="limit" value="<%=request.getParameter("limit") %>" />
confirm:<input name="con" value="<%=request.getParameter("con") %>" />
<input type="submit" />
<input type="hidden" name="run" value="true">
输入1表示查昨天的，输入2表示查前天的…懂了吗？
</form>

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
if(!"confirm".equals(request.getParameter("con"))){
	out.println("not conf");
    return;
}
%>

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
	out.println("nid 请输入正整数!");
	return;
}

String str_limit =  request.getParameter("limit");
int limit = 1;

try{
	limit = Integer.parseInt(str_limit);
	if(limit>1000){
		limit = 200;
	}
}
catch(Exception e){
    
}
if(limit == 0){
    out.println("limit 请输入正整数!");
    return;
}


List<RecallBonusAwardModel> list = null;

User host = SnsAdapterFactory.getUserAdapter().get(81120);
if(host == null){
	GuideLogger.printLog(" host null");
}
list = RecallBonusAwardLogic.getInstance().selectYesterdayAwardsWillSendWithLimit(host,nid,limit);
GuideLogger.printLog("Ready to Send:"+" day:"+nid+" limit:"+limit);
int i = 0;
for(RecallBonusAwardModel m : list){
    String ret = "-";
    if(m.getAward_id()>=1 && m.getAward_id()<=6){
    	ret = SendAwardLogic.getInstance().mysend(m.getAward_id(),m.getUid());
    }
	GuideLogger.printLog("Send:"+m.getAward_id()+"->"+m.getUid()+" ret:"+ret);
	RecallBonusAwardLogic.getInstance().sendAward(host,m.getId());
	GuideLogger.printLog("Update:"+m.getId());
	if(i+1 < limit)
		Thread.sleep(1000/limit);
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