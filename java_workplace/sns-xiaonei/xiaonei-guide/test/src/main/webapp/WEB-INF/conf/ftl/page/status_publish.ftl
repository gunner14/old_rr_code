${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
	<#if idef?exists><img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/></#if>
	: ${title[0]}
	<#if wap?exists>
		<span class="from">通过<a target="_blank" href="http://mobile.renren.com/?newsfeed">手机</a>更新 <a target="_blank" href="http://mobile.renren.com/?newsfeed"><img class="iTinyphone" alt="通过手机发布" src="http://s.xnimg.cn/a.gif"/></a></span>
	</#if>
	</h3>
	<div class="details">
		<div class="legend"><img alt="状态" src="http://s.xnimg.cn/a.gif"" class="iStatus"/>
		<span class="duration"><@format>${dtime[0]}</@format></span>
		<a href="#nogo" id="replyKey${dID[0]}" onClick="getReplyOfTheDoing4Page('${dID[0]}','${fID[0]}','${userID[0]}','f',false,'3')">
			<span id="replyCount${dID[0]}"><#if ((count[0])?number > 0)>${count[0]}条</#if></span>回复
		</a>
		<span class="seperator">|</span>
		<a href="#nogo" onclick="forwardDoing('${dID[0]}', '${fID[0]}','page');" >转发</a>
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
				getReplyOfDoingFromJSON4Page( reply${dID[0]} , '${dID[0]}' ,'${userID[0]}' , '${count[0]}','3' )
			})();
		</script>
	</div>
<#include "1,1">
</li>
</#compress>