<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<c:if test="${!empty recentMenuHome}">
	<article class="site-menu-apps" id= "appDynamic">
		<h3>应用动态</h3>
		<section class="site-menu-apps-recommend">
			<c:forEach var="item" items="${recentMenuHome}" begin="0" end="5">
				<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
				<c:set var="target" value="" />
				<c:set var="from" value="default"/>
				<c:if test="${!empty item.urlTarget}">
					<c:set var="target" value=" target=\"${item.urlTarget}\"" />
				</c:if>
				<c:choose>
					<c:when test="${fn:contains(item.sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
					<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
				</c:choose>
				<c:choose>
					<c:when test="${item.appId==42}"><c:set var="playNum" value ="5062378"/></c:when>
					<c:otherwise><c:set var="playNum" value="${item.installNum}"/></c:otherwise>
				</c:choose>
				<div class="apps-item" id="appsItem_${item.appId}" style="clear:both;">
					<div class="item-title">
						<span class="item-main">
							<a href="${sideNavUrl}${sp}origin=50031"${target} target="_blank">
								<img class="icon" width="16" height="16" src="${item.icon}" /> ${item.appName}
							</a>
						</span>
					</div>
					<c:choose>
					<c:when test="${item.playUserId==0}"></c:when>
					<c:otherwise><c:set var="from" value="play"/></c:otherwise>
					</c:choose>	
					<div class="item-detail" id = "itemDetail">
						<div class="item-main">
								<a href ="${sideNavUrl}&from=${from}" target="_blank" class="headWrap"><img src="${item.logo48}" class=""/><div class="mask"></div></a>
							<div class="appdesc"> 
								<a href="${sideNavUrl}&from=${from}" target="_blank" class="appname" title="${item.appName}">${item.appName}</a>
								<p><span>${playNum}</span>人在玩</p>
								<c:choose>
									<c:when	test="${item.playUserId==0}"><span>${item.category}</span></c:when>
									<c:otherwise><%--<a href="http://www.renren.com/${item.playUserId}" title="${item.playUserName}" target="_blank">${item.playUserShortName}</a><span> 正在玩</span>--%></c:otherwise>
								</c:choose>	
							</div>
						</div>
					</div>
				</div>
			</c:forEach>
		</section>
	</article>
</c:if>

