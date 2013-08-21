
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