<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" begin="${begin}" end="${end}">
	<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
	<c:set var="target" value="" />
	<c:if test="${!empty item.urlTarget}">
		<c:set var="target" value=" target=\"${item.urlTarget}\"" />
	</c:if>
	<c:choose>
		<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
		<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
	</c:choose>
	<c:choose>
	<c:when test="${item.appId==147394}"></c:when><%-- 人人视频 置底去了 --%>
	<c:when test="${item.appId==21461 && !visiter.userStateInfo.guideDone && (visiter.stage==20||visiter.stage==30)}"></c:when><%-- 德州扑克置顶于guide的白领和大学用户 --%>
	<c:when test="${item.appId==99542}"> <%-- 人人喜欢 --%>
		<div class="apps-item" id="appsItem_${item.appId}">
			<div class="item-title">
				<span class="item-main">
					<a href="${sideNavUrl}${sp}origin=${origin}"${target}>
						<img class="icon" width="16" height="16" src="${item.appIcon}" />
						${item.appName}
					</a>
				</span>
			</div>
		</div>	
	</c:when>
	<c:when test="${item.appId==45}"><%-- 状态 --%>
		<div class="apps-item with-menu">
			<div class="item-title"><span class="item-main"><a stats="V5Homeleft_status" href="http://status.renren.com/?fromType=nomore&amp;from=homeleft" ui-async="async"><img class="icon" src="${item.appIcon}" /> ${item.appName}</a></span></div>
			<ul>
				<li><a stats="V5Homeleft_myStatus" href="http://status.renren.com/status?id=${visiter.id}&amp;from=homeleft" ui-async="async">我的状态</a></li>
				<li><a stats="V5Homeleft_focus" href="http://status.renren.com/status/hot/list" ui-async="async">人人聚焦</a></li>
			</ul>
		</div>
	</c:when>
	

	<c:when test="${!empty onelevelmap[item.appId]}"><%-- 如果在常驻列表 --%>
		<c:set var="changzhu_app_1" value="${onelevelmap[item.appId]}"/>
		<div class="apps-item<c:if test="${!empty twolevelmap[item.appId]}"> with-menu</c:if>" id="appsItem_${item.appId}">
			<div class="item-title">
				<span class="item-main">
					<a href="${sideNavUrl}${sp}origin=${origin}"${target} <c:if test="${changzhu_app_1.newWindowFlag==0}">ui-async="async"</c:if>>
						<img src="${item.appIcon}" class="icon" width="16" height="16" /> 
						${item.appName} 
					</a>
				</span>
				<%--如果有右链接 --%>
				<c:if test="${!empty changzhu_app_1.operationName}">
					<span class="item-operation"><a href="${changzhu_app_1.operationUri}">${changzhu_app_1.operationName}</a></span>
				</c:if>
			</div>
			<%--如果有子菜单 --%>
			<c:if test="${!empty twolevelmap[item.appId]}">
				<ul>
				<c:set var="changzhu_app_2" value="${twolevelmap[item.appId]}"/>
				<c:forEach var="item" items="${changzhu_app_2}">
					<li><a href="${item.menuUri}" ui-async="async">${item.menuName}</a></li>
				</c:forEach>
				</ul>
			</c:if>
		</div>
	</c:when>
	<c:when test="${item.appId==115147}"></c:when><%-- 小组 --%>
	<c:when test="${item.appId==60}"></c:when><%-- 音乐 --%>
	<c:when test="${item.appId==101362}"></c:when><%-- 校园频道 --%>
	<c:when test="${item.appId==110388 && visiter.stage==10}"></c:when><%-- 校花校草110388 --%>
	<c:when test="${item.appId==118523 && visiter.stage==10}"></c:when><%-- 桃花朵朵118523 --%>
	<c:when test="${item.appId==26}"></c:when><%-- 应用中心26 --%>
	<c:otherwise>
		<div class="apps-item" id="appsItem_${item.appId}">
			<div class="item-title">
				<span class="item-main">
					<a href="${sideNavUrl}${sp}origin=${origin}"${target}>
						<c:if test="${!noDel}">
							<span class="x-to-hide-holder"><span class="x-to-hide" onclick="delHomeApp(event,'${item.appId}')"></span></span>
						</c:if>
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
		</div>
	</c:otherwise>
	</c:choose>
</c:forEach>
