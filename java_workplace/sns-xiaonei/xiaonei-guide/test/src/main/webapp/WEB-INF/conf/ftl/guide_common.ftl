${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#if fID[0]?number != 0>
			<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
		<#else>
			Äã
		</#if>
		${pre[0]} <a href="${(url[0])?replace('xiaonei.com','renren.com')}" target="_blank">${title[0]}</a> ${cont[0]}
	</h3>
	<div class="details">
		<div class="legend">
			<img lala="${(icon[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
