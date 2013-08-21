
<%@page import="java.util.ArrayList"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.xce.userpassport.UserPassportAdapter"%>
<%@page import="com.xiaonei.xce.userpassport.UserPassportInfo"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.member.service.VipClient"%>
<%@page import="com.xiaonei.member.service.VipService"%>
<%@page import="com.xiaonei.xce.userstate.UserStateInfo"%>
<%@page import="com.xiaonei.reg.common.users.UserAdapter"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.*"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="java.util.Date"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%
if(!Globals.DEBUG_ENV){
	out.println("are you a hacker? haha :)");
	return;
}
%>
<form action="">

id:<input name="regaccount" value="renren_r_" />
<input type="submit" />
<input name="run" value="true">
</form>

<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
String id = request.getParameter("regaccount");
int aid = Integer.parseInt(id);
List<Integer> list = new ArrayList<Integer>(); 
list.add(301920811);
list.add(301920791);
list.add(301920539);
list.add(301917920);
list.add(301916879);
list.add(301915743);
list.add(301896381);

	for(int bid : list){
		out.println("id:"+id+" with "+bid+" <br />");
		RegFriendShipLogic.getInstance().createFriendShip(aid, bid);
	}
%>
