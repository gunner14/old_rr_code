${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 发表影评 <a href="${(curl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${ctitle[0]}</a>
	</h3>
	<div class="content">
		<div class="figure">
			<a href="${(murl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img class="photo" lala="${(msrc[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
		</div>
		<p><a href="${(murl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><strong>${mtitle[0]}</strong></a></p>
		<meter class="rating" min="0" max="5" value="${star[0]}">${star[0]}星</meter>
		<p>${ (msg[0])?html}</p>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="影评" class="iFilm" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
