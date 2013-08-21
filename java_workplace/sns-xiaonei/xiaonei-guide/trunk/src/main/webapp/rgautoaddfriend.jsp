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

	for (int a = 4000; a <= 4060; a++) {
		int aid = UserAdapter.getUser("renren_"+a,"renren.com").getId();
		System.out.print("a:"+aid+" + ");
		for (int b = a + 1; b <= 4060; b++) {
			int bid = UserAdapter.getUser("renren_"+b,"renren.com").getId();
			RegFriendShipLogic.getInstance().createFriendShip(aid, bid);
			System.out.print(bid+", ");
		}
		System.out.println();
	}
%>
