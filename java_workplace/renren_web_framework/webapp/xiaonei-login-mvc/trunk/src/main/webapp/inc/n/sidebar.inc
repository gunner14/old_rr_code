<%@ page pageEncoding="UTF-8" %>
<div id="sidebar">

	<c:if test="${not fn:contains(PAGE_MODE, 'nosidead, ') and requestScope.isFromIM != true}">
		<div class="blank-bar">
			<c:if test="${requestScope.visiter.auth >= 5}">
				<div class="admin-menu">
					<ul>
						<li>
							<a href="${applicationScope.urlAdmin}/admin/adminmanage.do">管理员入口</a>
						</li>
						<c:if test="${requestScope.visiter.auth >= 20}">
							<li>
								<a href="${applicationScope.urlAdmin}/admin/oldadminmanage.do">旧管理员入口</a>
							</li>
							<li>
								<a href="${applicationScope.urlAdmin}/admin/superadminmanage.do">超级管理员</a>
							</li>
						</c:if>
					</ul>
				</div>
			</c:if>
			<c:if test="${empty cleanpage}">
				<c:if test="${requestScope.visiter.loginCount > 6}">
					<div class="ad-bar">	
						<c:if test="${visiter.stage == 10}">
							<%@ include file="/inc/promo-narrow-highschool.inc" %>
							<%@ include file="/inc/promo-hotcommend-highschool.inc" %>
						</c:if>
						<c:if test="${visiter.stage == 20}">
							<%@ include file="/inc/promo-narrow-new.inc" %>		
						</c:if>
						<c:if test="${visiter.stage == 30}">
							<%@ include file="/inc/promo-narrow-new.inc" %>
						</c:if>
					</div>
				</c:if>
			</c:if>
		</div>
	</c:if>

</div>
