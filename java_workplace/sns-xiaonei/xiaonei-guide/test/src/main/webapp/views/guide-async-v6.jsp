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
					<%@ include file="/views/guide/v6/body/guide_v6_body.jsp"%>	
				</div>
			</div>
		</div>
		<div id="sidebar2">
	      	<div class="home">
	      		<div class="home-sidebar"> 
					<%@ include file="/inc/n/modules/right-side-guide.inc" %>  
					<%@ include file="/views/guide/common/right/guide_manage_page.jsp" %>  
					<%@ include file="/views/guide/common/right/guide_footprint.jsp" %><%--最近来访部分--%>
					<c:if test="${newbieHelperStep == null}">
						<%@ include file="/views/guide/xf/common/right/guide_get_touch.jsp" %>
					</c:if>
					<%@ include file="/views/guide/xf/common/right/guide_xiaohua.jsp" %><%--符颖的校花校草--%> 
					<%@ include file="/views/guide/xf/common/right/guide_sponsors_widget.jsp" %>
					<%@ include file="/views/guide/xf/common/right/guide_frid_birthday.jsp" %>
                    <%@ include file="/views/guide/xf/common/right/guide_friend_list.jsp" %>
					<%@ include file="/pages/guide/right/guide_my_pages.jsp" %>   
					<%@ include file="/views/guide/right/guide_ex_star.jsp" %><%-- 中学网络人气之星 --%>
					<div class="ready-to-fix">					
						<%@ include file="/views/guide/xf/common/right/guide_pymk.jsp" %><%-- 好友推荐 --%>	
					</div>
	      		</div>
	      	</div>
		</div>  				            	
	</body>
</html>
