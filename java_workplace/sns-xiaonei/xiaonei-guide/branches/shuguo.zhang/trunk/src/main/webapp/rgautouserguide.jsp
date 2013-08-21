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
<script type="text/javascript" src="http://s.xnimg.cn/a9923/jspro/base.js"></script>    

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
<%@page import="com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil"%>
<%@page import="mop.hi.oce.domain.network.CollegeInfo"%>
<%@page import="mop.hi.oce.domain.network.ElementarySchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.HighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.JuniorHighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%>
<%@page import="mop.hi.oce.domain.network.WorkplaceInfo"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.WUserTimeAdapter"%>
<%@page import="com.xiaonei.platform.core.model.UserTime"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="com.xiaonei.platform.component.friends.home.FriendsHome"%>
<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%><style>
	input {
		border:1px solid blue;
	}
	
	body {
	   color:yellow;
	   background-color:#000000;
	}
</style>
<form action="">

id:<input name="id" value="" />

<input type="submit" />
<input type="hidden" name="run" value="true">
</form>

<a href="javascript:;" id="ttt">aaa</a>
<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>
<%
String id = request.getParameter("id");
int nid = Integer.parseInt(id);
User user = SnsAdapterFactory.getUserAdapter().get(nid);
if(user!= null){
	UserGuideAdapter.getInstance().push(nid,UserGuideAdapter.NEW_REG_ARRIVAL,null);
}

%>
<%out.println(" <br />"); %>
<%out.println("~The End~" +" <br />"); %>