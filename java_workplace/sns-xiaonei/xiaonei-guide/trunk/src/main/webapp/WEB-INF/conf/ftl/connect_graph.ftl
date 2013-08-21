${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 
		分享了 <a href="${pSrc[0]}" target="_blank">${pWhere[0]}</a>
		图片 <a href="${sSrc[0]}" target="_blank">${headLine[0]}</a>
	</h3>
	<div id="moreThumbnails${id[0]}" class="content">
		<#list src as aSrc>
			<#if 3==aSrc_index><#break></#if>
			<a href="${url[aSrc_index]}&ref=${vType}" target="_blank"><img width="100" lala="${aSrc}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
		</#list>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
		<#assign photoIDs=""/>
		<#list pId as aPid>
				<#if aPid_index ==20><#break></#if>
				<#assign photoIDs=photoIDs + aPid +","/>
		</#list>
		<span style="display: none;" id="morePicFeed${id[0]}">${photoIDs}</span>
		<#if numm?exists>
            <a href="#nogo" class="more" onclick="moreFeedPic('${id[0]}','${fID[0]}')">查看更多</a>
        </#if>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="图片" src="http://s.xnimg.cn/a.gif" class="iPhoto"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="${pSrc}" target="_blank">我也要连接：${pWhere}</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
