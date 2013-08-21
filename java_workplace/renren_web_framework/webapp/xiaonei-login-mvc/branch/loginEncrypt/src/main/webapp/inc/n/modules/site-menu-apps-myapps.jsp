<section class="site-menu-apps-others site-menu-apps-default">
	<c:set var="begin" value="0" />
	<c:set var="end" value="${myAppCount - 1}" />
	<%@ include file="/inc/n/modules/site-menu-apps-item3.jsp" %>
	<div class="site-menu-apps-more" style="display:none;">
		<c:set var="begin" value="${myAppCount}" />
		<c:set var="end" value="9" />
		<%@ include file="/inc/n/modules/site-menu-apps-item3.jsp" %>
		<div class="site-menu-apps-manage">
			<a href="http://app.renren.com/app/editApps?origin=50115" class="manage-my-apps-link" target="_blank"><img src="http://a.xnimg.cn/imgpro/v6/allapp.png" /> 我的全部应用</a>
		</div>
	</div>	
</section>
<div class="add-new-apps">
	<a href="http://app.renren.com/?origin=50115" class="add-new-apps-link" target="_blank">添加更多应用</a>
</div>
<div style="display:block;" class="site-menu-apps-toggle-show">
	<a class="to-show" href="#nogo" onclick="return false;" title="展开"></a>
</div>