<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>

<c:set var="origin" value="50301" />
<c:forEach var="item" items="${requestScope.userMenu.ugcMenu}" varStatus="vStat" begin="${begin}" end="${end}">
	<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
	<c:set var="target" value="" />
	
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
	<c:if test="${item.appId==215498 || item.appId == 212540}">
		<c:set var="target" value="" />
	</c:if><%-- 私信和小群、足迹当前页面打开 --%>
	<c:if test="${item.appId == 166616}">
		<%-- 小站新窗口打开 --%>
		<c:set var="target" value=" target=\"_blank\"" />
		<c:set var="sideNavUrl" value="http://zhan.renren.com/home?from=homeTab&id=${visiter.id}" />
	</c:if>
	
	<c:if test="${item.appId == 166617}">
		<%-- 小组新窗口打开 --%>
		<c:set var="target" value=" target=\"_blank\"" />
		<c:set var="sideNavUrl" value="http://xiaozu.renren.com" />
	</c:if>
	<c:if test="${item.appId==166613}"><%--相册--%>
		<c:set var="sideNavUrl" value="http://www.renren.com/photo/${visiter.id}/album/friends" />
	</c:if>
	<c:if test="${item.appId==45}"><%--状态--%>
		<c:set var="sideNavUrl" value="http://status.renren.com/status/?fromType=nomore" />
	</c:if>
	<c:choose>
		<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
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
	
	<c:set var="canNotDel" value="false" /><%--逻辑修改，取消不能删除的项目--%>
	<%--<c:set var="canNotDel" value="${item.appId == 166613 || item.appId == 166607 || item.appId == 166615 || item.appId == 60 || item.appId == 166617 || item.appId == 166616 || item.appId == 101644}" />--%>

	<c:set var="classify"><%--站内功能--%>
		<c:choose>
			<c:when test="${item.appId==166613 || item.appId==45 || item.appId==166614 || item.appId==166607 || item.appId==166617 || item.appId==166615 || item.appId==8 || item.appId==166616 || item.appId==212540 || item.appId==95003 ||item.appId==215494 || item.appId==215498 || item.appId == 212540}">instation</c:when>
			<c:otherwise></c:otherwise>
		</c:choose>
	</c:set>
	
	<c:choose>
		<c:when test="${item.appId==8&&presentNum!=0}"> <%--礼物商城--%>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a href="http://gift.renren.com/?origin=50115" target="_blank">
							<img class="icon" width="16" height="16" src="${item.appIcon}" />
							<span class="app-name">${item.appName}</span>
							<span class="apps-item-count" id="allAppsInvite">${presentNum}</span>
						</a>
					</span>
				</div>
				<div class="item-editor">
					<span class="editor-operations">
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation an-operation-add" title="移至常用"><img src="http://a.xnimg.cn/imgpro/v6/up.png" /></a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" data-classify="${classify}" onclick="this.blur();return false;" class="an-operation an-operation-del" title="移除"><img src="http://a.xnimg.cn/imgpro/v6/remove.png" /></a>
						</c:if>
					</span>
				</div>
			</div>	
		</c:when>
		<c:otherwise>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a href="${sideNavUrl}${sp}origin=${origin}" ${target} <c:if test="${item.appId==60||item.appId==95003||item.appId==120019||item.appId==166607||item.appId==166613||item.appId==166614||item.appId==166615||item.appId==215498||item.appId == 212540}"> ui-async="async"</c:if>>
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
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation an-operation-add" title="移至常用"><img src="http://a.xnimg.cn/imgpro/v6/up.png" /></a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" data-classify="${classify}" onclick="this.blur();return false;" class="an-operation an-operation-del" title="移除"><img src="http://a.xnimg.cn/imgpro/v6/remove.png" /></a>
						</c:if>
					</span>
				</div>
			</div>
		</c:otherwise>
	</c:choose>
</c:forEach>
