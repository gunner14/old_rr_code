<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<%-- 常规应用置顶 --%>
<c:forEach var="item" items="${requestScope.userMenu.topMenu}" begin="0" end="0">
	<c:set var="origin" value="50164" />
	<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
	<c:set var="target" value="" />
	
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
	
	<c:choose>
		<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
		<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
	</c:choose>
	
	<c:set var="canNotDel" value="${item.appId == 166613 || item.appId == 166607 || item.appId == 166615 || item.appId == 60 || item.appId == 166617 || item.appId == 166616 || item.appId == 101644}" />
	
	<c:set var="stats">
		<c:choose>
			<c:when test="${item.appId==45}">stats="V6Guideleft_status"</c:when> <%-- 状态 --%>
			<c:when test="${item.appId==166615}">stats="V6Guideleft_share"</c:when> <%-- 分享 --%>
			<c:when test="${item.appId==166614}">stats="V6Guideleft_music"</c:when> <%-- 音乐 --%>
			<c:when test="${item.appId==166613}">stats="V6Guideleft_album"</c:when> <%-- 相册 --%>
			<c:when test="${item.appId==166607}">stats="V6Guideleft_blog"</c:when> <%-- 日志 --%>
			<c:when test="${item.appId==166616}">stats="V6Guideleft_zhan"</c:when> <%-- 小站 --%>
			<c:when test="${item.appId==166617}">stats="V6Guideleft_xiaozu"</c:when> <%-- 小组 --%>
			<c:otherwise></c:otherwise>
		</c:choose>
	</c:set>
	
	<div class="apps-item" id="appsItem_${item.appId}">
		<div class="item-title">
			<span class="item-main">
				<a ${stats} href="${sideNavUrl}${sp}origin=${origin}"${target}<c:if test="${item.appId==60||item.appId==95003||item.appId==120019||item.appId==166607||item.appId==166613||item.appId==166614||item.appId==166615||item.appId==166616||item.appId==166617}"> ui-async="async"</c:if>>
					<c:choose>
						<c:when test="${!empty appPicItems[item.appId]}">
							<img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background-image:url(${applicationScope.urlSmallStatic}/imgpro/icons/new-app100.png);background-position:0 ${-appPicItems[item.appId]}px;" />
						</c:when>
						<c:otherwise>
							<img class="icon" width="16" height="16" src="${item.appIcon}" /> 
						</c:otherwise>
					</c:choose> 
					${item.appName}
				</a>
			</span>
		</div>
		<div class="item-editor">
			<span class="editor-button"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
			<span class="editor-operations">
				<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
				<c:if test="${!canNotDel}">
					<a href="#nogo" data-deluri="http://apps.renren.com/menu/removeTopApp" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
				</c:if>
			</span>
		</div>
	</div>
</c:forEach>

<%-- 其他推广需求置顶 --%>
<c:forEach var="item" items="${requestScope.userMenu.SecondTopMenuOut}" begin="0" end="0">
	<c:set var="origin" value="50173" />
	<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
	<c:set var="target" value="" />
	
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
	
	<c:choose>
		<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
		<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
	</c:choose>
	
	<c:set var="canNotDel" value="${item.appId == 166613 || item.appId == 166607 || item.appId == 166615 || item.appId == 60 || item.appId == 166617 || item.appId == 166616 || item.appId == 101644}" />
	
	<c:set var="stats">
		<c:choose>
			<c:when test="${item.appId==45}">stats="V6Guideleft_status"</c:when> <%-- 状态 --%>
			<c:when test="${item.appId==166615}">stats="V6Guideleft_share"</c:when> <%-- 分享 --%>
			<c:when test="${item.appId==166614}">stats="V6Guideleft_music"</c:when> <%-- 音乐 --%>
			<c:when test="${item.appId==166613}">stats="V6Guideleft_album"</c:when> <%-- 相册 --%>
			<c:when test="${item.appId==166607}">stats="V6Guideleft_blog"</c:when> <%-- 日志 --%>
			<c:when test="${item.appId==166616}">stats="V6Guideleft_zhan"</c:when> <%-- 小站 --%>
			<c:when test="${item.appId==166617}">stats="V6Guideleft_xiaozu"</c:when> <%-- 小组 --%>
			<c:otherwise></c:otherwise>
		</c:choose>
	</c:set>
	
	<div class="apps-item" id="appsItem_${item.appId}">
		<div class="item-title">
			<span class="item-main">
				<a ${stats} href="${sideNavUrl}${sp}origin=${origin}"${target}<c:if test="${item.appId==60||item.appId==95003||item.appId==120019||item.appId==166607||item.appId==166613||item.appId==166614||item.appId==166615||item.appId==166616||item.appId==166617}"> ui-async="async"</c:if>>
					<c:choose>
						<c:when test="${!empty appPicItems[item.appId]}">
							<img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background-image:url(${applicationScope.urlSmallStatic}/imgpro/icons/new-app100.png);background-position:0 ${-appPicItems[item.appId]}px;" />
						</c:when>
						<c:otherwise>
							<img class="icon" width="16" height="16" src="${item.appIcon}" /> 
						</c:otherwise>
					</c:choose> 
					${item.appName}
				</a>
			</span>
		</div>
		<div class="item-editor">
			<span class="editor-button"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
			<span class="editor-operations">
				<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
				<c:if test="${!canNotDel}">
					<a href="#nogo" data-deluri="http://apps.renren.com/menu/removeTopApp" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
				</c:if>
			</span>
		</div>
	</div>
</c:forEach>