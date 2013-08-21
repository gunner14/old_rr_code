<%@ page contentType="text/html;charset=UTF-8" language="java" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<div class="wiki-intro-kind clearfix">
	<div class="wiki-head-box" ftype="user">
		<div class="wiki-head-list<c:if test="${fn:length(followingList) == 0}"> wiki-head-list-none</c:if> clearfix">
			<ul class="clearfix">
				<c:forEach var="item" items="${followingList}">
					<li><img title="${item.name}" alt="头像" src="${item.tiny_Url}"></li>
				</c:forEach>
			</ul>
			<c:if test="${friendsFollowCount>0}">
				<span class="wiki-comm-fri">${friendsFollowCount}个好友</span>
			</c:if>
		</div>
		<p>
			<a href="javascript:void(0)">喜欢</a><span class="wiki-num">(${followCount})</span>
		</p>
	</div>
	<div class="wiki-photo-box" ftype="photo">
	<script>
	function photoAjust(img,w,h){
		var r = img.width/img.height;
		if (r >= w/h) {
			img.height = h;
			img.width = img.height * r;
		}else{
			img.width = w;
			img.height = img.width / r;

		}
	}</script>
		<div class="wiki-video<c:if test="${photoHighLight.count eq 0}"> wiki-video-none</c:if>">
		<c:choose>
				<c:when test="${photoHighLight.count > 0}">
					<c:forEach var="_pthumbnail" items="${photoHighLight.thumbnails}">
						<img src="${_pthumbnail}" onload="photoAjust(this,128.90)"/> 
					</c:forEach>
				</c:when>
				<c:otherwise>

				</c:otherwise>
			</c:choose>
		</div>
		<p>
			<a href="javascript:void(0)">照片</a><span class="wiki-num">(${photoHighLight.count})</span>
		</p>
	</div>
	<div class="wiki-video-box" ftype="video">
		<div class="wiki-video<c:if test="${videoHighLight.count eq 0}"> wiki-video-none</c:if>">
			<c:choose>
				<c:when test="${videoHighLight.count > 0}">
					<c:forEach var="_vthumbnail" items="${videoHighLight.thumbnails}">
						<img width="128" height="90"
							src="${_vthumbnail}"
							id="previewImg" />
					</c:forEach>
					<a href="javascript:;" class="m-play-btn"></a>
				</c:when>
				<c:otherwise>
				</c:otherwise>
			</c:choose>
		</div>
		<p>
			<a href="javascript:void(0)">视频</a><span class="wiki-num">(${videoHighLight.count})</span>
		</p>
	</div>
</div>