${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 发表日志 <a href="${(url[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank" id="title${id[0]}">${title[0]}</a>
	<#if wap?exists>
		<span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span>
	</#if>
	<#if source[0] == "2">
		<a href="http://i.renren.com/privilege/letter.action?wc=210000" target="_blank"><img lala="http://s.xnimg.cn/imgpro/editor/letter.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="附有信纸" /></a>
	</#if>
	</h3>
	<div class="content">
		<#if video?exists>
			<div class="blogMedia"> 
				<div style="background-image: url(${vSrc[0]});" class="video">
					<a target="_blank" href="${url[0]}">播放</a>
				</div>
			</div>
		</#if>
		<#if pic?exists>
			<div class="blogMedia">
				<a target="_blank" href="${url[0]}"><img width="100" class="photo" lala="${src[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a>
			</div>
		</#if>
		<#if music?exists>
			<div class="blogMedia">
				<div class="audio playmedia">
					<a target="_blank" href="${url[0]}" class="pl-btn">播放音乐</a>
				</div>
			</div>
		</#if>
		<blockquote>${brief[0]}</blockquote>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="日志" src="http://s.xnimg.cn/a.gif" class="iBlog"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#nogo" id="replyKey${blogID[0]}" onClick="getReplyOfTheDoing('${blogID[0]}','${fID[0]}','${userID[0]}','f',false,'0','','blog')">
				<span id="replyCount${blogID[0]}"><#if ((count[0])?number > 0)>${count[0]}条</#if></span>回复
			</a>
			<span class="seperator">|</span>
				<#if selfzancount?exists>
					<a href="#nogo" id="ILike${blogID[0]}" onclick="ILike_toggleUserLike('blog','${blogID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					不赞
					</a>
					<#else>
					<a href="#nogo" class="ilike_icon" id="ILike${blogID[0]}" onclick="ILike_toggleUserLike('blog','${blogID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					赞
					</a>
				</#if>
		</div>
		<div id="media${blogID[0]}"></div>
		<div id="feedbody${blogID[0]}" style="display:none;" class="replies"></div>
		<#assign isShow="false"/>
		<#if showJson?exists>
			<#assign isShow="true"/>
		</#if>
			<script language="Javascript" status='1'> 
				(function()
				{
					var reply${blogID[0]} = {"replyList":[
						<#list rid as id>
							<#if 2==id_index><#break></#if>
							{"id":"${id}","replyer_tinyurl":"${tu[id_index]}","replyContent":${con[id_index]},"ubname":"${un[id_index]}","replyTime":"${rt[id_index]}","type":${ty[id_index]},"reply_layer":${la[id_index]},"ubid":${ui[id_index]}}
							<#if (rid?size > 1) && 0==id_index>,</#if>
						</#list>
						],"ownerid":${fID[0]},"userDigged":"${selfzancount[0]}","digged":"${totalzancount[0]}","type":"blog","guestName":"${userName[0]}"};
					getReplyOfDoingFromJSON( reply${blogID[0]} , '${blogID[0]}' ,'${userID[0]}' , '${count[0]}','0' ,'','',${isShow})
				})();
			</script>
	</div>
<#include "1,1">
</li>
</#compress>
