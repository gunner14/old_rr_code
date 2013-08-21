<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei" %>
<% com.xiaonei.platform.core.model.User user = com.xiaonei.platform.core.opt.base.BaseThreadUtil.currentHost(request); %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html ui-mode="rewrite" xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/views/guide/xf/common/top/guide_xf_css_js.jsp"%>
		<%@ include file="/views/guide/xf/common/top/guide_xf_json.jsp"%>
  		<title>${domainIdentified["siteName_fillinfo"]} - <c:out value="${visiter.name}" escapeXml="true" /></title>
	</head>
	<body>
		<div id="content">
			<div class="home">
				<div class="home-body">  
					<%@ include file="/views/guide/xf/common/body/guide_vip_remind.jsp"%>										
					<div id="publisher_frame" class="publisher">						
						<%@ include file="/inc/publisher/publisher-show-home.inc" %>
					</div>
					<%@ include file="/views/guide/middle-school/body/guide_middle_school_body.jsp"%>	
				</div>
			</div>
		</div>
		<div id="sidebar2">
	      	<div class="home">
	      		<div class="home-sidebar"> 
	      			<%@ include file="/views/guide/right/message-box.jsp" %> 
					<%@ include file="/views/guide/common/right/guide_manage_page.jsp" %>  
					<%@ include file="/views/guide/common/right/guide_footprint.jsp" %><%--最近来访部分--%> 
					<%@ include file="/views/guide/xf/common/right/guide_xiaohua.jsp" %><%--符颖的校花校草--%> 
					<%@ include file="/views/guide/xf/common/right/guide_friend_list.jsp" %>									
					<%@ include file="/views/guide/xf/common/right/guide_get_touch.jsp"%>	
	      		</div>
	      	</div>
		</div>  				            	
	</body>
</html>
