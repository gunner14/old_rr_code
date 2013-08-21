${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
	    <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 
		分享了<a href="${pSrc[0]}" target="_blank">${pWhere[0]}</a>
		音频 <a href="${sSrc[0]}" target="_blank">${headLine[0]}</a>
	</h3>
	<div class="content">
		<div class="audio playmedia">
			<a class="pl-btn" onclick="playFeedAudio('${id[0]}','${mType[0]}','${url[0]}');this.style.display='none';" href="#nogo">播放音乐</a>
		</div>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="音频" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="${pSrc[0]}" target="_blank">我也要连接：${pWhere[0]}</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
