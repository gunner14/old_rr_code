<#compress>
<div class="feed ${readed[0]}" id="${id[0]}">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="校内三国" class="iSanguo" />
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<#if visitType[0]=="minifeed">
						<#if userID[0] == fID[0]>
							<a class="x-to-hide" href="javascript:void(0);" onclick="javascript:readThisFeed('${id[0]}','${userID[0]}','minifeed');"></a>
						</#if>
						<#else>
							<a class="x-to-hide" href="javascript:void(0);" onclick="readHomeFeed('${id[0]}')"></a>
					</#if>
				</span>
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a> ${prefix[0]} <a target="_blank" href="http://sanguo.renren.com">校内三国风云</a> 
				<#if serv?exists>
					${serv[0]}
				</#if>
				<#if suffix?exists>
					${suffix[0]}
				</#if>
				<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
			<#if cont?exists>
				<div class="feedbody">
					<p>${cont[0]}<a href="http://sanguo.renren.com/" target="_blank" onclick="readHomeFeed('${id[0]}')">» 继续阅读</a></p>
				</div>
			</#if>
		</div>
	</div>
</div>
</#compress>