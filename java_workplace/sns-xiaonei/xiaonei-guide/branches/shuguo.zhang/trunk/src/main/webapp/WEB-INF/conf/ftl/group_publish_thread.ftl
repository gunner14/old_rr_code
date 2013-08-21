${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a target="_blank" href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}">${fName[0]}</a> 发表话题 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
	</h3>
	<div class="details">
		<div class="legend">
			<img lala="http://app.rrimg.com/application/20090904/11/20/L467474953897TJS.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="帖吧" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
