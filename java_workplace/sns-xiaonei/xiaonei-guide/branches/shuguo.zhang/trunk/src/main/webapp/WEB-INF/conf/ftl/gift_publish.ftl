${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 收到了
		<#if uName[0] == "一个TA">
			一个TA
		<#elseif uName[0] == "神秘的TA">
			神秘的TA
		<#else>
			<a href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}" target="_blank">${uName[0]}</a>
		</#if>
		送的 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a>
		<#if cont?exists>
			${cont[0]}
		</#if>
		<#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
	</h3>
	<div class="content">
		<div class="figure"><a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" /></a></div>
		<#if uName[0] != "一个TA" && msg?exists && msg[0] != ""><p class="comment"><q>${msg[0]}</q></p></#if>
		<#if ad?exists><p class="giftlink">${ad[0]}</p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="礼物" class="iGift" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="http://gift.renren.com/send.do?fid=${fID[0]}" target="_blank">更多精彩礼物&raquo;</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
