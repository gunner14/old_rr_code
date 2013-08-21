${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<div class="figure">
		<a class="avatar" href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank"><img needclip="1" width="50" height="50" lala="${(h[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" /></a>
	</div>
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}">${fName[0]}</a>
		评论了日志 <a href="${url[0]}&ref=${vType}" target="_blank">${title[0]}</a>
	</h3>
	<div class="content">
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="评论" class="iPost" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>