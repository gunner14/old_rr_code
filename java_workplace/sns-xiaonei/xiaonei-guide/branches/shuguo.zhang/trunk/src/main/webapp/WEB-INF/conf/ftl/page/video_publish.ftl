${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://page.renren.com/${fID[0]}?ref=${vType}" target="_blank">${fName[0]}</a> 
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
		上传视频 <a target="_blank" href="http://page.renren.com/media/video/singleVideo?pid=${pageID[0]}&vid=${videoID[0]}">${videoName[0]}</a>
		到
		<a target="_blank" href="http://page.renren.com/media/video/videoList?pid=${pageID[0]}&albumId=${albumID[0]}">${albumName[0]}</a>
	</h3>
	<div class="content">
		<div class="video" style="background-image: url(${thumb[0]});">
			<a href="http://page.renren.com/media/video/singleVideo?pid=${pageID[0]}&vid=${videoID[0]}">${videoName[0]}</a>
		</div>
		<#if brief?exists && brief[0] != ""><p class="comment"><q>${brief[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img class="iPagevideo" src="http://s.xnimg.cn/a.gif"" alt="视频"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
			<a target="_blank" href="http://page.renren.com/media/video/singleVideo?pid=${pageID[0]}&vid=${videoID[0]}">查看视频</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>