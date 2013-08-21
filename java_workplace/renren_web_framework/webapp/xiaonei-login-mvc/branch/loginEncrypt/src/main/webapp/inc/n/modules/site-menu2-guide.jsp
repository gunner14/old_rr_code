<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt"%>
<c:set var="newsFeedUrl" value="http://www.renren.com/home?from=homeleft" />
<c:if test="${!visiter.userStateInfo.guideDone}">
	<c:set var="newsFeedUrl" value="http://guide.renren.com/guide?from=homeleft" />
</c:if>
<c:set var="newFeedTitle" value="${visiter.userStateInfo.guideDone || requestScope.newStageTen}" />
<c:set var="home_url" value="http://guide.renren.com/guide" />
<div class="site-menu-nav-box box">
	<article id="site-menu-nav" class="site-menu-apps">
		<h3>
			<a href="http://app.renren.com/app/editApps" ui-async="async" class="more">更多</a>我的应用
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
						<span class="item-main"> <a stats="V6Guideleft_matter" href="http://matter.renren.com/${visiter.id}?page=0&from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/imgpro/v6/icon/matter-logo.gif" /> 与我相关<span class="apps-item-count" id="rm_feed_count"></span></a>
						</span>
					</div>
				</div>
				
				<div class="apps-item">
					<div class="item-title">
						<span class="item-main"> <a href="http://message.renren.com/session/index?from=lefthome" ui-async="async"><img class="icon" src="http://a.xnimg.cn/apps/session/res/img/s-icon.png" /> 私信</a>
						</span>
					</div>
				</div>
				
			</c:if>
		</section>
		<section class="site-menu-apps-favor">
			<%-- 收藏的应用 --%>
			<%@ include file="/inc/n/modules/site-menu-apps-fav-guide.jsp" %>
		</section>
		<section id="saveAppsOrder" class="site-menu-apps-favor-save-order" style="display: none;">
			<a href="#nogo" onclick="return false;">完成排序</a>
		</section>
	</article>
	
	<article class="site-menu-apps" id="site-menu-apps-nav">
		<section class="site-menu-apps-others">
			<c:if test="${!host.page}">
				<div class="apps-item">
					<div class="item-title">
						<span class="item-main"> 
						<a stats="V6Guideleft_appCenter" href="http://app.renren.com/apps/track/homeView?originUrl=http://app.renren.com&origin=50116" target="_blank" onclick="if($('newAppsTody')){$('newAppsTody').hide();}" class="with-count"> <img width="16" height="16" src="${applicationScope.urlStatic}/imgpro/v6/icon/app.png" class="icon"> 应用中心

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
							<span class="item-main"> <a stats="V6Guideleft_webGame" href="http://game.renren.com/" target="_blank"> <img width="16" height="16" src="${applicationScope.urlStatic}/imgpro/v6/icon/game.png" class="icon" /> 网页游戏
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
				<%@ include file="/inc/n/modules/site-menu-apps-top-guide.jsp" %>
				
				<c:set var="currentTime"><%=System.currentTimeMillis() %></c:set>
				<c:set var="showBegin" value="1335891600000" />
				<c:set var="showEnd" value="1336575600000" />
				<c:if test="${currentTime>=showBegin && currentTime<=showEnd}">
					<%-- <div id="appsItem_189913" class="apps-item">
						<div class="item-title">
							<span class="item-main"> 
							<a href="http://show.renren.com?ref=sitemenu" target="_blank"> <img width="16" height="16" src="http://a.xnimg.cn/imgpro/icons/show.png" class="icon"> 缤纷五月,欢乐秀</a>
							</span>
						</div>
					</div> --%>
				</c:if>
			
			</c:if>
			
			<c:set var="origin">
				<c:choose>
					<c:when test="${visiter.userStateInfo.guideDone}">50115</c:when>
					<c:otherwise>103&guide</c:otherwise>
				</c:choose>
			</c:set>
			<c:set var="begin" value="0" />
			<c:set var="end" value="5" />
			<%@ include file="/inc/n/modules/site-menu-apps-item2-guide.jsp" %>
			<div class="site-menu-apps-more" style="display:block;">
				<c:set var="begin" value="6" />
				<c:set var="end" value="14" />
				<%@ include file="/inc/n/modules/site-menu-apps-item2-guide.jsp" %>
				<c:if test="${!host.page}">
				<div class="add-new-apps">
					<a href="http://app.renren.com/" class="add-new-apps-link" target="_blank">添加新应用</a>
				</div>
				</c:if>
			</div>
		</section>
		<section style="display:none;" class="site-menu-apps-toggle-show">
			<a class="to-show" href="#nogo" onclick="return false;">展开</a>
		</section>
	</article>
	
	<c:if test="${!host.page}">
		<%-- 小群 --%>
		<%@ include file="/inc/n/modules/site-menu-apps-minigroup.jsp" %>
		
		<%-- 我管理的小站、公共主页、情侣空间、家庭空间等 --%>
		<%@ include file="/inc/n/modules/site-menu-apps-admin.jsp" %>
		
		<%-- 推荐应用 
			<%@ include file="/inc/n/modules/site-menu-apps-recommend.jsp" %>
		--%>
		<%--应用动态--%>
		<%@ include file="/inc/n/modules/site-menu-apps-dynamic-guide.jsp"%>
	</c:if>
</div>
