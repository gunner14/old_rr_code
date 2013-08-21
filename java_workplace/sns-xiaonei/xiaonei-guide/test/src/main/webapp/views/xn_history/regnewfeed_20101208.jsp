
<%@page import="com.xiaonei.platform.component.feed.api.model.GuideRecommend"%>
<%@page import="com.xiaonei.platform.component.feed.api.IGuideRecommend"%>
<%@page import="com.xiaonei.platform.component.feed.api.IGuideRecommend.GuideRecommendModel"%>
<%@page import="com.xiaonei.reg.guide.logic.search.GuideFriendNewsLogic"%>
<%@page import="com.xiaonei.reg.guide.action.a090729.A090729Util"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>
<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>


<%
String comment = "大学同学来了，看看你能不能成为TA的第一个好友吧！";
String network = "沈阳市和平区职业技术学院 软件学院 03级";
String url = "javascript:void(0);";

IGuideRecommend publish = GuideRecommend.getInstance();
GuideRecommendModel model = new GuideRecommendModel();
model.setComment(comment);
model.setNetwork(network);
model.setUrl(url);
int toID = 64131466; //程
publish.dispatch(81120, "银君鑫", toID, model);// toID：要发给谁。

%>


<%
int id = 0;
try{
	//id = Integer.parseInt(strid);
}
catch(Exception e){
	
}
if(id != 0){
	
	RequestRemoteLogger.getInstance().log(id, 1, "just_a_test");
	XceLogger myLogger = XceLoggerAdapter.getLogger("M@XceLogger0");
	String[] CATEGORIES = new String[] { "ActiveTrackXN", "action", "bucket" };
	String KEY = "GUIDE";
	String MSG_SEPARATOR = "\t";
	long TIME = new Date().getTime();
	//myLogger.addLog(CATEGORIES, new StringBuilder(KEY).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(1).append(MSG_SEPARATOR).append(0).append(MSG_SEPARATOR).append("just_a_test").toString());
	String msg = new StringBuilder().append(TIME).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(KEY).append(MSG_SEPARATOR).append("LEAD_just_a_test").toString();
	out.println(msg);
	myLogger.addLog(CATEGORIES, msg);
	
	RequestRemoteLogger.getInstance().log(id, 1, "just_a_test");
	XceLogger myLogger = XceLoggerAdapter.getLogger("M@XceLogger0");
	String[] CATEGORIES = new String[] { "ActiveTrackXN", "action", "bucket" };
	String KEY = "GUIDE";
	String MSG_SEPARATOR = "\t";
	long TIME = new Date().getTime();
	//myLogger.addLog(CATEGORIES, new StringBuilder(KEY).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(1).append(MSG_SEPARATOR).append(0).append(MSG_SEPARATOR).append("just_a_test").toString());
	String msg = new StringBuilder().append(TIME).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(KEY).append(MSG_SEPARATOR).append("LEAD_just_a_test").toString();
	out.println(msg);
	myLogger.addLog(CATEGORIES, msg);
	
	RequestRemoteLogger.getInstance().log(id, 1, "just_a_test");
	XceLogger myLogger = XceLoggerAdapter.getLogger("M@XceLogger0");
	String[] CATEGORIES = new String[] { "ActiveTrackXN", "action", "bucket" };
	String KEY = "GUIDE";
	String MSG_SEPARATOR = "\t";
	long TIME = new Date().getTime();
	//myLogger.addLog(CATEGORIES, new StringBuilder(KEY).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(1).append(MSG_SEPARATOR).append(0).append(MSG_SEPARATOR).append("just_a_test").toString());
	String msg = new StringBuilder().append(TIME).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(KEY).append(MSG_SEPARATOR).append("LEAD_just_a_test").toString();
	out.println(msg);
	myLogger.addLog(CATEGORIES, msg);
	
}

%>