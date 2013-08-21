${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 分享应用 <a target="_blank" href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}">${title[0]}</a>
	</h3>
	<div class="content">
		<div class="figure"><a target="_blank" href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}"><img class="photo" lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<p>类别: ${cate[0]}</p>
		<p>安装: ${count[0]} 人</p>
		<meter class="rating" min="0" max="5" value="${star[0]}">${star[0]}星</meter>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
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