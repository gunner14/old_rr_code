${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<div class="figure">
		<a class="avatar" href="http://org.renren.com/${fID[0]}?ref=${vType}" target="_blank"><img needclip="1" width="50" height="50" lala="${(h[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" /></a>
	</div>
	<h3>
		<#list fName as n>
			<a target="_blank"  href="http://org.renren.com/${fID[n_index]}&ref=${vType}">${n}</a>
		</#list>
		成为
		<a href="http://org.renren.com/${uID[0]}?ref=${vType}" target="_blank">${uName[0]}</a> 
		俱乐部的管理员

	</h3>
	<div class="details">
		<div class="legend">
			<img lala="http://xnimg.cn/imgpro/icons/pageclub.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>