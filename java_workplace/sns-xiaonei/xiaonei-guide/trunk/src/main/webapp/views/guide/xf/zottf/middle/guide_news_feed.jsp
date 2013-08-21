<%@ page pageEncoding="utf-8" %>
<div class="news-feed">
	<div class="feed-header clearfix" style="display:none">
		<ul class="feed-filter clearfix">
			<li class="fresh-news">新鲜事</li>
			<li class="current" id="showAllFeed"><a onfocus="this.blur();" href="javascript:;">全部</a></li>
			<li id="showStatusFeed"><a onfocus="this.blur();" href="javascript:;">状态</a></li>
			<li id="showMenuFeed"><a onfocus="this.blur();" href="javascript:;">分享</a></li>			
            <li id="showAlbumFeed"><a onfocus="this.blur();" href="javascript:;">相册</a></li>
            <li id="showBlogFeed"><a onfocus="this.blur();" href="javascript:;">日志</a></li>
			<li id="showPageFeed"><a onfocus="this.blur();" href="javascript:;" onclick="ajax_stat_page(${visiter.id});">公共主页</a></li>
		</ul>
	</div>
	<div class="feed-holder" id="feedHolder">
		<ul id="feedHome" class="feeds richlist">	
			<c:choose>
				<c:when test="${empty requestScope.userFeed}">
					<li class="newsfeed-empty">
						<div class="no-feedContent">
							<p><font color="black">当你的好友发布状态、相册等的时候这里会显示。</font></p>							
						</div>
					</li>
				</c:when>
				<c:otherwise>
					${requestScope.userFeed}
				</c:otherwise>
			</c:choose>
		</ul>		
	</div>
	<c:choose>
		<c:when test="${empty requestScope.userFeed}">
			<div class="show-all trigger"  style="display:none">
				<a id="moreFeed" href="javascript:;">更多新鲜事</a>
				<p style="display:none" id="feedLoading"><span>新鲜事读取中...</span></p>
			</div>
			<div class="more-feed"  style="display:none">
				<span class="mark-read" style="display:none"><a id="setAllFeedsAsRead" href="javascript:;">全部标记为已读</a></span>
				<span style="display:none"><a href="${applicationScope.urlWww}/feedprefs.do">新鲜事设置</a></span>
				<input type="hidden" id="maxPage" value="${requestScope.maxPage}"/>
			</div>
		</c:when>
		<c:otherwise>
			<div class="show-all trigger">
			<a id="moreFeed" href="javascript:;">更多新鲜事</a>
			<p style="display:none" id="feedLoading"><span>新鲜事读取中...</span></p>
		</div>
		<div class="more-feed">
			<span class="mark-read" style="display:none"><a id="setAllFeedsAsRead" href="javascript:;">全部标记为已读</a></span>
			<span style="display:none"><a href="${applicationScope.urlWww}/feedprefs.do">新鲜事设置</a></span>
			<input type="hidden" id="maxPage" value="${requestScope.maxPage}"/>
		</div>
		</c:otherwise>
	</c:choose>
</div>
