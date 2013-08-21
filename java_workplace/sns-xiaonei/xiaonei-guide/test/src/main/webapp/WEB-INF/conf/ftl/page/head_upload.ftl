${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
		修改了头像
	</h3>
	<div class="content">
		<a target="_blank" href="http://page.renren.com/${fID[0]}?ref=${vType}"><img class="photo" lala="${ (src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" class="iProfile" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
		</div>
	</div>
<#include "1,1">
</li>
</#compress>