${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
		发表日志 <a href="http://page.renren.com/note/singleNote?pid=${fID[0]}&id=${blogID[0]}&ref=${vType}" target="_blank">${title[0]}</a>
	</h3>
	<div class="content">
		<blockquote>${brief[0]}</blockquote>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="日志" src="http://s.xnimg.cn/a.gif"" class="iBlog"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
		</div>
	</div>
<#include "1,1">
</li>
</#compress>