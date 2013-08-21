<%@ page pageEncoding="utf-8" %>
<c:if test="${visiter.selected!=1}">
	<div id="setHeadItem" class="side-item">
		<div class="side-item-header clearfix">
		<h4>设置头像</h4>	
		</div>
		<div class="side-item-body clearfix">
			<div class="figurepanel">
				<div class="figure">
					<img src="${requestScope.visiter.tinyFullUrl}" alt="头像" />
				</div>
				<div class="article">
					<p>使用真实照片，<br />方便好友找到您</p>
				</div>	
				<div class="footer">
					<a id="uploadPhotoLink" href="javascript:;">上传头像</a> 
					<span class="pipe">|</span> 
					<a href="${applicationScope.urlHeadUpload}/Upload.do?ref=guide" target="_blank">使用摄像头</a>
				</div>
			</div>
		</div>
	</div>
</c:if>