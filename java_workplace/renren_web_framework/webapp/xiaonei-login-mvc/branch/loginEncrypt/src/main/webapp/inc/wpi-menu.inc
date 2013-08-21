<c:if test="${not empty requestScope.host}">
	<script type="text/javascript">
		var wpiMenuInfo = {recentMenu : [<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" varStatus="status" begin="0" end="8">{"id":"${item.appId}", "name":"${item.appName}", "icon":"${item.appIcon}", "href":"${item.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"}${!status.last ? ',' : ''}</c:forEach>],
			favoriteMenu : [<c:forEach var="item" items="${requestScope.userMenu.favoriteMenu}" varStatus="status">{"id":"${item.appId}", "name":"${item.appName}", "icon":"${item.appIcon}", "href":"${item.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"}${!status.last ? ',' : ''}</c:forEach>],
			user : {"id" : ${requestScope.host.id}},
			currentApp : [<c:if test="${!empty requestScope.currentApp}"> {"id":"${requestScope.currentApp.appId}","name":"${requestScope.currentApp.appName}", "icon":"${requestScope.currentApp.appIcon}","href":"${requestScope.currentApp.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"} </c:if>]
		};
	</script>
</c:if>


