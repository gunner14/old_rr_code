${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list> 
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
		分享 <a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}">${uName[0]}</a> 的照片 <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${aName[0]}</a>
	</h3>
	<div class="content">
		<div class="figure"><a href="${(pUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${ (pSrc[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<p>${title[0]}</p>
		<p>相册: <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${aName[0]}</a></p>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="分享" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
			<a href="http://share.renren.com/share/ShareList.do?id=${fID[0]}&share_id=${sID[0]}#share_${sID[0]}" target="_blank">评论该分享</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>