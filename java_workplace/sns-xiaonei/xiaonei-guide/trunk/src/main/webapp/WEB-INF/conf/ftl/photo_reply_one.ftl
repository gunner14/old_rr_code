${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
		<#if desc?exists>
			: ${desc[0]}			
		<#else>
			的照片 <a href="http://photo.renren.com/getalbum.do?id=${aID[0]}&owner=${fID[0]}&ref=${vType}" target="_blank" id="title${id[0]}">${aName[0]}</a>
		</#if>
	</h3>
	<div class="content">
			<a href="http://photo.renren.com/getphoto.do?id=${pID[0]}&owner=${fID[0]}&ref=${vType}" target="_blank"><img lala="${src[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="相册" src="http://s.xnimg.cn/a.gif" class="iPhoto"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#nogo" id="replyKey${pID[0]}" onClick="getReplyOfTheDoing('${pID[0]}','${fID[0]}','${userID[0]}','f',false,'1','','photo')">
				<span id="replyCount${pID[0]}"><#if ((commentcount[0])?number > 0)>${commentcount?last}条</#if></span>回复
			</a>
			<span class="seperator">|</span>
				<#if selfzancount?exists>
					<a href="#nogo" id="ILike${pID[0]}" onclick="ILike_toggleUserLike('photo','${pID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					不赞
					</a>
					<#else>
					<a href="#nogo" class="ilike_icon" id="ILike${pID[0]}" onclick="ILike_toggleUserLike('photo','${pID[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
					赞
					</a>
				</#if>
		</div>
		<div id="media${pID[0]}"></div>
		<div id="feedbody${pID[0]}" style="display:none;" class="replies"></div>
		<#assign isShow="false"/>
		<#if showJson?exists>
			<#assign isShow="true"/>
		</#if>
			<script language="Javascript" status='1'> 
			(function()
			{
				var reply${pID[0]} = {"replyList":[
					<#if rid?size == 1 || (commentcount?last)?number == 1>
					{"id":"${rid[0]}","replyer_tinyurl":"${tu[0]}","replyContent":${con[0]},"ubname":"${un[0]}","replyTime":"${rt[0]}","type":${ty[0]},"reply_layer":${la[0]},"ubid":${ui[0]}}
					<#elseif rid?size==2>
					{"id":"${rid[0]}","replyer_tinyurl":"${tu[0]}","replyContent":${con[0]},"ubname":"${un[0]}","replyTime":"${rt[0]}","type":${ty[0]},"reply_layer":${la[0]},"ubid":${ui[0]}},
					{"id":"${rid[1]}","replyer_tinyurl":"${tu[1]}","replyContent":${con[1]},"ubname":"${un[1]}","replyTime":"${rt[1]}","type":${ty[1]},"reply_layer":${la[1]},"ubid":${ui[1]}}
					<#elseif rid?size==3>
					{"id":"${rid[1]}","replyer_tinyurl":"${tu[1]}","replyContent":${con[1]},"ubname":"${un[1]}","replyTime":"${rt[1]}","type":${ty[1]},"reply_layer":${la[1]},"ubid":${ui[1]}},
					{"id":"${rid[2]}","replyer_tinyurl":"${tu[2]}","replyContent":${con[2]},"ubname":"${un[2]}","replyTime":"${rt[2]}","type":${ty[2]},"reply_layer":${la[2]},"ubid":${ui[2]}}
					<#elseif rid?size==4>		
					{"id":"${rid[2]}","replyer_tinyurl":"${tu[2]}","replyContent":${con[2]},"ubname":"${un[2]}","replyTime":"${rt[2]}","type":${ty[2]},"reply_layer":${la[2]},"ubid":${ui[2]}},
					{"id":"${rid[3]}","replyer_tinyurl":"${tu[3]}","replyContent":${con[3]},"ubname":"${un[3]}","replyTime":"${rt[3]}","type":${ty[3]},"reply_layer":${la[3]},"ubid":${ui[3]}}
					</#if>
					],"ownerid":${fID[0]},"userDigged":"${selfzancount[0]}","digged":"${totalzancount[0]}","type":"photo","guestName":"${userName[0]}"};
				getReplyOfDoingFromJSON( reply${pID[0]} , '${pID[0]}' ,'${userID[0]}' , '${commentcount?last}','1','','',${isShow})
			})();
			</script>
		
	</div>
<#include "1,1">
</li>
</#compress>