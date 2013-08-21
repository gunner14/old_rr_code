${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 评论了
		<a href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}" target="_blank">${uName[0]}</a> 的
		<#if (type[0])?number == 1207>相册<#else>照片</#if>
		<a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${aName[0]}</a>
	</h3>
	<div class="content">
		<div class="figure"><img width="100" lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></div>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img class="iPost" alt="相册" src="http://s.xnimg.cn/a.gif"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#" class="more">查看更多评论</a>
			<a href="#" class="share">分享</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
