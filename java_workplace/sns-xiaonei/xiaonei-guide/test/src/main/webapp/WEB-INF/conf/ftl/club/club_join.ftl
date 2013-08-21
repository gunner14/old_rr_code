${"\r"}
<#compress>
<#assign page = "1"/>
<#include "1,0">
<li id="feed${id[0]}">
	<div class="figure">
		<a class="avatar" href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank"><img needclip="1" width="50" height="50" lala="${(h[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" /></a>
	</div>
	<h3>
		<#list fName as n>
			<a target="_blank" href="http://renren.com/profile.do?id=${fID[n_index]}&ref=${vType}">${n}</a>
		</#list>
		加入了俱乐部 <a href="http://org.renren.com/${pageId[0]}?ref=${vType}" target="_blank">${title[0]}</a> 
	</h3>
	<div class="content">
		<div class="figure"><a href="http://org.renren.com/${pageId[0]}?ref=${vType}" target="_blank"><img class="photo" lala="${ (src[0])!"about:blank"}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/></a></div>
		<#if brief?exists && brief[0] != ""><p class="comment"><q>${brief[0]}</q></p></#if>
	</div>
	<div class="details">
		<div class="legend">
			<img lala="http://xnimg.cn/imgpro/icons/pageclub-join.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);"/>
			<span class="duration"><@format>${time[0]}</@format></span>
			<a href="http://org.renren.com/pa/bf?pid=${pageId[0]}&ref=${vType}" target="_blank">${bfName[0]}</a>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>