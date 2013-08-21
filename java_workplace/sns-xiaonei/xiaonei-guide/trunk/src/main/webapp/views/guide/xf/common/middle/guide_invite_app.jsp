<%@ page pageEncoding="utf-8" %>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<c:if test="${!empty requestScope.inviteAppName}">
	<c:if test="${requestScope.disGuideInviteApp}">	
		<c:set var="H" value="他" />
		<c:if test="${requestScope.appInviter.gender == '女生'}"><c:set var="H" value="她" /></c:if>
		<div class="web-invite clearfix" id="guideInviteApp">
			<a class="user-pic" href="${applicationScope.urlWww}/profile.do?id=${requestScope.appInviter.id}">
				<img src="${requestScope.appInviter.tinyFullUrl}" alt="${requestScope.appInviter.name}">
			</a>
			<p>
				<a href="${applicationScope.urlWww}/profile.do?id=${requestScope.appInviter.id}">
					${requestScope.appInviter.name}
				</a>
				 正在邀请你和${H}一起去玩 <a href="${requestScope.inviteAppUrl}">${requestScope.inviteAppName}</a>
			</p>	
			<c:choose>
				<c:when test="${!empty requestScope.inviterOtherApps}">
					<span>${requestScope.appInviter.name}还在玩 : 
						<c:forEach var="item" items="${requestScope.inviterOtherApps}" begin="0" end="2" varStatus="status">
							<img src="${item.AppIcon}"> <a href="${item.AppUrl}">${item.AppName}</a>
						</c:forEach>
					</span>
				</c:when>
				<c:otherwise>
					<span>更多推荐 : 
						<c:forEach var="item" items="${requestScope.moreAppList}" begin="0" end="2" varStatus="status">
							<img src="${item.AppIcon}"> <a href="${item.AppUrl}">${item.AppName}</a>
						</c:forEach>
					</span>
				</c:otherwise>
			</c:choose>			
			<a href="javascript:;" class="close" onclick="if($('guideInviteApp'))$('guideInviteApp').hide();ajax_set_mem('guideInviteApp_${visiter.id}','21');">关闭</a>
			<a href="${requestScope.inviteAppUrl}" class="btn"><img src="${requestScope.inviteAppIcon}">进入${requestScope.inviteAppName}</a>
		</div>
	</c:if>
</c:if>