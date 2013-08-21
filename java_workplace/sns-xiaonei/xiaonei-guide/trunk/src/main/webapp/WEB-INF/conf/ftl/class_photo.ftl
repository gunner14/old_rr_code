${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a target="_blank" href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}">${fName[0]}</a> 上传了
			<#if count?exists>
				<#assign num = 0/>
				<#list count as c>
					<#assign num=num+c?number>
				</#list>
				${num} 张
			</#if>
			照片至 <a href="${(curl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${cname[0]}</a>
	</h3>
	<div class="content">
		<#list src as aSrc>
			<#if 4==aSrc_index><#break></#if>
			<a href="${(purl[aSrc_index])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${aSrc}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
		</#list>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="班级" class="iClass" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>