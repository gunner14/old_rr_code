${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
		<#if prefix?exists>${prefix[0]}</#if>
		<a href="${(url[0])?replace('xiaonei.com','renren.com')}" target="_blank" onclick="readHomeFeed('${id[0]}')"> ${title[0]} </a>
		<#if cont?exists>${cont[0]}</#if>
		<#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
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
