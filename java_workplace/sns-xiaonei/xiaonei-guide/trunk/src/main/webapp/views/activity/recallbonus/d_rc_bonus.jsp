<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.logic.RecallBonusObjectLogic"%>
<%@page import="com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%><table>
<%@ include file="d_rc_head.inc"%>

<%
if(!Globals.DEBUG_ENV){
    out.println("");
    return;
}
%>

<tr>
    <td>id</td>
    <td>花费</td>
    <td>已送出</td>
    <td>最大值</td>
    <td>名称</td>
    <td>-</td>
</tr>
<%
List<RecallBonusObjectModel> list = RecallBonusObjectLogic.getInstance().select();
for(RecallBonusObjectModel m : list){
%>
<tr>
    <td><%=m.getId() %></td>
    <td><%=m.getBonus_cost() %></td>
    <td><%=m.getBonus_count() %></td>
    <td><%=m.getBonus_max() %></td>
    <td><%=m.getBonus_name() %></td>
    <td><%=m.getBonus_rate() %></td>
</tr>
<%	
}
%>
</table>