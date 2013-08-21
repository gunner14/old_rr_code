${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>${title[0]}</h3>
	<#if body?exists>
		<div class="content">
			<p>${body[0]}</p>
		</div>
	</#if>
	<div class="details">
		<div class="legend">
			<img lala="${(icon[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
