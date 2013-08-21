<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>	

<div id="header">
<div class="header-wrapper">
	<div class="site-header clearfix">
		<div class="user-info clearfix">
			<a href="http://photo.renren.com/getalbumprofile.do?owner=${visiter.id}" class="avatar">
				<img src="${userBasic.tiny_Url}" width="50" height="50" />
			</a>
			<div class="info">
				<div class="user-name">
					<a href="${applicationScope.urlWww}/profile.do?id=${visiter.id}" class="name" title="${visiter.name}">${fn:substring(visiter.name, 0, 6)}</a>
				</div>
				<c:choose>
					<c:when test="${host.page}">
						<div style="position: relative; top: -2px;" class="page-admin-wrapper">
							<a class="link" href="http://page.renren.com/${host.id}/admin/"><img src="http://a.xnimg.cn/imgpro/page/page-admin-left.png" /></a>
						</div>
					</c:when>
					<c:otherwise>
						<div class="icons-grade">
							<span class="icons">
								<c:if test="${requestScope.isVip==1}">
									<a style="${requestScope.homeVipIcon}" class="vip-level" href="${applicationScope.urlI}/icon" title="点击更换VIP图标">${VipLevel}</a>
								</c:if>
								<c:if test="${requestScope.isVip==0}">
									<a class="expired vip-level" href="${applicationScope.urlI}/icon" title="点击更换VIP图标">${VipLevel}</a>
								</c:if>
							</span>
							<span class="live-days-grade">
								<a href="http://sc.renren.com/scores/myscore">${loginCDays}天</a>
							</span>
						</div>
					</c:otherwise>
				</c:choose>
			</div>
		</div>
		<div id="global-publisher" class="global-publisher-box">
			<jsp:include page="/inc/n/modules/global-publisher.jsp" />
		</div>
		<div class="ugc-operations clearfix">
			<a stats="V6Pub_uploadphoto" class="uo-upload-photo flashUploader" href="#nogo" onclick="return false;">传照片</a>
			<a stats="V6Pub_compose" class="uo-new-blog" href="http://blog.renren.com/NewEntry.do">写日志</a>
		</div>
	</div>
	<c:choose>
		<c:when test="${host.page}"></c:when>
		<c:otherwise>
			<div class="site-header-tips-holder">
				<div class="user-info-grade">
					<span class="tip-arrow-top"></span>
					<div class="tip-content">
						<c:if test="${!empty myLevel}">
							<a class="level" href="http://sc.${applicationScope.domain}/scores/myscore"><c:forEach var="icon" items="${scoreIcons}"><img class="${icon}" src="http://a.xnimg.cn/a.gif" /></c:forEach></a>
							<span class="user-grade">${myLevel}级</span>
						</c:if>
						<span class="user-live-days">连续登录${loginCDays}天</span>
					</div>
				</div>
			</div>
		</c:otherwise>
	</c:choose>
</div>
</div>