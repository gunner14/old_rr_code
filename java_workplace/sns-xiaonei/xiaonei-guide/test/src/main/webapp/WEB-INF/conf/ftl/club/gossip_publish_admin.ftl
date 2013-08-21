${"\r"}
<#compress>
<#assign page = "1"/>
	<#include "1,0">
	<li id="feed${id[0]}">
		<div class="figure">
			<a class="avatar" href="http://org.renren.com/${fID[0]}?ref=${vType}" target="_blank"><img needclip="1" width="50" height="50" lala="${(h[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" /></a>
		</div>
		<h3>
			<a href="http://org.renren.com/${fID[0]}?ref=${vType}" target="_blank">${fName[0]}</a>
			在 <a href="${url[0]}&s=gossip" target="_blank">留言板</a> 回复了成员
		</h3>
		<div class="details">
			<div class="legend">
				<img src="http://s.xnimg.cn/a.gif"" alt="留言" class="iPost" />
				<span class="duration"><@format>${time[0]}</@format></span>
			</div>
		</div>
		<#include "1,1">
	</li>
</#compress>