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
List<Integer> list = DogBonusInviteLogic.getInstance().getAllInviteeId(nid,nid);
int index  = 0;
for(int i : list){
	out.println((index++)+":"+i+":"+"<a href=\"/views/activity/dogbonus/d_dog_link.jsp?id="+i+""+"&run=true\">"+"看Link"+"</a>"+"<br />");
}
%>
