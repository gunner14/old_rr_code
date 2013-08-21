${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
		<#list fName as n>
			<a target="_blank"  href="http://renren.com/profile.do?id=${fID[n_index]}">${n}</a>
		</#list>&nbsp;已通过绑定手机升级为安全账户，<a href="http://mobile.renren.com/?${vType}" target="_blank">马上升级&raquo;</a>
	</h3>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="手机绑定" class="iBindphone" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li> 
</#compress>
