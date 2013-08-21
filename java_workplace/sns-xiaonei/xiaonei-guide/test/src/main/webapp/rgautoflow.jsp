
<%@page import="com.xiaonei.reg.newguide.engine.config.UnitConfig"%>
<%@page import="com.xiaonei.reg.newguide.engine.config.MappingConfig"%>
<%@page import="com.xiaonei.reg.newguide.engine.config.FlowConfig"%>
<%@page import="com.xiaonei.reg.guide.action.a090729.MyLogger"%>
<%@page import="com.dodoyo.opt.DodoyoResource"%>
<%@page import="com.xiaonei.search2.condition.subcondition.UniversityCondition"%>
<%@page import="com.xiaonei.reg.guide.action.a090821.UserProfileUtil"%>
<%@page import="com.xiaonei.search2.SearchAdapter"%>
<%@page import="com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory"%>
<%@page import="com.xiaonei.search2.SearchViewFactory"%>
<%@page import="com.xiaonei.search2.model.IndexType"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.search2.SearchResult"%>
<%@page import="com.xiaonei.search2.condition.subcondition.WorkplaceCondition"%>
<%@page import="com.xiaonei.search2.condition.GuideCondition"%>
<%@page import="mop.hi.oce.adapter.AdapterFactory"%>
<%@page import="mop.hi.oce.domain.network.WorkplaceInfo"%>
<%@page import="com.xiaonei.search2.model.client.SearchView"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%>
<%@page import="org.apache.taglibs.standard.lang.jpath.encoding.HtmlEncoder"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.platform.core.model.UserBasic"%>
<%@page import="com.dodoyo.datum.client.DatumNetworkClient"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%>
<%@page import="java.util.List"%>
<%@page import="com.dodoyo.datum.client.DatumInfoClient"%>
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

FlowConfig.printAll();
MappingConfig.printAll();
UnitConfig.printAll();

%>