<#compress>
<div class="feed feed-act ${readed[0]}" id="${id[0]}">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="${type[0]}" class="iAiting" />
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
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a> 将 <a href="${url[0]}" target="_blank">${name[0]}</a> 添加为喜欢的歌手.
				<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
	</div>
</div>
</#compress>