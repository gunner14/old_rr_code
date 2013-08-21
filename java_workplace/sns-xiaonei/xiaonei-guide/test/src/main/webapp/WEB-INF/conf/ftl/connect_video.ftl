${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
	    <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 
		分享了
		<#if appId?exists>
			<a href="http://www.connect.renren.com/share/redirect.do?loc=1&app_id=${appId[0]}" target="_blank">${pWhere[0]}</a>
		<#else>
			<a href="${pSrc[0]}" target="_blank">${pWhere[0]}</a>
		</#if>
		视频<a href="${sSrc[0]}" target="_blank">${headLine[0]}</a>
	</h3>
	<div class="content">
		<div class="video" style="background-image: url(${thumb[0]});">
			<a href="http://share.renren.com/share/GetShare.do?id=${sID[0]}&owner=${fID[0]}&ref=${vType}" target="_blank">播放</a>
		</div>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
		    <img lala="${icon[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="分享"  />
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#nogo" id="replyKey${sID[0]}" onClick="getReplyOfTheDoing('${sID[0]}','${fID[0]}','${userID[0]}','f',false,'4')">
				<span id="replycommentCount${sID[0]}"><#if ((cCount[0])?number > 0)>${cCount[0]}条</#if></span>回复
			</a>
			<span class="seperator">|</span>
			<a href="#nogo" onclick="create_share_div(${sID[0]},${fID[0]},${fID[0]});" >分享</a>
			<span class="seperator">|</span>
			<#if appId?exists>
				<a href="http://www.connect.renren.com/share/redirect.do?loc=2&app_id=${appId[0]}" target="_blank">连接：${pWhere[0]}</a>
			<#else>
				<a href="${pSrc[0]}" target="_blank">连接：${pWhere[0]}</a>
			</#if>
		</div>
		<div id="media${sID[0]}"></div>
		<div id="feedbody${sID[0]}" style="display:none;" class="replies"></div>
		<#assign isShow="false"/>
		<#if showJson?exists>
			<#assign isShow="true"/>
		</#if>
		<script language="Javascript" status='1'> 
			(function()
			{
				var reply${sID[0]} = {"replyList":[
					<#list rid as id>
						<#if 2==id_index><#break></#if>
						{"id":"${id}","replyer_tinyurl":"${tu[id_index]}","replyContent":${con[id_index]},"ubname":"${un[id_index]}","replyTime":"${rt[id_index]}","type":${ty[id_index]},"reply_layer":${la[id_index]},"ubid":${ui[id_index]}}
						<#if (rid?size > 1) && 0==id_index>,</#if>
					</#list>
					],"ownerid":${fID[0]}};
				getReplyOfDoingFromJSON( reply${sID[0]} , '${sID[0]}' ,'${userID[0]}' , '${cCount[0]}','4','','',${isShow})
			})();
		</script>
	</div>
<#include "1,1">
</li>
</#compress>
