<%@ page pageEncoding="utf-8" %>
<div class="interest">
	<h3>再看看大家喜欢什么吧！</h3>
	<div class="videos">
		<p class="legend">
			今日最火视频	
			(<a href="${applicationScope.urlShare}/share/hotsharelist.do?ref=guide_video_more" target="_blank">${requestScope.hotVideoCount}</a> 个)
		</p>
		<span class="more"><a href="${applicationScope.urlShare}/share/hotsharelist.do?ref=guide_video_more" target="_blank">更多</a></span>
		<ul class="clearfix">
			<c:forEach varStatus="status" var="item" items="${requestScope.hotVideoList}" begin="0" end="3">
				<li>
					<div class="figure">
						<img src="${item.picFullUrl}" title="${item.title}" width="120px" height="90px"/>
						<a href="${urlShare}/share/GetShare.do?id=${item.id}&amp;owner=${item.owner}&amp;ref=guide_video" target="_blank"></a>
					</div>
					<div class="info">
						<p class="title">
							<c:choose>
								<c:when test="${fn:length(item.title)<=20}">
									<a href="${urlShare}/share/GetShare.do?id=${item.id}&amp;owner=${item.owner}&amp;ref=guide_video" title="${item.title}" target="_blank">
										${item.title}
									</a>
								</c:when>
								<c:otherwise>
									<a href="${urlShare}/share/GetShare.do?id=${item.id}&amp;owner=${item.owner}&amp;ref=guide_video" title="${item.title}" target="_blank">
										${fn:substring(item.title,0,19)}...
									</a>
								</c:otherwise>
							</c:choose>							
						</p>
						<p class="num">分享人数：${item.commentcount}</p>
					</div>
				</li>
			</c:forEach>											
		</ul>
	</div>
	<div class="online"></div>
	<div class="photos">
		<p class="legend">
			今日人气相册
			(<a href="${applicationScope.urlShare}/share/hotsharelist.do?ref=guide_photo_more" target="_blank">${requestScope.hotPhotoCount}</a> 个)</p>
		</p>
		<span class="more"><a href="${applicationScope.urlShare}/share/hotsharelist.do?ref=guide_photo_more" target="_blank">更多</a></span>
		<ul>
			<c:forEach varStatus="status" var="item" items="${requestScope.hotPhotoList}" begin="0" end="3">
				<li>
					<div class="figure">
						<a href="${item.link}&amp;ref=guide_photo" target="_blank">
							<img src="${item.picFullUrl}" title="${item.summary}" width="100"/>
						</a>
					</div>
					<div class="info">
						<p class="title">
							<c:choose>
								<c:when test="${fn:length(item.summary)<=20}">
									<a href="${item.link}&amp;ref=guide_photo" title="${item.summary}" target="_blank">${item.summary}</a>
								</c:when>
								<c:otherwise>
									<a href="${item.link}&amp;ref=guide_photo" title="${item.summary}" target="_blank">${fn:substring(item.summary,0,16)}...</a>
								</c:otherwise>								
							</c:choose>								
						</p>
						<p class="num">分享人数：${item.commentcount}</p>
					</div>
				</li>		
			</c:forEach>									
		</ul>
	</div>
	<div class="online"></div>
	<%@ include file="/views/guide/xf/common/middle/guide_hot_pages.jsp"%>
</div>
