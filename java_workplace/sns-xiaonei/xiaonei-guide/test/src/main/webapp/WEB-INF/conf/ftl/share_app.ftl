${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 分享应用 <a target="_blank" href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}">${title[0]}</a>
	</h3>
	<div class="content">
		<div class="figure"><a target="_blank" href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}"><img class="photo" lala="${(src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<p>类别: ${cate[0]}</p>
		<p>安装: ${count[0]} 人</p>
		<meter class="rating" min="0" max="5" value="${star[0]}">${star[0]}星</meter>
		<#if comm?exists && comm[0] != ""><p class="comment"><q>${comm[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="分享" class="iShare" />
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#nogo" id="replyKey${sID[0]}" onClick="getReplyOfTheDoing('${sID[0]}','${fID[0]}','${userID[0]}','f',false,'4')">
				<span id="replycommentCount${sID[0]}"><#if ((cCount[0])?number > 0)>${cCount[0]}条</#if></span>回复
			</a>
			<span class="seperator">|</span>
			<a target="_blank" href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}">安装此应用</a>
			<span class="seperator">|</span>
				<#if selfzancount?exists>
					<a href="#nogo" id="ILike${sID[0]}" onclick="ILike_toggleUserLike('share','${sID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					不赞
					</a>
					<#else>
					<a href="#nogo" class="ilike_icon" id="ILike${sID[0]}" onclick="ILike_toggleUserLike('share','${sID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					赞
					</a>
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
					],"ownerid":${fID[0]},"userDigged":"${selfzancount[0]}","digged":"${totalzancount[0]}","type":"share","guestName":"${userName[0]}"};
				getReplyOfDoingFromJSON( reply${sID[0]} , '${sID[0]}' ,'${userID[0]}' , '${cCount[0]}','4','','',${isShow})
			})();
		</script>
	</div>
<#include "1,1">
</li>
</#compress>
