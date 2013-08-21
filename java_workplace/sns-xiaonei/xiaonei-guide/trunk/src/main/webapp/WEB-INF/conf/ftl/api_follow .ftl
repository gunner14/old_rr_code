${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list> 在人人网成为了 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a> 的关注者
	</h3>
	<#if desc?exists>
		<div class="content">
			<blockquote>${desc[0]}</blockquote>
		</div>
	</#if>
	<div class="details">
		<div class="legend">
			<#if appIcon?exists>
				<img src="${appIcon[0]}" alt="关注者"/>	
			<#else>
				<img src="http://xnimg.cn/a.gif" alt="关注者" class="iApp" />
			</#if>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="http://app.renren.com/profile/${appID}?ref=${vType}" target="_blank">成为关注者</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>