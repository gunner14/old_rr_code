${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
		<#if desc?exists>
			: ${desc[0]}			
		<#else>
			的照片 <a href="http://photo.renren.com/getalbum.do?id=${aID[0]}&owner=${fID[0]}&ref=${vType}" target="_blank">${aName[0]}</a>
		</#if>
	</h3>
	<div class="content">
			<a href="http://photo.renren.com/getphoto.do?id=${pID[0]}&owner=${fID[0]}&ref=${vType}" target="_blank"><img lala="${src[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="相册" src="http://s.xnimg.cn/a.gif"" class="iPhoto"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>