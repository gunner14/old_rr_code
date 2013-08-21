<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:set var="favorOrig" value="50119" />
<c:forEach var="item" items="${favList}" begin="0" end="19">
	
	<c:set var="itemUrl" value="${item.sideNavUrl}" />
	<c:set var="target" value="" />
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
	<c:if test="${item.appId == 166616}">
		<%-- 小站新窗口打开 --%>
		<c:set var="target" value=" target=\"_blank\"" />
		<c:set var="itemUrl" value="http://zhan.renren.com/home?from=homeTab&id=${visiter.id}" />
	</c:if>
	<c:if test="${item.appId == 166617}">
		<%-- 小组新窗口打开 --%>
		<c:set var="target" value=" target=\"_blank\"" />
		<c:set var="itemUrl" value="http://xiaozu.renren.com" />
	</c:if>
	<c:if test="${item.appId==166613}">
		<c:set var="itemUrl" value="http://www.renren.com/photo/${visiter.id}/album/friends" />
	</c:if>
	<c:if test="${item.appId==45}">
		<c:set var="itemUrl" value="http://status.renren.com/status/?fromType=nomore" />
	</c:if>
	<c:choose>
		<c:when test="${fn:contains(itemUrl, '?')}"><c:set var="sp" value="&" /></c:when>
		<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
	</c:choose>
	
	<c:set var="stats">
		<c:choose>
			<c:when test="${item.appId==45}">V6Homeleft_status</c:when> <%-- 状态 --%>
			<c:when test="${item.appId==166615}">V6Homeleft_share</c:when> <%-- 分享 --%>
			<c:when test="${item.appId==166614}">V6Homeleft_music</c:when> <%-- 音乐 --%>
			<c:when test="${item.appId==166613}">V6Homeleft_album</c:when> <%-- 相册 --%>
			<c:when test="${item.appId==166607}">V6Homeleft_blog</c:when> <%-- 日志 --%>
			<c:when test="${item.appId==166616}">V6Homeleft_zhan</c:when> <%-- 小站 --%>
			<c:when test="${item.appId==166617}">V6Homeleft_xiaozu</c:when> <%-- 小组 --%>
			<c:otherwise></c:otherwise>
		</c:choose>
	</c:set>
	<div class="apps-item" id="appsItem_${item.appId}">
		<div class="item-title">
				<span class="item-main"> <a stats="${stats}" ${target} href="${itemUrl}${sp}origin=${favorOrig}" <c:if test="${item.appId==45||item.appId==60||item.appId==95003||item.appId==120019||item.appId==141879||item.appId==166607||item.appId==166613||item.appId==166614||item.appId==166615}"> ui-async="async"</c:if>><img src="${item.appIcon}" class="icon"> ${item.appName}
								<c:if test="${item.appId==8&&presentNum!=0}"><span  class="apps-item-count" id="allAppsInvite">${presentNum}</span></c:if>		
			</a>
			</span>
		</div>
		<div class="item-editor">
				<span class="editor-button" <c:if test="${item.appId==8&&presentNum!=0}">style="right:29px;top:-29px;"</c:if>><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span> <span class="editor-operations"> <a href="#nogo" data-appid="${item.appId}" data-optype="remove" onclick="this.blur();return false;" class="an-operation">移出</a> <a href="#nogo" data-optype="order" onclick="this.blur();return false;" class="an-operation">排序</a>
			</span>
		</div>
	</div>
</c:forEach>
