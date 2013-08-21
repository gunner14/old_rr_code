<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn"%>
<%@ page import="com.xiaonei.platform.component.tools.UserUtil" %>
<%
com.xiaonei.platform.core.model.User user = null;
%>
<%
try {
	user = (com.xiaonei.platform.core.model.User)request.getAttribute("host");	
} catch(Exception e) {
}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
	<c:set var="INC_COMMON_CSS" value="true" />
    <%@ include file="/inc/headpro.inc"%>
	<%@ include file="/pages/register/guide_css_js.jsp"%>
  		<title>${domainIdentified["siteName_fillinfo"]} - ${requestScope.host.name}</title>
	</head>
	<body id="dashboardPage" class="home" g="999_999">
		<c:set var="cleanpage" value="ture" />&nbsp;  
		<%@ include file="/inc/header.inc"%>
		<%@ include	file="/inc/wrapper.inc"%>
		<%@ include file="/pages/register/guide_app_menu.jsp"%><%--应用菜单 --%>	
		<div class="main-page">
			<div class="home-content-holder clearfix">
				<div class="home-body">
	  				<%@ include file="/pages/register/guide_hs_info.jsp" %><%--公告部分 --%>
      				<div class="xn-publisher">
						<%@ include file="/inc/publisher/status-home.inc" %>
					</div>					
					<%@ include file="/pages/register/guide_news_feed.jsp"%><%--新鲜事 --%>
        			<div class="news-feed hot-feed">
          				<%@ include file="/pages/register/guide_hs_hotAlbum.jsp" %>
          				<%@ include file="/pages/register/guide_hs_hotBlog.jsp" %>
          				<%@ include file="/pages/register/guide_hs_hotVudio.jsp" %><%--视频，音乐位置调整 --%>						
					</div>	
					<%@ include file="/pages/register/guide_hs_hotApp_mntOfEdu.jsp" %><%--热门应用 --%> 				      				
     				<%@ include file="/pages/register/guide_hs_star.jsp" %><%--高中人气之星 end--%>      				
				</div>
				<div class="home-sidebar">
      				<div class="rounded-box sidebar-holder">
        				<div class="corner tl">
          					<div class="corner tr">
            					<div class="corner bl">
              						<div class="corner br">
                						<div class="corner-body">
                							<div class="side-item selected">          										
          										<%@include file="/pages/register/guide_hs_footprint.jsp" %><%--最近来访部分--%>          										 
												<%@include file="/pages/register/guide_hs_rightGames_mntOfEdu.jsp" %>
                							</div>
                						</div>
              						</div>
            					</div>
          					</div>
        				</div>
      				</div>
    			</div>
			</div>
		</div>
		<%@ include file="/inc/footer_guide_hs.inc"%>
	</body>
</html>
