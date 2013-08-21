<%@ page pageEncoding="utf-8" %>
<div class="news-feed">
	<div class="feed-header clearfix">
		<ul class="feed-filter clearfix">
			<li class="fresh-news">好友新鲜事</li>				
		</ul>
	</div>
	<div class="feed-holder" id="feedHolder">
		<ul id="feedHome" class="feeds richlist">	
			<c:choose>
				<c:when test="${empty requestScope.userFeed}"><li class="newsfeed-empty">当你的朋友发表日志、照片或者帖子，这里会有提示。</li></c:when>
				<c:otherwise>
					${requestScope.userFeed}
				</c:otherwise>
			</c:choose>
		</ul>
	</div>
	<div class="show-all trigger" style="height:0;line-height:0;overflow:hidden;border:0px;">
		<a id="moreFeed" href="javascript:;">更多新鲜事</a>
		<p style="display:none" id="feedLoading"><span>新鲜事读取中...</span></p>
	</div>
	<div class="more-feed">			
		<input type="hidden" id="maxPage" value="${requestScope.maxPage}"/>
	</div>
</div>


