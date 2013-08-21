${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 
	    分享了 <a href="${pSrc[0]}" target="_blank">${pWhere[0]}</a>
	     内容 <a href="${sSrc[0]}" target="_blank">${headLine[0]}</a>
	</h3>
	<div class="content">
	    <#if graph?exists>
	        <div>
	            <img lala="${graph[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
	        </div>
	    </#if>
		<blockquote>${brief[0]}</blockquote>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="文字" src="http://s.xnimg.cn/a.gif" class="iBlog"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="${pSrc[0]}" target="_blank">我也要连接： ${sWhere[0]}</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
