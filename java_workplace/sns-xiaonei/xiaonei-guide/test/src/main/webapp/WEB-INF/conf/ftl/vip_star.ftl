<#compress>
<div class="feed feed-vip ${readed[0]}" id="${id[0]}">
<#if visitType[0]=="minifeed">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="${type[0]}" class="iVip" />
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<#if userID[0] == fID[0]>
						<a class="x-to-hide" href="javascript:void(0);" onclick="javascript:readThisFeed('${id[0]}','${userID[0]}','minifeed');"></a>
					</#if>
				</span>
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}&ref=minifeed" target="_blank">${fName[0]}</a> 经过不懈的努力，终于走上了星光大道，成为万众瞩目的<a href="http://i.renren.com/star-init.action" target="_blank">紫豆之星</a>啦！<a href="http://i.renren.com/star-init.action?ref=minifeed" target="_blank">一起支持Ta</a> 吧！
				<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
	</div>
<#else>
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="${type[0]}" class="iVip" />
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<a class="x-to-hide" href="javascript:void(0);" onclick="readHomeFeed('${id[0]}')"></a>
				</span>
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}&ref=newsfeed" target="_blank">${fName[0]}</a> 经过不懈的努力，终于走上了星光大道，成为万众瞩目的<a href="http://i.renren.com/star-init.action" target="_blank">紫豆之星</a>啦！<a href="http://i.renren.com/star-init.action?ref=newsfeed" target="_blank">一起支持Ta</a> 吧！
				<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
	</div>
</#if>
</div>
</#compress>