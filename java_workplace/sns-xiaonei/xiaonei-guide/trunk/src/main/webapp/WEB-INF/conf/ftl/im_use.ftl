${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
	<#list fName as n>
		<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}">${n}</a>
	</#list> 安装了 <a href="http://im.renren.com/?ref=feedlogo" target="_blank">“校内通”</a> 软件
	</h3>
	<div class="content">
		<a href="http://im.renren.com/?ref=feedlogo" target="_blank"><img class="photo" lala="http://xnimg.cn/imgpro/chat/logo2.png" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="校内通"/></a>
		<p>校内人专用的即时聊天软件，<br/>实时收到新鲜事，第一时间知道好友的动态。</p>
		<p><a href="http://im.renren.com/?ref=feedmore" target="_blank">了解更多</a><span class="pipe">|</span><a href="http://im.renren.com/xnsetup.exe" target="_blank">立即下载</a></p>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="校内通" class="iIm" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
