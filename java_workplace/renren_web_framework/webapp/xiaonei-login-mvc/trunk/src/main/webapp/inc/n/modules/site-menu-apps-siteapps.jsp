<section class="site-menu-apps-in site-menu-apps-default">
	<c:set var="begin" value="0" />
	<c:set var="end" value="${siteAppCount - 1}" />
	<c:set var="siteAppsLength" value="${fn:length(requestScope.userMenu.ugcMenu)}" />
	<%@ include file="/inc/n/modules/site-app.jsp" %>
</section>
<div class="site-menu-apps-more" style="display:none;">
	<c:set var="begin" value="${siteAppCount}" />
	<c:set var="end" value="30" />
	<%@ include file="/inc/n/modules/site-app.jsp" %>
</div>
<div class="site-menu-apps-manage">
	<a href="http://app.renren.com/app/editApps?t=ugc&origin=50301" class="manage-my-apps-link" target="_blank" ><img src="http://a.xnimg.cn/imgpro/v6/allfunc.png" /> 全部站内功能</a>
</div>
<c:if test="${fn:length(requestScope.userMenu.ugcMenu) > siteAppCount}">
	<div style="display:block;" class="site-menu-apps-toggle-show">
		<a class="to-show" href="#nogo" onclick="return false;" title="展开"></a>
	</div>
</c:if>