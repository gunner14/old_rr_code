${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list> 
		<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
		分享音乐 <a target="_blank" href="${url[0]}">${title[0]}</a>
	</h3>
	<div class="content">
		<p>歌手: ${singer[0]}</p>
		<div class="audio playmedia">
			<a class="pl-btn" onclick="playFeedAudio('${id[0]}','${mType[0]}','${url[0]}');this.style.display='none';" href="#nogo">播放音乐</a>
		</div>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif"" alt="分享" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<#include "1,5">
			<a href="http://page.renren.com/share/list?pid=${pageId[0]}&share_id=${sID[0]}#${sID[0]}" target="_blank">评论该分享</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>