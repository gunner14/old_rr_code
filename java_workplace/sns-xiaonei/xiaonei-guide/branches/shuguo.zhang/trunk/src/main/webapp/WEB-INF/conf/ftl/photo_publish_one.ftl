${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
	    <#if status?exists>
                <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}:</a> ${status[0]}
         <#else>
                <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
				<#if source?exists && source[0] == "1">
					使用<a href="http://i.renren.com/pe/home.action?ref=${vType}" target="_blank">照片美容工坊</a>美化了一张照片
				<#elseif source?exists && source[0] == "2">
					使用<a href="http://avatar.renren.com/avatar" target="_blank">照片美容工坊</a>制作了一张自己的阿凡达照片，<a href="http://avatar.renren.com/avatar" target="_blank">我也来试试！</a>
				<#else>
					上传了${num}张照片至 <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank" id="title${id[0]}">${aName[0]}</a>
				</#if>
					
         </#if>
		 <#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
		 <#if appIconSrc?exists><span class="from"><a href="${appUrl[0]}" target="_blank"><img lala="${appIconSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="通过手机发布"/></a></span></#if>
	</h3>
	<div class="content">
			<a href="http://photo.renren.com/getphoto.do?id=${pId[0]}&owner=${fID[0]}&ref=${vType}" target="_blank"><img lala="${mSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="相册" src="http://s.xnimg.cn/a.gif" class="iPhoto"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="#nogo" id="replyKey${pId[0]}" onClick="getReplyOfTheDoing('${pId[0]}','${fID[0]}','${userID[0]}','f',false,'1','','photo')">
				<span id="replyCount${pId[0]}"><#if ((commentcount[0])?number > 0)>${commentcount[0]}条</#if></span>回复
			</a>
			<span class="seperator">|</span>
			<#if selfzancount?exists>
				<a href="#nogo" id="ILike${pId[0]}" onclick="ILike_toggleUserLike('photo','${pId[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
				不赞
				</a>
				<#else>
				<a href="#nogo" class="ilike_icon" id="ILike${pId[0]}" onclick="ILike_toggleUserLike('photo','${pId[0]}','${userID[0]}','${fID[0]}','${userName[0]}')" >
				赞
				</a>
			</#if>
		</div>
		<div id="media${pId[0]}"></div>
		<div id="feedbody${pId[0]}" style="display:none;" class="replies"></div>
		<#assign isShow="false"/>
		<#if showJson?exists>
			<#assign isShow="true"/>
		</#if>
			<script language="Javascript" status='1'> 
			(function()
			{
				var reply${pId[0]} = {"replyList":[
					<#list rid as id>
						<#if 2==id_index><#break></#if>
						{"id":"${id}","replyer_tinyurl":"${tu[id_index]}","replyContent":${con[id_index]},"ubname":"${un[id_index]}","replyTime":"${rt[id_index]}","type":${ty[id_index]},"reply_layer":${la[id_index]},"ubid":${ui[id_index]}}
						<#if (rid?size > 1) && 0==id_index>,</#if>
					</#list>
					],"ownerid":${fID[0]},"userDigged":"${selfzancount[0]}","digged":"${totalzancount[0]}","type":"photo","guestName":"${userName[0]}"};
				getReplyOfDoingFromJSON( reply${pId[0]} , '${pId[0]}' ,'${userID[0]}' , '${commentcount[0]}','1' ,'','',${isShow})
			})();
			</script>
	</div>
<#include "1,1">
</li>
</#compress>
