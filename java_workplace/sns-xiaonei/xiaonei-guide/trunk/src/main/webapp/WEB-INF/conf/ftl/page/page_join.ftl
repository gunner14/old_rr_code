${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank"  href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list>
		在人人成为了 <a href="${(url[0])?replace('xiaonei.com','renren.com')}?ref=${vType}" target="_blank">${title[0]}</a> 
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
		的
		<#if fansName?exists>
			${fansName[0]}
		<#else>
			粉丝
		</#if>
	</h3>
	<#if src?exists>
		<div class="content">
			<div class="figure"><a href="${(url[0])?replace('xiaonei.com','renren.com')}?ref=${vType}" target="_blank"><img class="photo" lala="${ (src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
			<#if brief?exists && brief[0] != ""><p class="comment"><q>${brief[0]}</q></p></#if>
		</div>
	</#if>
	<div class="details">
		<div class="legend">
			<img lala="http://xnimg.cn/imgpro/icons/follow-add.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
			<a href="http://page.renren.com/pa/bf?pid=${pageId[0]}&ref=${vType}" target="_blank">
			<#if bfName?exists>
				${bfName[0]}
			<#else>
				成为粉丝
			</#if>
			</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>