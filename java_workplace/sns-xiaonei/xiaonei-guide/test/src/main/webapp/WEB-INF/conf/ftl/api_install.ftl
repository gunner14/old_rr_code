${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list> 在使用应用 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
	</h3>
	<#if desc?exists>
		<div class="content">
			<blockquote>${desc[0]}</blockquote>
		</div>
	</#if>
	<div class="details">
		<div class="legend">
			<#if appIcon?exists>
				<img lala="${appIcon[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="应用安装"/>	
			<#else>
				<img src="http://s.xnimg.cn/a.gif" alt="应用安装" class="iApp" />
			</#if>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="${aboutUrl[0]}&ref=${vType}" target="_blank">查看应用详情</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
