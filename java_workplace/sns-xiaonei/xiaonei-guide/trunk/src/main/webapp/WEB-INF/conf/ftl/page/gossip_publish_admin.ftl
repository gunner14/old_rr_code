${"\r"}
<#compress>
<#assign page = "1"/>
	<#include "1,0">
	<li id="feed${id[0]}">
		<#include "1,4">
		<h3>
			<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
			<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
			在 <a href="http://page.renren.com/${fID[0]}?id=${fID[0]}&s=gossip" target="_blank">留言板</a> 回复了粉丝
		</h3>
		<div class="details">
			<div class="legend">
				<img src="http://s.xnimg.cn/a.gif"" alt="留言" class="iPost" />
				<span class="duration"><@format>${time[0]}</@format></span>
				<#include "1,5">
			</div>
		</div>
		<#include "1,1">
	</li>
</#compress>