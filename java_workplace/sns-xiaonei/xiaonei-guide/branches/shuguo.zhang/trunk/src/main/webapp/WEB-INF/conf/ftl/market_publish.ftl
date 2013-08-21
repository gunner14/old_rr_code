${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 发布集市信息 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
	</h3>
	<div class="content">
		<#if src?exists && src[0] != "">
			<a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
		</#if>
		<p>来自: <a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}">${uName[0]}</a> </p>
		<p>${brief[0]}</p>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="跳蚤市场" class="iMarket" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
