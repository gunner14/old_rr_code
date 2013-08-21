${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 发表日志 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
	<#if wap?exists><span class="from">通过<a href="http://mobile.renren.com/?${vType}" target="_blank">手机</a>发布 <a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif"" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
	</h3>
	<div class="content">
		<blockquote>${brief[0]}</blockquote>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="日志" src="http://s.xnimg.cn/a.gif"" class="iBlog"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>