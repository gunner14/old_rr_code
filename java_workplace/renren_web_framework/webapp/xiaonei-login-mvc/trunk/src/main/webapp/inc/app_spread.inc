<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:if test="${!empty requestScope.bottomRecommendedApps}">
<!-- app-spread begin  -->
<link href="${applicationScope.urlSmallStatic}/css/app-spread.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<div class="app-spread" <c:if test="${appSpreadDivAlign == 'center' }">style="margin-left:76px;"</c:if> > 
<h3>更多精彩应用</h3>
<c:forEach var="item" items="${requestScope.bottomRecommendedApps}" >	
	<div class="app-spread-holder">
		<div class="app-spread-img">
			<a target="_blank" href="${applicationScope.urlApp}/apps/enterApp.do?origin=502&ref=rec_xnapp_bottom&id=${item.appId}"><img alt="" src="${item.logoSrc}" /></a>
		</div>
		<div class="app-spread-info">
			<a target="_blank" href="${applicationScope.urlApp}/apps/enterApp.do?origin=502&ref=rec_xnapp_bottom&id=${item.appId}">${item.appName }</a>
			<p>${item.description }</p>
			<p class="number">用户总数:<span>${item.totalUsers }</span></p>
		</div>
	</div>
</c:forEach>
</div><!-- app-spread end  -->
</c:if>

