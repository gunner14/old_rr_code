${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 分享影评 <a href="${(curl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><strong>${ctitle[0]}</strong></a>
	</h3>
	<div class="content">
		<div class="figure"><a href="${(curl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${(msrc[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<meter class="rating" min="0" max="5" value="${star[0]}">${star[0]}星</meter>
		<blockquote>${msg[0]}</blockquote>
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
