${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 分享好友 <a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}">${uName[0]}</a>
	</h3>
	<div class="content">
		<div class="figure">
			<a target="_blank" href="http://renren.com/profile.do?id=${uID[0]}&ref=${vType}"><img lala="${ (src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="${uName[0]}"/></a>
		</div>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="分享" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</#compress>