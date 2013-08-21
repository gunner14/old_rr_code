<div class="side-item-header clearfix">
<h4>推荐应用</h4>
</div>
<div class="side-item-body clearfix">
	<div class="figure">
		<a href="${applicationScope.urlApp}/apps/enterApp.do?id=${recommendApp.appId}&origin=162"><img width="64" src="${recommendApp.logoSrc}"/></a>
	</div>
	<div class="info">
		<a href="${applicationScope.urlApp}/apps/enterApp.do?id=${recommendApp.appId}&origin=164">${recommendApp.appName}</a>
		<img width="59" height="10" src="${applicationScope.urlSmallStatic}/imgpro/info/5stars.png" alt="支持度" title="支持度"/>
		<p>${recommendApp.description}</p>	
		<a href="${applicationScope.urlApp}/apps/enterApp.do?id=${recommendApp.appId}&origin=163" target="_black">
			<img src="${applicationScope.urlStatic}/imgpro/button/enter.png" width="67" height="24" />
		</a>
	</div>
</div>