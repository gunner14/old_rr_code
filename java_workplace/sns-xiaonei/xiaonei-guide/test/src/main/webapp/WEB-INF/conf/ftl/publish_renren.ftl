${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a>
		正在关注<a href="http://www.renren.com/renren/index.html" target="_blank">校内网改名为人人网</a>的最新动态
	</h3>
		<div class="content">
			<div class="figure"><a href="http://www.renren.com/renren/index.html" target="_blank"><img class="photo" lala="http://xnimg.cn/imgpro/homeAd/renren100.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		</div>
	<div class="details">
		<div class="legend">
			<img lala="http://xnimg.cn/imgpro/icons/renren.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="http://www.renren.com/renren/index.html" target="_blank">去了解下>></a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>