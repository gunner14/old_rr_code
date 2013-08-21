${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
		<span class="user-group">(${network[0]})</span>注册了人人
	</h3>
	<div class="content">
		<#if comm?exists && comm[0] != "">
			<p class="comment"><q>${comm[0]}</q></p>
		</#if>
	</div>
	<div class="details">
		<div class="legend"><img class="iShare" src="http://s.xnimg.cn/a.gif" alt="分享"/> 
		<span class="duration"><@format>${time[0]}</@format></span>
		<a class="addfriend_action" onclick="showRequestFriendDialog('${fID[0]}','${fName[0]}','${h[0]}',false,'addFriendFromFeed')" href="javascript:void(0);"><img alt="我认识TA" lala="http://s.xnimg.cn/imgpro/bg/knowit.png" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a> </div>
	</div>
	<#include "1,1">
</li>
</#compress>