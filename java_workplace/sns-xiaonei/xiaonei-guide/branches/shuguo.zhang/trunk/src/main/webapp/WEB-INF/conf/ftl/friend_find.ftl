${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		通过 <a href="http://renren.com/findfriends.do?action=find&ref=${vType}" target="_blank">寻找朋友</a>，
		<a target="_blank" href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}">${fName[0]}</a>
		和
		<#list uName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${uID[n_index]}&ref=${vType}">${n}</a>
		</#list> 成为好友
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
