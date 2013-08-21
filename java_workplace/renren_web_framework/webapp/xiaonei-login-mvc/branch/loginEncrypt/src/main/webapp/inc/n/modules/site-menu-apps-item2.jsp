<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" begin="${begin}" end="${end}">
	<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
	<c:set var="target" value="" />
	
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
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
	
	<c:choose>
		<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
		<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
	</c:choose>
	
	<c:set var="canNotDel" value="${item.appId == 166613 || item.appId == 166607 || item.appId == 166615 || item.appId == 60 || item.appId == 166617 || item.appId == 166616 || item.appId == 101644}" />
	
	<c:set var="stats">
		<c:choose>
			<c:when test="${item.appId==45}">stats="V6Homeleft_status"</c:when> <%-- 状态 --%>
			<c:when test="${item.appId==166615}">stats="V6Homeleft_share"</c:when> <%-- 分享 --%>
			<c:when test="${item.appId==166614}">stats="V6Homeleft_music"</c:when> <%-- 音乐 --%>
			<c:when test="${item.appId==166613}">stats="V6Homeleft_album"</c:when> <%-- 相册 --%>
			<c:when test="${item.appId==166607}">stats="V6Homeleft_blog"</c:when> <%-- 日志 --%>
			<c:when test="${item.appId==166616}">stats="V6Homeleft_zhan"</c:when> <%-- 小站 --%>
			<c:when test="${item.appId==166617}">stats="V6Homeleft_xiaozu"</c:when> <%-- 小组 --%>
			<c:otherwise></c:otherwise>
		</c:choose>
	</c:set>
	
	<c:choose>
		
		<c:when test="${item.appId==147394}"></c:when><%-- 人人视频 --%>
		<c:when test="${item.appId==26}"></c:when><%-- 应用中心 --%>
		<c:when test="${item.appId==2}"></c:when><%-- 老日志 --%>
		<c:when test="${item.appId==3}"></c:when><%-- 老相册 --%>
		<c:when test="${item.appId==6}"></c:when><%-- 老分享 --%>
		<c:when test="${item.appId==60}"></c:when><%-- 音乐 --%>
		<c:when test="${item.appId==147795}"></c:when><%-- 大街网 --%>
		<c:when test="${item.appId==8&&presentNum!=0}"> <%--礼物商城--%>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a ${stats}  href="http://gift.renren.com/?origin=50115" target="_blank">
							<img class="icon" width="16" height="16" src="${item.appIcon}" />
							${item.appName}
							<span class="apps-item-count" id="allAppsInvite">${presentNum}</span>
						</a>
					</span>
				</div>
				<div class="item-editor">
					<span class="editor-button" style="right:29px;top:-29px;"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
					<span class="editor-operations">
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
						</c:if>
					</span>
				</div>
			</div>	
		</c:when>
		<c:when test="${item.appId==166613}">
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a ${stats} ui-async="async" href="http://www.renren.com/photo/${visiter.id}/album/friends?origin=${origin}">
							<img class="icon" width="16" height="16" src="${item.appIcon}" />
							${item.appName}
						</a>
					</span>
				</div>
				<div class="item-editor">
					<span class="editor-button"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
					<span class="editor-operations">
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
						</c:if>
					</span>
				</div>
			</div>
		</c:when>
		
		<c:when test="${item.appId==99542}"> <%-- 人人喜欢 --%>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a ${stats} href="${sideNavUrl}${sp}origin=${origin}"${target}>
							<img class="icon" width="16" height="16" src="${item.appIcon}" />
							${item.appName}
						</a>
					</span>
				</div>
				<div class="item-editor">
					<span class="editor-button"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
					<span class="editor-operations">
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
						</c:if>
					</span>
				</div>
			</div>	
		</c:when>
		<c:when test="${item.appId==45}"><%-- 状态 --%>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title"><span class="item-main"><a ${stats} href="http://status.renren.com/status/?fromType=nomore&amp;from=homeleft" ui-async="async"><img class="icon" src="${item.appIcon}" /> ${item.appName}</a></span></div>
				<div class="item-editor">
					<span class="editor-button"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
					<span class="editor-operations">
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
						</c:if>
					</span>
				</div>
			</div>
		</c:when>
		
		<c:when test="${!empty onelevelmap[item.appId]}"><%-- 如果在常驻列表 --%>
			<c:set var="changzhu_app_1" value="${onelevelmap[item.appId]}"/>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a ${stats} href="${sideNavUrl}${sp}origin=${origin}"${target} <c:if test="${changzhu_app_1.newWindowFlag==0}">ui-async="async"</c:if>>
							<img src="${item.appIcon}" class="icon" width="16" height="16" /> 
							${item.appName} 
						</a>
					</span>
					<%--如果有右链接 --%>
					<c:if test="${!empty changzhu_app_1.operationName}">
						<span class="item-operation"><a href="${changzhu_app_1.operationUri}">${changzhu_app_1.operationName}</a></span>
					</c:if>
				</div>
				<div class="item-editor">
					<span class="editor-button"><a href="#" onclick="this.blur();return false;" class="icon">编辑</a></span>
					<span class="editor-operations">
						<a href="#nogo" data-appid="${item.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation">移动至常用</a>
						<c:if test="${!canNotDel}">
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
						</c:if>
					</span>
				</div>
			</div>
		</c:when>
		
		<c:otherwise>
			<div class="apps-item" id="appsItem_${item.appId}">
				<div class="item-title">
					<span class="item-main">
						<a ${stats} href="${sideNavUrl}${sp}origin=${origin}"${target}<c:if test="${item.appId==60||item.appId==95003||item.appId==120019||item.appId==166607||item.appId==166613||item.appId==166614||item.appId==166615}"> ui-async="async"</c:if>>
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
							<a href="#nogo" data-appid="${item.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation">移除</a>
						</c:if>
					</span>
				</div>
			</div>
		</c:otherwise>
		
	</c:choose>
</c:forEach>
