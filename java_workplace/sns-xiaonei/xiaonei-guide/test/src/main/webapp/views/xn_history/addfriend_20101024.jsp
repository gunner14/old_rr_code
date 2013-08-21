
<%@page import="java.util.List"%>
<%@page import="java.util.ArrayList"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
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
List<Integer> list = new ArrayList<Integer>();
list.add(303427216);
list.add(303427236);
list.add(303427276);
list.add(303428406);
list.add(303427160);
list.add(303427180);
list.add(303080430);
list.add(303080460);
list.add(303080470);
list.add(303080480);
list.add(303080510);
list.add(303080530);
list.add(303080540);
list.add(303080630);
list.add(303080640);
list.add(303080670);
list.add(303080680);
list.add(303080720);
list.add(303080730);
list.add(303081010);
list.add(303081050);
list.add(303081060);
list.add(303081070);
list.add(303081120);
list.add(303081130);
list.add(303081310);
list.add(303081330);
list.add(303081340);
list.add(303081360);
list.add(303081400);
list.add(303081410);
list.add(303235340);
list.add(303235400);
list.add(303235420);
list.add(303235620);
list.add(303235630);
list.add(303235680);
list.add(303235690);
list.add(303235700);
list.add(303235710);
list.add(303237781);
list.add(303237790);
list.add(303237801);
list.add(303237821);
list.add(303237830);
list.add(303237841);
list.add(303237851);
list.add(303237860);
list.add(303237861);
list.add(303237870);
list.add(303237880);
list.add(303246001);
list.add(303246011);
list.add(303246020);
list.add(303246031);
list.add(303246041);
list.add(303246051);

int cnt = list.size();
int cur = 0;
	for (int a = 0; a < cnt; a++) {
		int aid = list.get(a);
		for (int b = a + 1; b < cnt; b++) {
			int bid = list.get(b);
			//RegFriendShipLogic.getInstance().createFriendShip(aid, bid);
			String o = cur+++"a:"+aid+" b:"+bid+"<br />";
			out.println(o);
			Thread.sleep(1000);
			out.flush();
			System.out.println(o);
		}
	}
%>
