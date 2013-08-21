${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list> 收到礼物 <a href="${ (url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
	</h3>
	<div class="content">
		<div class="sub-title">
			<h5>最新消息: <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 收到
			<#if uName[0] == "一个TA">
				一个TA
			<#else>
				<a href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}" target="_blank">${uName[0]}</a>
			</#if>
			送的 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a></h5>
		</div>
		<div class="figure"><a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<#if uName[0] != "一个TA" && msg?exists && msg[0] != ""><p class="comment"><q>${msg[0]}</q></p></#if>
		<#if ad?exists><p class="giftlink">${ad[0]}</p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="礼物" class="iGift" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="http://gift.renren.com/send.do?fid=${fID[0]}" target="_blank">更多精彩礼物>></a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>