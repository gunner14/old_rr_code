${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 上传了${num}张照片至 <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank" id="title${id[0]}">${aName[0]}</a>
		<#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
		<#if appIconSrc?exists><span class="from"><a href="${appUrl[0]}" target="_blank"><img lala="${appIconSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="通过手机发布" /></a></span></#if>
	</h3>
	<div id="moreThumbnails${id[0]}" class="content">
		<#list src as aSrc>
			<#if 3==aSrc_index><#break></#if>
			<a href="${(url[aSrc_index])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank"><img width="100" lala="${aSrc}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
		</#list>
		<#assign photoIDs=""/>
		<#list pId as aPid>
				<#if aPid_index ==20><#break></#if>
				<#assign photoIDs=photoIDs + aPid +","/>
		</#list>
		<span style="display: none;" id="morePicFeed${id[0]}">${photoIDs}</span>
		<#if (num > 3) && photoIDs!="">
			<a href="#nogo" class="more" onclick="moreFeedPic('${id[0]}','${fID[0]}')">查看更多</a>
		</#if>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="相册" src="http://s.xnimg.cn/a.gif" class="iPhoto"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#nogo" id="replyKey${aID[0]}" onClick="getReplyOfTheDoing('${aID[0]}','${fID[0]}','${userID[0]}','f',false,'2','','album')">
				<span id="replyCount${aID[0]}"><#if ((commentcount[0])?number > 0)>${commentcount[0]}条</#if></span>回复
			</a>
			<span class="seperator">|</span>
				<#if selfzancount?exists>
					<a href="#nogo" id="ILike${aID[0]}" onclick="ILike_toggleUserLike('album','${aID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					不赞
					</a>
					<#else>
					<a href="#nogo" class="ilike_icon" id="ILike${aID[0]}" onclick="ILike_toggleUserLike('album','${aID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					赞
					</a>
				</#if>
		</div>
		<div id="media${aID[0]}"></div>
		<div id="feedbody${aID[0]}" style="display:none;" class="replies"></div>
		<#assign isShow="false"/>
		<#if showJson?exists>
			<#assign isShow="true"/>
		</#if>
			<script language="Javascript" status='1'> 
			(function()
			{
				var reply${aID[0]} = {"replyList":[
					<#list rid as id>
						<#if 2==id_index><#break></#if>
						{"id":"${id}","replyer_tinyurl":"${tu[id_index]}","replyContent":${con[id_index]},"ubname":"${un[id_index]}","replyTime":"${rt[id_index]}","type":${ty[id_index]},"reply_layer":${la[id_index]},"ubid":${ui[id_index]}}
						<#if (rid?size > 1) && 0==id_index>,</#if>
					</#list>
					],"ownerid":${fID[0]},"userDigged":"${selfzancount[0]}","digged":"${totalzancount[0]}","type":"album","guestName":"${userName[0]}"};
				getReplyOfDoingFromJSON( reply${aID[0]} , '${aID[0]}' ,'${userID[0]}' , '${commentcount[0]}','2','','',${isShow} )
			})();
			</script>
	</div>
<#include "1,1">
</li>
</#compress>
