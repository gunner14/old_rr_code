<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt"%>

<c:set var="classifyUsers">
	<c:choose>
		<c:when test="${visiter.id % 100 == 8 || visiter.id % 100 == 18}">class1</c:when>
		<c:when test="${visiter.id % 100 == 28 || visiter.id % 100 == 38}">class2</c:when>
		<c:when test="${visiter.id % 100 == 48 || visiter.id % 100 == 58 || visiter.id % 100 == 68}">class3</c:when>
		<c:when test="${visiter.id % 100 == 78 || visiter.id % 100 == 88 || visiter.id % 100 == 98}">class4</c:when>
		<c:otherwise>class1</c:otherwise>
	</c:choose>
</c:set>

<c:forEach items="${requestScope.userMenu.menuIds}" var="menuId" varStatus="idStat"><%--“我的应用”在“站内功能”上方--%>
	<c:if test="${idStat.last}">
		<c:set var="storedState" value="${menuId}"></c:set>
	</c:if>
</c:forEach>

<c:set var="myAppCount"><%--“我的应用”默认显示数量--%>
	<c:choose>
		<c:when test="${classifyUsers == 'class1' || classifyUsers == 'class2' || classifyUsers == 'class4'}">6</c:when>
		<c:when test="${classifyUsers == 'class3'}">4</c:when>
	</c:choose>
</c:set>
<c:set var="siteAppCount"><%--“站内功能”默认显示数量--%>
	<c:choose>
		<c:when test="${classifyUsers == 'class1' || classifyUsers == 'class2' || classifyUsers == 'class3'}">6</c:when>
		<c:when test="${classifyUsers == 'class4'}">4</c:when>
	</c:choose>
</c:set>
<c:if test="${isMenuVisited}">
	<script type="text/javascript">window.isMenuVisited = true;</script>
</c:if>

<c:set var="currentTime"><%=System.currentTimeMillis() %></c:set>
<c:set var="newsFeedUrl" value="http://www.renren.com/home?from=homeleft" />
<c:if test="${!visiter.userStateInfo.guideDone}">
	<c:set var="newsFeedUrl" value="http://guide.renren.com/guide?from=homeleft" />
</c:if>
<c:set var="newFeedTitle" value="${visiter.userStateInfo.guideDone || requestScope.newStageTen}" />
<div class="site-menu-nav-box box">
	<article id="site-menu-nav" class="site-menu-apps">
		<h3>
			<a href="http://app.renren.com/app/editApps" ui-async="async" class="more">更多</a>常用应用
		</h3>
		<section class="site-menu-apps-base">
			<div class="apps-item">
				<div class="item-title">
					<span class="item-main"> <a href="${home_url}" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/imgpro/v6/icon/feed.png" /> 新鲜事</a>
					</span>
				</div>
			</div>
			<c:if test="${!host.page}">
				<div class="apps-item">
					<div class="item-title">
						<span class="item-main"> <a stats="V6Homeleft_matter" href="http://matter.renren.com/${visiter.id}?page=0&from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/imgpro/v6/icon/matter-logo.gif" /> 与我相关<span class="apps-item-count" id="rm_feed_count"></span></a>
						</span>
					</div>
				</div>
			</c:if>
		</section>
		<section class="site-menu-apps-favor">
			<%-- 收藏的应用 --%>
			<%@ include file="/inc/n/modules/site-menu-apps-fav2.jsp" %>
		</section>
		<section id="saveAppsOrder" class="site-menu-apps-favor-save-order" style="display: none;">
			<a href="#nogo" onclick="return false;">完成排序</a>
		</section>
	</article>
	<article class="site-menu-apps site-menu-apps-locked" id="site-menu-apps-nav-locked">
		<section class="site-menu-apps-others">
			<c:if test="${!host.page}">
				<div class="apps-item">
					<div class="item-title">
						<span class="item-main"> 
						<a stats="V6Homeleft_appCenter" href="http://app.renren.com/?origin=50116" target="_blank" onclick="if($('newAppsTody')){$('newAppsTody').hide();}" class="with-count"> <img width="16" height="16" src="${applicationScope.urlStatic}/imgpro/v6/icon/app.png" class="icon" /> 
							应用中心

							<c:if test="${recent_online_app_count ne null}">
                                <a id="apps-count" target="_blank" href="http://app.renren.com/?origin=50170#game-dots">${recent_online_app_count}</a>
							</c:if> 
						
						</a>
						</span>
					</div>
				</div>
				<c:if test="${requestScope.visiter.stage==20||requestScope.visiter.stage==30}">
					<div class="apps-item">
						<div class="item-title">
							<span class="item-main"> <a stats="V6Homeleft_webGame" href="http://wan.renren.com/" target="_blank"> <img width="16" height="16" src="${applicationScope.urlStatic}/imgpro/v6/icon/game.png" class="icon" /> 网页游戏
							</a>
							</span>
						</div>
					</div>
				</c:if>
				<c:set var="requestCount" value="0" />
				<c:forEach items="${requestScope.userCountMap}" var="entry">
					<c:set var="key" value="${entry.key}"></c:set>
					<c:if test="${key==0 || key==40}">
						<%--0代表邀请，40代表礼物--%>
						<c:set var="requestCount" value="${requestCount + entry.value}" />
					</c:if>
				</c:forEach>
				<c:if test="${requestCount>0}">
					<div id="appsItem_26" class="apps-item">
						<div class="item-title">
							<span class="item-main"> <a href="http://app.${applicationScope.sysdomain}/app/appRequestList?origin=700045" onclick="if($('allAppsInvite')){$('allAppsInvite').hide();}" ui-async="async" class="with-count"> <img width="16" height="16" src="http://app.xnimg.cn/application/20101018/16/35/L1MGd11n018153.gif" class="icon"> 应用请求 <c:if test="${requestCount>99}">
										<c:set var="requestCount" value="99+" />
									</c:if> <span id="allAppsInvite" class="apps-item-count" title="${requestCount}条应用请求">${requestCount}</span>
							</a>
							</span>
						</div>
					</div>
				</c:if>	

				<%-- 置顶应用 --%>
				<%@ include file="/inc/n/modules/site-menu-apps-top2.jsp" %>

			</c:if>
		</section>
	</article>
	<%--我的应用与站内功能模块begin--%>
	<c:if test="${storedState == 3}">
		<section class="site-menu-apps-module-order">
			<article class="site-menu-apps site-menu-apps-can-drag" id="site-menu-apps-mine" data-order="2" data-position="top">
				<h3><p class="reorder" title="移动模块至下方"><img src="http://a.xnimg.cn/imgpro/v6/reorder.png"></p>我的应用</h3>
				<%@ include file="/inc/n/modules/site-menu-apps-myapps.jsp" %>
			</article>

			<article class="site-menu-apps site-menu-apps-can-drag" id="site-menu-apps-func"  data-order="3" data-position="bottom">
				<h3><p class="reorder" title="移动模块至上方"><img src="http://a.xnimg.cn/imgpro/v6/reorder.png"></p>站内功能</h3>
				<%@ include file="/inc/n/modules/site-menu-apps-siteapps.jsp" %>
			</article>
		</section>
	</c:if>
	<c:if test="${storedState == 2}">
		<section class="site-menu-apps-module-order">
			<article class="site-menu-apps site-menu-apps-can-drag" id="site-menu-apps-func"  data-order="3" data-position="top">
				<h3><p class="reorder" title="移动模块至下方"><img src="http://a.xnimg.cn/imgpro/v6/reorder.png"></p>站内功能</h3>
				<%@ include file="/inc/n/modules/site-menu-apps-siteapps.jsp" %>
			</article>
			<article class="site-menu-apps site-menu-apps-can-drag" id="site-menu-apps-mine" data-order="2" data-position="bottom">
				<h3><p class="reorder" title="移动模块至上方"><img src="http://a.xnimg.cn/imgpro/v6/reorder.png"></p>我的应用</h3>
				<%@ include file="/inc/n/modules/site-menu-apps-myapps.jsp" %>
			</article>
		</section>
	</c:if>
	<%--我的应用与站内功能模块end--%>
	<c:if test="${fn:length(requestScope.userMenu.connectMenu) > 0}">
		<article class="site-menu-apps" id="site-menu-apps-recent-link">
			<h3>最近连接</h3>
			<section class="site-menu-app">
				<c:forEach var="connectItem" items="${requestScope.userMenu.connectMenu}" begin="0" end="2">	
					<c:choose>
						<c:when test="${fn:contains(connectItem.sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
						<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
					</c:choose>
					<div class="apps-item" id="appsItem_${connectItem.appId}">
						<div class="item-title">
							<span class="item-main"> <a href="${connectItem.sideNavUrl}${sp}origin=50302" target="_blank"><img class="icon" src="${connectItem.appIcon}" /> <span class="app-name">${connectItem.appName}</span></a>
							</span>
						</div>
						<div class="item-editor">
							<span class="editor-operations">
								<a href="#nogo" data-appid="${connectItem.appId}" data-optype="add" onclick="this.blur();return false;" class="an-operation an-operation-add" title="移至常用"><img src="http://a.xnimg.cn/imgpro/v6/up.png" /></a><a href="#nogo" data-appid="${connectItem.appId}" data-optype="del" onclick="this.blur();return false;" class="an-operation an-operation-del" title="移除"><img src="http://a.xnimg.cn/imgpro/v6/remove.png" /></a>
							</span>
						</div>
					</div>
				</c:forEach>
			</section>
		</article>
	</c:if>
	<c:if test="${!host.page}">
		<%-- 小群 --%>
		<%--file="/inc/n/modules/site-menu-apps-minigroup.jsp" --%>
		
		<%-- 我管理的小站、公共主页、情侣空间、家庭空间等 --%>
		<%@ include file="/inc/n/modules/site-menu-apps-admin2.jsp" %>
		
		<%-- 推荐应用 
			<%@ include file="/inc/n/modules/site-menu-apps-recommend.jsp" %>
		--%>
	</c:if>	
</div>
<c:if test="${!host.page}">
	<%--应用动态--%>
	<%@ include file="/inc/n/modules/site-menu-apps-dynamic.jsp"%>
</c:if>	