<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.logic.DogBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.model.DogBonusObjectModel"%>


<%@page import="com.xiaonei.reg.activity.dogbonus.logic.DogBonusAccountLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.model.DogBonusAccountRecordModel"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.logic.DogBonusAwardLogic"%>
<%@page import="com.xiaonei.reg.activity.dogbonus.model.DogBonusAwardModel"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><form action="">
<%@ include file="d_dog_head.inc"%>
id:<input name="id" value="" />

<input type="submit" />
<input type="hidden" name="run" value="true">
看最近的10条中奖记录请输入-1

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
    <td>id</td>
    <td>用户id</td>
    <td>奖品id</td>
    <td>奖品</td>
    <td>获得时间</td>
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
List<DogBonusAwardModel> list = null;
if(nid == -1){
    list = DogBonusAwardLogic.getInstance().select10(SnsAdapterFactory.getUserAdapter().get(81120));
}
else{
    User host = SnsAdapterFactory.getUserAdapter().get(nid);
    out.println(" host:"+host.getId()+" name:"+host.getName());
    list = DogBonusAwardLogic.getInstance().selectById(host);
}
for(DogBonusAwardModel m : list){
%>
<tr>
    <td><%=m.getId() %></td>
    <td><%=m.getUid() %></td>
    <td><%=m.getAward_id() %></td>
    <td><%=m.getAward_remark() %></td>
    <td><%=m.getAward_time() %></td>
</tr>
<%	
}
%>
</table>