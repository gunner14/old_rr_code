<#if wap?exists>
	<span class="from">
		<a href="http://mobile.renren.com/simulator.do?ref=${vType}" target="_blank">通过手机人人网</a> 
		<a href="http://mobile.renren.com/simulator.do?ref=${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" class="icon iTinyphone" title="通过手机人人网"/></a>
	</span>
	<span class="seperator">|</span>
</#if>
<#if hLink?exists>
	<span class="from"><a href="${hLink[0]}" target="_blank">通过${hText[0]}发布</a></span>
	<span class="seperator">|</span>
</#if>
<#if appIconSrc?exists>
	<span class="from">
		<a href="http://mobile.renren.com/client.do" target="_blank">通过手机人人客户端</a>
		<a href="http://mobile.renren.com/client.do" target="_blank"><img class="iPhone" title="通过手机人人客户端" src="http://s.xnimg.cn/a.gif"/></a>
	</span>
	<span class="seperator">|</span>
</#if>
<#if source[0] == 10>
	<span class="from">
		<a href="http://mobile.renren.com/simulator.do?ref=${vType}" target="_blank">通过手机人人网</a> 
		<a href="http://mobile.renren.com/simulator.do?ref=${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif" class="icon iTinyphone" title="通过手机人人网"/></a>
	</span>
	<span class="seperator">|</span>
</#if>