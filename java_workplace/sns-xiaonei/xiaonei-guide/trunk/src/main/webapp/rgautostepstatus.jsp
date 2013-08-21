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
<%
//request.getRequestDispatcher("/views/function/profile.jsp").forward(request,response);
response.sendRedirect("/views/function/profile.jsp");
%>

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
<%@page import="com.xiaonei.platform.component.friends.home.FriendsHome"%><style>
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

<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
String id = request.getParameter("id");
int nid = Integer.parseInt(id);
String prefix = "&nbsp&nbsp&nbsp&nbsp id:"+nid;
String brmark = "==================================";
int step = StepStatusLogic.getStepStatus(nid);
out.println("STEP_INFO"+brmark+" <br />");
out.println(prefix+" step:"+step+" <br />");
List<String> list = StepStatusLogic.doTest(step);
for(String s : list){
	out.println(prefix+" "+s+" <br />");
}
boolean isguide = SnsAdapterFactory.getUserAdapter().get(nid).getUserState().isGuideDone();
out.println(prefix+" isguidedone?:"+isguide+" <br />");
out.println("ACCT_INFO"+brmark+" <br />");

User user = WUserAdapter.getInstance().get(nid);
UserBasic ub = WUserBasicAdapter.getInstance().get(nid);
out.println(prefix+" acct:"+user.getAccount()+" <br />");

String froms = RegFromSourceLogic.getFromSource(user);
out.println(prefix+" from:"+froms+" <br />");

Map<String,RegExtraMsg> map =  RegExtraMsgLogic.getInstance().loadExtraMsgMap(user);

int extraindex = 0;
for (String k : map.keySet()) {
	out.println(prefix+" reg_extra:"+(extraindex++)+" ["+map.get(k).getInput_key()+","+map.get(k).getInput_value()+"] <br />");
}
out.println(" <br />");

UserTime time = SnsAdapterFactory.getUserTimeAdapter().getUserTime(nid);
out.println(prefix+" time:"+time.getRegisterTime()+" -> "+time.getActivateTime()+" <br />");

int utype = StepProcessLogic.getInstance().getGuideUserType(user,froms);
out.println(prefix+" type:"+utype+" <br />");

int stage = user.getStage();
out.println(prefix+" stage:"+stage+" <br />");

out.println(prefix+" birth:"+ub.getBirthYear()+"-"+ub.getBirthMonth()+"-"+ub.getBirthDay()+""+" <br />");

int identity = GuideXFiveUseIdentityLogic.getInstance().getIdentity(nid);
int identityDao = GuideUserIdentityDAO.getInstance().getIdentity(nid);
out.println(prefix+" identity:"+identity+" -- in db:"+identityDao+" <br />");

int dec = GuideXFiveUserDecisionLogic.getInstance().getDecision(nid);
int decinDao = GuideUserDecisionDAO.getInstance().getDecision(nid);
out.println(prefix+" decision:"+dec+" -- dec in db:"+decinDao+" [1:good,2:bad,0:def]<br />");

String tinyurl = user.getTinyUrl();
out.println(prefix+" tinyUrl:"+tinyurl+" -> ");
out.println("<img src=http://hdn.xnimg.cn/photos/"+tinyurl+" />"+"|");
out.println("<img src="+user.getHeadFullUrl()+" />"+"|");
out.println("<img src="+user.getHeadUrl()+" />"+" <br />");
out.println("PROF_INFO"+brmark+" <br />");

%>

<table>
    <tr><td>

<%
out.println(prefix+" CurNetid::</td><td>"+user.getUniv()+" "+ub.getUnivName()+" <br />");
%>
    </td></tr>
<%
//UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(nid);
//RegionInfo re = DatumInfoClient.getInstance().getRegionInfo(user);
//out.println(prefix+" home:"+ub.getHomeCity()+" of "+ub.getHomeProvince()+":"+re.getCity_name()+" of "+re.getProvince_name()+" <br />");
CollegeInfo info = GuideXFiveUserProfileUtil.loadTechschool(user);
if(info != null){
	%>
	<tr><td>
	<%
	out.println(prefix+" College:</td><td>"+info.getCollegeId()+" "+info.getCollegeName()+" <br />");
	%>
	</td></tr>
	<%
}
RegionInfo region = GuideXFiveUserProfileUtil.loadCurrentRegion(user);
if(region!=null){
	%>
    <tr><td>
    <%
	out.println(prefix+" CurrentPlace:</td><td>"+region.getRegion_id()+" "+region.getProvince_name()+" "+region.getCity_name()+" <br />");
    %>
    </td></tr>
    <%
}

RegionInfo homeinfo = GuideXFiveUserProfileUtil.loadHomeRegion(user);
if(homeinfo!=null){
    %>
    <tr><td>
    <%
	out.println(prefix+" HomePlace:</td><td>"+homeinfo.getRegion_id()+" "+homeinfo.getProvince_name()+" "+homeinfo.getCity_name()+" <br />");
    %>
    </td></tr>
    <%
}

UniversityInfo uinfo = GuideXFiveUserProfileUtil.loadUniversity(user);
if(uinfo!=null){
    %>
    <tr><td>
    <%
	out.println(prefix+" University:</td><td>"+uinfo.getUniversityId()+" "+uinfo.getUniversityName()+" <br />");
    %>
    </td></tr>
    <%
}

HighSchoolInfo hinfo =  GuideXFiveUserProfileUtil.loadHighschool(user);
if(hinfo != null){
    %>
    <tr><td>
    <%
    out.println(prefix+" SeniorHigh:</td><td>"+hinfo.getHighSchoolId()+" "+hinfo.getHighSchoolName()+" <br />");
    %>
    </td></tr>
    <%
}

JuniorHighSchoolInfo jinfo = GuideXFiveUserProfileUtil.loadJuniorschool(user);
if(jinfo!=null){
    %>
    <tr><td>
    <%
    out.println(prefix+" JuniorHigh:</td><td>"+jinfo.getJunHighentarySchoolId()+" "+jinfo.getJunHighentarySchoolName()+" <br />");
    %>
    </td></tr>
    <%
}

ElementarySchoolInfo einfo = GuideXFiveUserProfileUtil.loadElementary(user);
if(einfo != null){
    %>
    <tr><td>
    <%
    out.println(prefix+" Elementary:</td><td>"+einfo.getElementarySchoolId()+" "+einfo.getElementarySchoolName()+" <br />");
    %>
    </td></tr>
    <%
}

WorkplaceInfo winfo = GuideXFiveUserProfileUtil.loadWorkplace(user);
if(winfo!=null){
    %>
    <tr><td>
    <%
	out.println(prefix+" Work:</td><td>"+winfo.getWorkplaceId()+" "+winfo.getWorkplaceName()+" <br />");
    %>
    </td></tr>
    <%
}
%>
</table>

<%out.println(" <br />"); %>
<%
List<Integer> idList = FriendsHome.getInstance().getApplyFriendList(user.getId());
out.println(prefix+"ApplyList: "+idList.size()+"<br/>");
for (int aid:idList){
	
}

%>
<%out.println(" <br />"); %>
<%out.println("~The End~" +" <br />"); %>