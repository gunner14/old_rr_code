${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>: ${title[0]} 
	<#if wap?exists><span class="from">通过<a href="http://mobile.renren.com/?${vType}" target="_blank">手机</a>更新 <a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif"" alt="通过手机发布" class="iTinyphone" /></a></span></#if></h3>
	<#if hLink?exists> - 来自<a href="${hLink[0]}" target="_blank">${hText[0]}</a> <img lala="${(hImg[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/> </#if>
	<#if tV?exists>
		<h3><a target="_blank" href="http://share.renren.com/share_redirect.do?url=${url[0]}&ref=${vType}">${tV[0]}</a></h3>
	    <div class="content">
		    <div class="video">
			    <#if !(scale?exists) || scale[0] == "">
				    <#assign s="1.2">
				<#else>
				    <#assign s=scale[0]>
			    </#if>
			    <a href="#nogo" onclick="playFeedVideo('${sID[0]}','${id[0]}','${fID[0]}',${s});this.style.display='none';">播放</a>
		    </div>
	    </div>
    </#if>
	<#if tA?exists>
	    <h3>
		    <a target="_blank" href="http://share.renren.com/share_redirect.do?url=${url[0]}&ref=${vType}">${tA[0]}</a>
	    </h3>
	    <div class="content">
		    <div class="audio playmedia">
			    <a class="pl-btn" onclick="playFeedAudio('${id[0]}','${mType[0]}','${url[0]}');this.style.display='none';" href="#nogo">播放音乐</a>
		    </div>
	    </div>
	</#if>
	
	<#if eurl?exists>
	   <h3>
		    <a target="_blank" href="http://goto.renren.com/share_redirect?url=${eurl[0]}">${url[0]}</a>
	   </h3>
	</#if>
	
	<#if mSrc?exists>
	    <div class="content">
			<a href="http://photo.renren.com/getphoto.do?id=${pID[0]}&owner=${fID[0]}" target="_blank"><img width="200" lala="${mSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
	    </div>
	</#if>
	<div class="details">
		<div class="legend"><img alt="状态" src="http://s.xnimg.cn/a.gif"" class="iStatus"/>
		<span class="duration"><@format>${dtime[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>