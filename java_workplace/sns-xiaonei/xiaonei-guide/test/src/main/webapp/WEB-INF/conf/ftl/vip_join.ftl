${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=newsfeed">${n}</a>
		</#list> 成为了人人网 <a href="${(url[0])?replace('xiaonei.com','renren.com')}" target="_blank">VIP会员</a>，尊享人人网VIP会员特权
	</h3>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="VIP" class="iVip" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
