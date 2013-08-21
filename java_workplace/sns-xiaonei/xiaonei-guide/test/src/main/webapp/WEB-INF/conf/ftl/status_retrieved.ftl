<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>: ${title[0]} 
	<#if wap?exists><span class="from">通过<a href="http://mobile.renren.com/${vType}">手机</a>更新</span></#if></h3>
	<#if hLink?exists> - 来自<a href="${hLink[0]}">${hText[0]}</a> <img src="${(hImg[0])!"about:blank"}"/> </#if>
	<#if tV?exists>
		<h3><a target="_blank" href="http://goto.renren.com/share_redirect?url=${url[0]}&ref=${vType}">${tV[0]}</a></h3>
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
		    <a target="_blank" href="${url[0]}">${tA[0]}</a>
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
			<a href="http://photo.renren.com/getphoto.do?id=${pID[0]}&owner=${fID[0]}" target="_blank"><img width="200" src="${mSrc[0]}" class="photo"/></a>
	    </div>
	</#if>
	<div class="details">
		<div class="legend"><img alt="状态" src="http://xnimg.cn/a.gif" class="iStatus"/>
		<span class="duration"><@format>${dtime[0]}</@format></span>
		<a href="#nogo" id="replyKey${dID[0]}" onClick="getReplyOfTheDoing('${dID[0]}','${fID[0]}','${userID[0]}','f',false,'3')">
			<span id="replyCount${dID[0]}"><#if ((count[0])?number > 0)>${count[0]}条</#if></span>回复
		</a>
		
		</div>
		<div id="media${dID[0]}"></div>
		<div id="feedbody${dID[0]}" style="display:none;" class="replies"></div>
		<script language="Javascript" status='1'> 
			(function()
			{
				var reply${dID[0]} = {"replyList":[
					<#list rid as id>
						<#if 2==id_index><#break></#if>
						{"id":"${id}","replyer_tinyurl":"${tu[id_index]}","replyContent":${con[id_index]},"ubname":"${un[id_index]}","replyTime":"${rt[id_index]}","type":${ty[id_index]},"reply_layer":${la[id_index]},"ubid":${ui[id_index]}}
						<#if (rid?size > 1) && 0==id_index>,</#if>
					</#list>
					],"ownerid":${fID[0]}};
				getReplyOfDoingFromJSON( reply${dID[0]} , '${dID[0]}' ,'${userID[0]}' , '${count[0]}','3' )
			})();
		</script>
	</div>
<#include "1,1">
</li>
</#compress>
