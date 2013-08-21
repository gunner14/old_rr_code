${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 分享链接
		<a target="_blank" href="http://share.renren.com/share/GetShare.do?id=${sID[0]}&owner=${fID[0]}"><#if title?exists>${title[0]}<#else>${url[0]}</#if></a>
	</h3>
	<div class="content">
		<#if comm?exists && comm[0] != "">
			<p class="comment"><q>${comm[0]}</q></p>
		</#if>
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