${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 分享 <a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}">${uName[0]}</a> 的相册
		<#if title?exists><a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${title[0]}</a></#if>
		<#if wap?exists><span class="from">通过<a href="http://mobile.renren.com/?${vType}" target="_blank">手机</a>发布 <a href="http://mobile.renren.com/?${vType}"><img src="http://s.xnimg.cn/a.gif"" alt="通过手机发布" class="iTinyphone" target="_blank"/></a></span></#if>
	</h3>
	<div class="content">
		<div class="figure"><a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${ (aSrc[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<p>共${count[0]}张照片</p>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="分享" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>