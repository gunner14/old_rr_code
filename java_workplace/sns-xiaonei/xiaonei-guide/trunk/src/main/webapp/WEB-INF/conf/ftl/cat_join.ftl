<#compress>
<div class="feed feed-myj ${readed[0]}" id="${id[0]}">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="猫游记" class="iMyj" />
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
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a> 加入校内猫游记 <a href="${url[0]}" target="_blank">${name[0]}</a>
				<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
	</div>
</div>
</#compress>