<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.reg.guide.logic.StepStatusLogic"%>
<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserDecisionDAO"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserIdentityDAO"%>
<%@ page import="java.util.Date"%>
<%out.println(" <br />"); %>

<%
if(!Globals.DEBUG_ENV){
	out.println("");
	return;
}
%>

<%out.println("To get an user's current state!" +" <br /><br />"); %>


<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.guide.root.logics.StepProcessLogic"%>
<%@page import="com.xiaonei.platform.core.model.UserBasic"%>
<%@page import="com.dodoyo.datum.model.RegionInfo"%>
<%@page import="com.dodoyo.datum.client.DatumInfoClient"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic"%>
<%@page import="com.xiaonei.reg.activity.manager.logic.SendAwardLogic"%><style>
	input {
		border:1px solid blue;
	}
	
	body {
		color:blue;
	}
</style>

1        礼券2个    -1
2        礼券5个    -1
3        礼券10个   -1
4        VIP7天免费体验   -1
5        VIP30天免费体验  -1
6        VIP3个月免费体验  -1

<form action="">

id:<input name="id" value="" />
aid:<input name="aid" value="" />

<input type="submit" />
<input type="hidden" name="run" value="true">
</form>

<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
String id = request.getParameter("id");
int nid = Integer.parseInt(id);
String aid = request.getParameter("aid");
int naid = Integer.parseInt(aid); 

SendAwardLogic.getInstance().mysend(naid,nid);

%>


<%out.println(" <br />"); %>
<%out.println("~The End~" +" <br />"); %>