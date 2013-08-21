${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 给 
		<a href="${(url[0])?replace('xiaonei.com','renren.com')}" target="_blank">${title[0]}</a>
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if> 留言了
	</h3>
	<#if comm?exists && comm[0] != "">
		<div class="content">
			<p>
				<q>${comm[0]}</q>
			</p>
		</div>
	</#if>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="留言" class="iPost" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
		</div>
	</div>
<#include "1,1">
</li>
</#compress>