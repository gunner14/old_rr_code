<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:if test="${ !(empty homeLeftSites||fn:length(homeLeftSites)==0) }">
	<c:set var="hasZhan" value="true" />
</c:if>
<c:if test="${ familyName != null && familyUrl != null}">
	<c:set var="hasFamilySpace" value="true" />
</c:if>
<c:if test="${ !(empty managePageList||fn:length(managePageList)==0) }">
	<c:set var="hasPage" value="true" />
</c:if>
<c:if test="${ !(empty manageLoverPageList||fn:length(manageLoverPageList)==0) }">
	<c:set var="hasLoverSpace" value="true" />
</c:if>
<c:if test="${hasZhan||hasFamilySpace||hasPage||hasLoverSpace}">
	<article class="site-menu-apps">
		<h3>我管理的</h3>
		<section class="site-menu-apps-admins">
			<c:if test="${hasPage}">
				<c:forEach var="item" items="${managePageList}">
					<div class="apps-item">
						<div class="item-title">
							<span class="item-main"> <a title="${item.name}" href="http://page.renren.com/${item.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/follow.gif" /> ${fn:substring(item.name, 0, 7)}</a>
							</span>
						</div>
					</div>
				</c:forEach>
			</c:if>
			<c:if test="${hasZhan}">
				<c:forEach var="item" items="${homeLeftSites}">
					<div class="apps-item">
						<div class="item-title">
							<span class="item-main"> <a title="${item.name}" href="http://zhan.renren.com/zhan/${item.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/zhan.png" /> ${fn:substring(item.name, 0, 7)}</a>
							</span>
						</div>
					</div>
				</c:forEach>
			</c:if>
			<c:if test="${hasFamilySpace}">
				<div class="apps-item">
					<div class="item-title">
						<span class="item-main"> <a title="${familyName}" href="http://jia.renren.com/" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/app/home.gif" /> ${fn:substring(familyName, 0, 7)}</a>
						</span>
					</div>
				</div>
			</c:if>
			<c:if test="${hasLoverSpace}">
				<c:forEach var="item" items="${manageLoverPageList}">
					<div class="apps-item">
						<div class="item-title">
							<span class="item-main"> <a title="${item.name}" href="${applicationScope.urlLover}/${item.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/page/love/lover-heart.png" /> ${fn:substring(item.name, 0, 7)}</a>
							</span>
						</div>
					</div>
				</c:forEach>
			</c:if>
		</section>
	</article>
</c:if>
