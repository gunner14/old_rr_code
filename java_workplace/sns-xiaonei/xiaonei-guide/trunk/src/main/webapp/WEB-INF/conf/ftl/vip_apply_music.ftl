${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 上传了音乐 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}">${title[0]}</a>，去听听吧
	</h3>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="VIP" title="音乐" class="iVipmusic" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
