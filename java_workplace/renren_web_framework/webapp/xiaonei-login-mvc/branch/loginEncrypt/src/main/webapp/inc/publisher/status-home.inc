<%@page pageEncoding="UTF-8" %>
<%@page import="com.xiaonei.platform.component.doing.model.Doing"%>
<%@page import="com.xiaonei.platform.component.doing.home.DoingHome"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@ taglib uri="http://www.xiaonei.com/tags-core" prefix="xn" %>
<div class="m-composer">	
	<div class="m-composer-header">
		<div class="headerimg" 
			<c:if test="${visiter.headUrl == '0/0/men_head.gif'||visiter.headUrl == '0/0/women_head.gif'}">title="请上传你的头像,更好的享受${domainIdentified["siteName"]}功能"</c:if> 
			<c:choose>
				<c:when test="${visiter.headUrl == '0/0/men_head.gif'||visiter.headUrl == '0/0/women_head.gif'}">
					onclick="javascript:window.location.href='http://head.upload.${applicationScope.domain}/Upload.do'" 
				</c:when>
				<c:otherwise>
					onclick="javascript:window.location.href='${applicationScope.urlPhoto}/getalbumprofile.do?owner=${userBasic.id}'" 
				</c:otherwise>
			</c:choose>			
			style="cursor:pointer; background-image:url(${userBasic.head_Url})">
			<c:if test="${visiter.headUrl == '0/0/men_head.gif'||visiter.headUrl == '0/0/women_head.gif'}">				
				<div style="background:#feeeb2 url(${applicationScope.urlSmallStatic}/imgpro/arrow/green-up.gif) 2px center no-repeat;*background-position:2px 3px;padding:3px 0 3px 14px;border-bottom:1px solid #ff9900;margin-top:78px;">
					<a href="http://head.upload.${applicationScope.domain}/Upload.do?from=log1">请上传真实头像</a>
				</div>			
			</c:if>	
		</div>		
		<div class="statuspanel">
			<%-- 下面的代码在status.inc里也用到，请保持一致 --%>
			<span class="myname">${userBasic.name}</span>
			<c:if test="${requestScope.isVip==1}">
				<a class="vip-level" href="http://i.${applicationScope.domain}/click.action?from=40041&url=%2F">${VipLevel}</a>
			</c:if>
			<%--已过期--%>
			<c:if test="${requestScope.isVip==0}">
				<a class="expired vip-level" href="http://i.${applicationScope.domain}/click.action?from=40041&url=%2F">${VipLevel}</a>
			</c:if>
			<span id="currentStatus" class="currentStatus"><c:choose><c:when test="${!empty requestScope.doing.content}">
				<%-- 手机版本改状态需要特殊的图标
						<c:if test="${doing.type == 4}">${doing.htmlContentForHome}</c:if>	
						<c:if test="${doing.type != 4}">${doing.htmlContentForHome}</c:if>
				 --%>				
				<c:if test="${doing.type==2||doing.type==3}"><%-- 这几种类型在IE6下加些料，以免不显示 --%>&nbsp;</c:if>${doing.htmlContentForHome}
				</c:when><c:otherwise>什么都没做</c:otherwise></c:choose></span>
				<span id="statusUpdateTime" class="changetime"><c:choose>
				 <c:when test="${requestScope.doneMinutes < 1}">刚刚更新</c:when>
					<c:when test="${requestScope.doneMinutes < 60}"><fmt:formatNumber value="${requestScope.doneMinutes}" pattern="#"/>分钟前</c:when>
					<c:when test="${requestScope.doneMinutes < 1440}"><fmt:formatNumber value="${requestScope.doneMinutes/60}" pattern="#"/>小时前</c:when>
					<c:when test="${requestScope.doneMinutes < 4320}"><fmt:formatNumber value="${requestScope.doneMinutes/1440}" pattern="#"/>天前</c:when>
					<c:otherwise>3天前</c:otherwise>
				 </c:choose>
				</span>
				<a href="${applicationScope.urlStatus}/getdoing.do?id=${visiter.id}">更多</a></span>
			<%@include file="/inc/publisher/status-common.inc" %>
		</div>
		<%@ include file="/inc/publisher/publisher_infoFin.inc"%>
	</div>
	<div id="publisherModule" class="m-composer-content" style="display:none;">
	</div>
</div>





		
