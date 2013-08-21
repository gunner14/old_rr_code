${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>发起了投票 
		<a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
		<#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
	</h3>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="投票" class="iPoll" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
