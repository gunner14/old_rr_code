${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a>${title[0]}</h3>
	<div class="content">
		<#if src?exists>
			<#list src as aSrc>
				<#if 1==aSrc_index><#break></#if>
				<div class="figure">
					<a href="${(url[aSrc_index])?replace('xiaonei.com','renren.com')}" target="_blank"><img class="photo" width="100" lala="${aSrc}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
				</div>
			</#list>
		</#if>
		<#if desc?exists>
			<p style="color:#515151;margin:0 0 5px 0px;">${desc[0]}</p>
		</#if>
		<#if cont?exists>
			<p style="margin-left:0px;">${cont[0]}</p>
		</#if>
	</div>
	<div class="details">
		<div class="legend">
			<img lala="${(icon[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<#if linkurl?exists>
				<a href="${(linkurl[0])?replace('xiaonei.com','renren.com')}" target="_blank">${linkName[0]}</a>
			</#if>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
