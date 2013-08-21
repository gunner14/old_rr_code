<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<%--
@param showHeader 是否显示顶部
@param twoCols 是否有切换热门
@param twoColsNew 是否有切换特别关注
@param showCategories 显示分类切换
@param inited 是否展示默认新鲜事
@param loadOnScroll 是否在滚屏时自动加载
@param canReadAll 是否允许标记已读
@param hostId hostid
@param readMoreAction 加载更多的js接口地址
--%>

<c:set var="readMoreActionAttribute">
<c:if test="${!empty param.readMoreAction}"> data-readmoreaction="${param.readMoreAction}"</c:if>
</c:set>

<div class="feed-module" data-loadonscroll="${param.loadOnScroll}" data-host="${param.hostId}"${readMoreActionAttribute}>

	<%--是否有新鲜事--%>
	<c:set var="noNewsfeed" value="false" />
	<c:if test="${empty requestScope.userFeed}">
		<c:set var="noNewsfeed" value="true" />
	</c:if>

	<c:if test="${param.showHeader == true}">

		<%--顶部导航两栏输出不同class--%>
		<c:set var="feedClass">
			<c:choose>
				<c:when test="${param.twoColsNew}">feed-header-new</c:when>
				<c:when test="${param.twoCols}">feed-header</c:when>
				<c:otherwise>feed-headline</c:otherwise>
			</c:choose>
		</c:set>

		<c:set var="hotClass" value=" selected" />
		<c:set var="liveClass" value="" />
		<c:if test="${savedFeedType == 'live'}">
			<c:set var="hotClass" value="" />
			<c:set var="liveClass" value=" selected" />
		</c:if>

		<div class="${feedClass}">

			<c:choose>
				<c:when test="${param.twoColsNew}">
				</c:when>
				<c:when test="${param.twoCols}">
					<div class="feed-types">
						<ul class="tab">
							<li class="feed-type${hotClass}" data-id="hot"><a href="javascript:;">精华新鲜事</a></li>
							<li class="feed-type${liveClass}" data-id="live"><a href="javascript:;">全部新鲜事 <span class="unreads-count" style="display:none"></span></a></li>
						</ul>
					</div>
				</c:when>
				<c:otherwise>
					<h3>新鲜事</h3>
				</c:otherwise>
			</c:choose>

			<c:choose>				
				<c:when test="${param.twoColsNew}">
					<div class="feed-tools">
						<menu type="toolbar">
							<li class="refresh">
								<a stats="V5Feed_refresh" class="reload-feed" href="#nogo" onclick="return false;">刷新</a>
							</li>
							<li class="setting">
								<a stats="V5Feed_option" class="feed-setting" id="setNewsfeed" href="${applicationScope.urlWww}/feedprefs.do">新鲜事设置</a>
								<a stats="V5Feed_option" class="feed-setting" id="setAttentionFeed" style="display: none;" href="${applicationScope.urlWww}/feedprefs.do">设置关注名单</a>
							</li>
							<li class="types">
								<menu class="feed-categories category-filter">
									<label id="feedTabCategories" data-id="old"<c:if test="${!(savedFeedType=='live')}"> class="s"</c:if>>
										<span class="label">
											<span class="text" stats="V5Feed_hotFeed">新鲜事</span>
											<span class="arrow-down"></span>
										</span>
									</label>
									<menu>
										<li class="selected feed-category" data-id="-1"><a href="javascript:;">全部</a></li>
										<li class="feed-category" data-id="5"><a href="javascript:;">状态</a></li>
										<li class="feed-category" data-id="7"><a href="javascript:;">相册</a></li>
										<li class="feed-category" data-id="1"><a href="javascript:;">分享</a></li>
										<li class="feed-category" data-id="6"><a href="javascript:;">日志</a></li>
										<li class="feed-category" data-id="20"><a href="javascript:;">公共主页</a></li>
									</menu>
								</menu>
								<c:if test="${savedFeedType == 'live' || savedFeedType=='hot' }">
									<c:set var="feedTabText">
										<c:choose>
											<c:when test="${visiter.id%10==6}">时间排序</c:when>
											<c:otherwise>好友原创</c:otherwise>
										</c:choose>
									</c:set>
									<menu class="original-ugc">
										<label id="feedTabOriginalUgc" data-id="originalUgc"<c:if test="${savedFeedType=='live'}"> class="s"</c:if>>
											<span class="label">
												<span stats="V5Feed_originalFeed" class="text">${feedTabText}</span>
											</span>
										</label>
									</menu>
								</c:if>
								<menu class="feed-attention">
									<label id="feedTabAttention" data-id="attention">
										<span class="label">
											<span stats="V5Feed_attention" class="text">特别关注</span>
										</span>
									</label>
								</menu>
							</li>
						</menu>
					</div>
				</c:when>
				<c:otherwise>
					<div class="feed-tools">
						<menu type="toolbar">
							<li>
								<a class="reload-feed" ui-async="async" href="/home">刷新</a>
							</li>
							<c:if test="${param.showCategories == true}">
								<li>
									<menu class="feed-categories category-filter">
										<label>全部</label>
										<menu>
											<li class="selected feed-category" data-id="-1"><a href="javascript:;">全部</a></li>
											<li class="feed-category" data-id="5"><a href="javascript:;">状态</a></li>
											<li class="feed-category" data-id="7"><a href="javascript:;">相册</a></li>
											<li class="feed-category" data-id="1"><a href="javascript:;">分享</a></li>
											<li class="feed-category" data-id="6"><a href="javascript:;">日志</a></li>
											<li class="feed-category" data-id="20"><a href="javascript:;">公共主页</a></li>
										</menu>
									</menu>
								</li>
							</c:if>
							<li>
								<a class="feed-setting" href="${applicationScope.urlWww}/feedprefs.do">设置</a>
							</li>
						</menu>
					</div>
				</c:otherwise>
			</c:choose>
		</div>
	</c:if>

	<div class="feed-list">
		<c:if test="${param.inited != false}">
			<c:choose>
				<c:when test="${noNewsfeed == true}"><div class="newsfeed-empty">当你的朋友发表日志、照片或者帖子，这里会有提示。</div></c:when>
				<c:otherwise>
					<script type="text/javascript">
						if (window.UGC && UGC.Network) UGC.Network.ping.normal.start("100007", "sns.ohome.feedlist");
					</script>
					${fn:trim(requestScope.userFeed)}
					<script type="text/javascript">
						if (window.UGC && UGC.Network) UGC.Network.ping.normal.end("100007", "sns.ohome.feedlist", 2, 150);
					</script>
				</c:otherwise>
			</c:choose>
		</c:if>
	</div>

	<c:if test="${param.inited == false || noNewsfeed != true}">
		<div class="feed-footer">
			<div class="feed-loading" style="display:none"><p>新鲜事读取中...</p></div>
			<div class="nomore-feed" style="display:none"><p>没有新鲜事了，<a href="http://explore.renren.com/?from=nofeed">去探索看看</a>更多新鲜精彩内容</p></div>
			<c:if test="${param.showCategories == true}">
				<div class="nomore-feed" data-forcategoryid="5" style="display:none"><p>没有更多状态新鲜事了，你可以看看<a href="http://status.renren.com/?fromType=nomore">好友最近的状态 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="7" style="display:none"><p>没有更多相册新鲜事了，你可以看看<a href="http://photo.renren.com/photo/${visiter.id}/album/friends">好友最近的相册 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="1" style="display:none"><p>没有更多分享新鲜事了，你可以看看<a href="http://share.renren.com/">热门分享 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="6" style="display:none"><p>没有更多日志新鲜事了，你可以看看<a href="http://blog.renren.com/friendsBlog.do">好友的日志 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="20" style="display:none"><p>没有更多的公共主页新鲜事了，人人网为你推荐，快来关注吧。</p></div>
			</c:if>
			<div class="more-feed"><a id="more-feed" href="#">更多新鲜事</a></div>
			<c:if test="${param.canReadAll != false}"><p class="mark-all-read"><a href="javascript:;">全部标记为已读</a></p></c:if>
		</div>
	</c:if>

</div>
