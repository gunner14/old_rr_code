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

		<c:set var="hotClass" value=" selected" />
		<c:set var="liveClass" value="" />
		<c:if test="${savedFeedType == 'live'}">
			<c:set var="hotClass" value="" />
			<c:set var="liveClass" value=" selected" />
		</c:if>
			
			<div class="tabview tabview-align-right">
				<h3 stats="V6Feed_refresh" class="title">
					<img src="http://a.xnimg.cn/imgpro/v6/icon/feed-hl.png" class="icon" />
					新鲜事
				</h3>
				<div class="tabview-options" style="padding-top:6px;">
					<a class="news-feed-setting" href="${applicationScope.urlWww}/feedprefs.do">
						<img stats="V6Feed_option" src="http://a.xnimg.cn/imgpro/v6/feed_setting.png" alt="新鲜事设置" title="新鲜事设置" style="display:block;" />
					</a>
				</div>
				<div class="tabview-tabs">
					<a stats="V6Feed_attention" href="#nogo" id="feedTabAttention" data-id="attention" onclick="return false;" class="a-tab">特别关注</a>
					<a stats="V6Feed_hotFeed" href="#nogo" id="feedTabFeed" data-id="old" onclick="return false;" class="a-tab selected">热点内容</a>
				</div>
			</div>
			
			<!--
			<div class="feed-tools">
				<menu type="toolbar">
					<li class="refresh">
						<a class="reload-feed" href="#nogo" onclick="return false;">刷新</a>
					</li>
					<li class="setting">
						<a class="feed-setting" id="setNewsfeed" href="${applicationScope.urlWww}/feedprefs.do">新鲜事设置</a>
						<a class="feed-setting" id="setAttentionFeed" style="display: none;" href="${applicationScope.urlWww}/feedprefs.do">设置关注名单</a>
					</li>
					<li class="types">
						<menu class="feed-categories category-filter">
							<label id="feedTabCategories" data-id="old" class="s">
								<span class="label">
									<span class="text">新鲜事</span>
								</span>
							</label>
						</menu>
						
						<menu class="feed-attention">
							<label id="feedTabAttention" data-id="attention">
								<span class="label">
									<span class="text">特别关注</span>
								</span>
							</label>
						</menu>
					</li>
				</menu>
			</div>
			-->
				
	</c:if>

	<div class="feed-list">
		<c:if test="${param.inited != false}">
			<c:choose>
				<c:when test="${noNewsfeed == true}"><div class="newsfeed-empty">当你的朋友发表日志、照片或者帖子，这里会有提示。</div></c:when>
				<c:otherwise>
					<script type="text/javascript">
						if (window.UGC && UGC.Network) UGC.Network.ping.normal.start("100001", "sns.home.feedlist");
					</script>
					${fn:trim(requestScope.userFeed)}
					<script type="text/javascript">
						if (window.UGC && UGC.Network) UGC.Network.ping.normal.end("100001", "sns.home.feedlist", 2, 150);
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
