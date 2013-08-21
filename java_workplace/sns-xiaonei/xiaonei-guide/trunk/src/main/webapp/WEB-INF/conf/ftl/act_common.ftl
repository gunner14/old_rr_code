${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<#if uLink?exists>
				<a target="_blank" href="${uLink[0]}">${n}</a>
			<#else>
				<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>				
			</#if>
		</#list> ${pre[0]} <a href="${url[0]}" target="_blank">${title[0]}</a> ${cont[0]}
	</h3>
	<#if src?exists>
		<div class="content">
				<a href="${url[0]}" target="_blank"><img lala="${src[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
		</div>
	</#if>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="人人" class="iActs" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
