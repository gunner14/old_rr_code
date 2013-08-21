${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list> 分享公共主页 <a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}">${uName[0]}</a>
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
	</h3>
	<div class="content">
		<div class="figure">
			<a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}"><img lala="${ (src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="${uName[0]}"/></a>
		</div>
		<#if summary?exists && summary[0] != "" ><p><strong>${summary[0]}</strong></p></#if>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="分享" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
			<a href="http://page.renren.com/pa/bf?pid=${uID[0]}&ref=${vType}" target="_blank">${bfName[0]}</a>
			<a href="http://share.renren.com/share/ShareList.do?id=${fID[0]}&share_id=${sID[0]}#share_${sID[0]}" target="_blank">评论该分享</a>
		</div>
	</div>
<#include "1,1">
</#compress>