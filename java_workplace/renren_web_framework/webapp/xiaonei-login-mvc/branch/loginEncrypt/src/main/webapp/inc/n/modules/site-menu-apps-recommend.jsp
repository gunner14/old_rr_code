<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<c:if test="${!empty recentMenuHome}">
	<article class="site-menu-apps">
		<h3>推荐应用</h3>
		<section class="site-menu-apps-recommend">
			<c:forEach var="item" items="${recentMenuHome}" begin="0" end="2">
				<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
				<c:set var="target" value="" />
				<c:if test="${!empty item.urlTarget}">
					<c:set var="target" value=" target=\"${item.urlTarget}\"" />
				</c:if>
				<c:choose>
					<c:when test="${fn:contains(item.sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
					<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
				</c:choose>
				<div class="apps-item" id="appsItem_${item.appId}">
					<div class="item-title">
						<span class="item-main">
							<a href="${sideNavUrl}${sp}origin=50031"${target}>
								<img class="icon" width="16" height="16" src="${item.appIcon}" /> ${item.appName}
							</a>
						</span>
					</div>
				</div>
			</c:forEach>
		</section>
	</article>
</c:if>