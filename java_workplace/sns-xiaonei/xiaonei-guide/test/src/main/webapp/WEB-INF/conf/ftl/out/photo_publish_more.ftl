${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 上传了${num}张照片至 <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${aName[0]}</a>
		<#if wap?exists><span class="from">通过<a href="http://mobile.renren.com/?${vType}" target="_blank">手机</a>发布 <a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif"" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
		<#if appIconSrc?exists><span class="from"><a href="${appUrl[0]}" target="_blank"><img lala="${appIconSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="通过手机发布" /></a></span></#if>
	</h3>
	<div id="moreThumbnails${id[0]}" class="content">
		<#list src as aSrc>
			<#if 3==aSrc_index><#break></#if>
			<a href="${(url[aSrc_index])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img width="100" lala="${aSrc}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
		</#list>
		<#assign photoIDs=""/>
		<#list pId as aPid>
				<#if aPid_index ==20><#break></#if>
				<#assign photoIDs=photoIDs + aPid +","/>
		</#list>
		<span style="display: none;" id="morePicFeed${id[0]}">${photoIDs}</span>
		<#if (num > 3) && photoIDs!="">
			<a href="#nogo" class="more" onclick="moreFeedPic('${id[0]}','${fID[0]}')">查看更多</a>
		</#if>
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