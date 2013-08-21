${"\r"}
<#compress>
<#assign num = 0/>
<#list count as c>
	<#assign num=num+c?number>
</#list>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 在 ${num} 张照片中被圈出来了
	</h3>
	<div class="content">
		<div class="figure">
			<a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
		</div>
		<#if title[0]?exists><p>${title[0]}</p></#if>
		<p>所属相册: <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${aTitle[0]}</a></p>
		<p>来自: <a href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}" target="_blank">${uName[0]}</a> </p>
		<p><a href="${(moreUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">更多${fName[0]}被圈的照片 >> </a></p>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="圈人" class="iPhoto" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>