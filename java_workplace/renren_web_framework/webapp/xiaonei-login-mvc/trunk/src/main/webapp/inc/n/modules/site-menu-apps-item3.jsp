<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" begin="${begin}" end="${end}">
	<c:set var="origin">
		<c:choose>
			<c:when test="${visiter.userStateInfo.guideDone}">50115</c:when>
			<c:otherwise>103&guide</c:otherwise>
		</c:choose>
	</c:set>
	<c:set var="itemUrl" value="${item.sideNavUrl}" />
	<c:set var="target" value="" />
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
	<c:choose>
		<c:when test="${fn:contains(itemUrl, '?')}"><c:set var="sp" value="&" /></c:when>
		<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
	</c:choose>

	<div class="apps-item" id="appsItem_${item.appId}">
		<div class="item-title">
			<span class="item-main">
				<a href="${itemUrl}${sp}origin=${origin}" ${target}>
					<c:choose>
						<c:when test="${!empty appPicItems[item.appId]}">
							<img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background-image:url(${applicationScope.urlSmallStatic}/imgpro/icons/new-app100.png);background-position:0 ${-appPicItems[item.appId]}px;" />
						</c:when>
						<c:otherwise>
							<img class="icon" width="16" height="16" src="${item.appIcon}" /> 
						</c:otherwise>
					</c:choose> 
					<span class="app-name">${item.appName}</span>
				</a>
			</span>
		</div>
		<div class="item-editor">
			<span class="editor-operations">
				<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation an-operation-add" title="移至常用"><img src="http://a.xnimg.cn/imgpro/v6/up.png" /></a><a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation an-operation-del" title="移除"><img src="http://a.xnimg.cn/imgpro/v6/remove.png" /></a>
			</span>
		</div>
	</div>
</c:forEach>
