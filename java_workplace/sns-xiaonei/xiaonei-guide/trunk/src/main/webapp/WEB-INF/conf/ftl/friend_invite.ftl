${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list uName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${uID[n_index]}&ref=${vType}">${n}</a>
		</#list> 接受了 <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank" class="dark">${fName[0]}</a> 的 
		<#if wap?exists>
			<a href="http://invite.renren.com/mInvite.do?ref=${vType}" target="_blank">
		<#else>
			<a href="http://renren.com/Invite.do?ref=${vType}" target="_blank">
		</#if>
		邀请</a> 注册人人网了
		<#if wap?exists><span class="from"><a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://xnimg.cn/a.gif" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
	</h3>
	<div class="details">
		<div class="legend">
			<img src="http://xnimg.cn/a.gif" alt="好友" class="iFriend" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>
