${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>: ${title[0]} 
	<#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
	<#if hLink?exists><span class="from">通过<a href="${hLink[0]}" target="_blank">${hText[0]}</a>发布</span></#if>
</h3>
	<div class="details">
		<div class="legend"><img alt="状态" src="http://s.xnimg.cn/a.gif" class="iStatus"/>
		<span class="duration"><@format>${dtime[0]}</@format></span>
		<a href="#nogo" id="replyKey${dID[0]}" onClick="getReplyOfTheDoing('${dID[0]}','${fID[0]}','${userID[0]}','f',false,'3','','status')">
			<span id="replyCount${dID[0]}"><#if ((count[0])?number > 0)>${count[0]}条</#if></span>回复
		</a>
		<#if vType != "minifeed" >
			<span class="seperator">|</span>
			<a href="#nogo" onclick="forwardDoing('${dID[0]}', '${fID[0]}');" >转发</a>
		<#elseif userID[0] != interviewee[0]>
			<span class="seperator">|</span>
			<a href="#nogo" onclick="a_fowardDoing('${dID[0]}', '${fID[0]}');return false;">转发</a>
		</#if>
		</div>
		<div id="media${dID[0]}"></div>
		<div id="feedbody${dID[0]}" style="display:none;" class="replies"></div>
		<#assign isShow="false"/>
		<#if showJson?exists>
			<#assign isShow="true"/>
		</#if>
		<script language="Javascript" status='1'> 
				(function()
				{
					var reply${dID[0]} = {"replyList":[
						<#list rid as id>
							<#if 2==id_index><#break></#if>
							{"id":"${id}","replyer_tinyurl":"${tu[id_index]}","replyContent":${con[id_index]},"ubname":"${un[id_index]}","replyTime":"${rt[id_index]}","type":${ty[id_index]},"reply_layer":${la[id_index]},"ubid":${ui[id_index]}}
							<#if (rid?size > 1) && 0==id_index>,</#if>
						</#list>
						],"ownerid":${fID[0]},"type":"status"};
					getReplyOfDoingFromJSON( reply${dID[0]} , '${dID[0]}' ,'${userID[0]}' , '${count[0]}','3','','',${isShow} )
				})();
		</script>
	</div>
<#include "1,1">
</li>
</#compress>