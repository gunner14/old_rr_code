<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}">${fName[0]}</a>${title[0]}</h3>
	<div class="details">
		<div class="legend">
			<img src="${(icon[0])!"about:blank"}"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>