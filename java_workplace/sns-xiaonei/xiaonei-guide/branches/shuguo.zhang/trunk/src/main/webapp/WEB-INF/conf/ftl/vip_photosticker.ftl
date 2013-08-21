${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
		<#if v?exists>
			<a href="http://i.renren.com/click.action?from=40040&amp;url=%2F" class="vip-icon" target="_blank"><img lala="http://s.xnimg.cn/imgpro/icons/vip_s.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="VIP" class="vip"/></a>
		</#if>
		更换了大头贴头像
	</h3>
	<div class="content">
		<a target="_blank" href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}"><img class="photo" lala="${ (src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="VIP" title="大头贴" class="iVippic" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
