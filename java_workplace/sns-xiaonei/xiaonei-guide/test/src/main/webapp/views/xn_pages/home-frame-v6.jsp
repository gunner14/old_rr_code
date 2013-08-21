<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.model.UserBasic"%>
<%@ page import="com.xiaonei.platform.core.opt.base.BaseThreadUtil"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:set var="channel" value="首页" />
<c:set var="INC_NEW_BASE" value="true" />
<c:set var="INC_NO_COMMON_CSS" value="true" />
<c:set var="INC_HOME_FRAME" value="true" />
<c:set var="isNewHome" value="true" />
<c:set var="isWhiteMouse" value="${requestScope.unifiedEntrance}" />
<c:set var="INC_PUBLISHER_IN_HEADER" value="true" />
<%
	User user = BaseThreadUtil.currentHost(request);
	UserBasic userBasic = (UserBasic)request.getAttribute("userBasic");
%>
<!DOCTYPE html>
<html>
	<head>		
		<%@ include file="/inc/n/head2.inc" %>
		<script>
			var the_temp_a = 1;
			XN.pageId = 'home';
			var stepJson = '${newbieHelperStep}';
			function imageOnLoad(img){
				if(img.height >= 300){
					img.parentNode.className = 'bigPic';
				}
			}
		</script>
		<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.share.js?ver=$revxxx$" type="text/javascript" ></script>
		<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$" type="text/javascript" ></script>
	</head>
	<body class="layout_home3cols">
		<%@ include file="/inc/n/header2.inc" %>
		<%@ include file="/inc/n/home-skin.inc" %>
		<div id="main">
			<div id="sidebar">
				<div class="site-menu-box box">
					<jsp:include page="/inc/n/modules/site-menu2-guide.jsp" />
				</div>
			</div>
			<div id="container2">
				<div id="main2">
					<div id="content">
					</div>
					<div id="sidebar2">
					</div>
				</div>
			</div>
		</div>
		<%@ include file="/inc/n/footer.inc" %>
		<c:if test="${!visiter.userStateInfo.guideDone}">
			
		</c:if>
		<c:if test="${requestScope.firstArriveGuide}">
			<script type="text/javascript">var page_id = "p51";</script>
			<%@ include file="/inc/guide_stat.inc" %>	
		</c:if>	
		<script>
			setTimeout(function(){
				window.delBySource = function(){};
			},5000);
		</script>
		<!--[if IE]><iframe id="HISTORY_ADAPTER" src="http://www.renren.com/ajaxhistory.html" style="display:none"></iframe><![endif]-->
	</body>
</html>
