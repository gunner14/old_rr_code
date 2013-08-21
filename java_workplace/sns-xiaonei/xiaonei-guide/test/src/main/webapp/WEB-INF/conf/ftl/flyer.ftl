${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<h3>
		<a href="${url[0]}" class="dark" target="_blank">${title[0]}</a>
	</h3>
	<div class="content">
	<#if sID?exists>
		<div class="video" style="background-image: url(${thumb[0]});">
			<a href="#nogo" onclick="playFeedVideo('${sID[0]}','${id[0]}','240570432',1.2);this.style.display='none';">播放</a>
		</div>
		<p><a href="${(url[0])?replace('xiaonei.com','renren.com')}" class="dark" target="_blank">${body[0]}</a></p>
	<#else>
		<p><a href="${(url[0])?replace('xiaonei.com','renren.com')}" class="dark" target="_blank">${body[0]}</a></p>
	</#if>
	</div>
	<div class="details">
		<div class="legend">
			<img lala="http://xnimg.cn/imgpro/icons/flyer.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<#if sID?exists>
			<a href="#nogo" onclick="create_share_div(${sID[0]},240570432,240570432);" >分享</a>
			</#if>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
