<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="com.xiaonei.xce.userpassport.UserPassportAdapter"%>
<%@ page import="com.xiaonei.xce.userpassport.UserPassportInfo"%>
<%@ page import="com.xiaonei.reg.common.constants.Globals"%>
<%@ page import="com.xiaonei.member.service.VipClient"%>
<%@ page import="com.xiaonei.member.service.VipService"%>
<%@ page import="com.xiaonei.xce.userstate.UserStateInfo"%>
<%@ page import="com.xiaonei.reg.common.users.UserAdapter"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.*"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="java.util.Date"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>
<%
int []  ids = new int []{
		333418108,
		334133612,
		334133759,
		334133807,
		334133853,
		334133904,
		334133943,
		334133982,
		334134025,
		334134081,
		334133832,
		334133894,
		334133944,
		334134033,
		334134103,  
		334134185,};
int cnt = 0;
for(int aindex = 0;aindex<ids.length;aindex++){
	for(int bindex = aindex+1;bindex<ids.length;bindex++){
		cnt++;
		System.out.println(cnt);
		try{
			RegFriendShipLogic.getInstance().createFriendShip(ids[aindex], ids[bindex]);
			out.println(cnt+":"+aindex+"["+ids[aindex]+"] <-> "+bindex+"["+ids[bindex]+"]<br />");
		}
		catch(Exception e){
			out.println("FFFFFF:"+cnt+":"+aindex+"["+ids[aindex]+"] <-> "+bindex+"["+ids[bindex]+"]<br />");
		}
	}
	
}
out.println("all:"+cnt);
%>
