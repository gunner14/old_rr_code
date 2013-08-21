<#compress>
<div class="feed feed-vip ${readed[0]}" id="${id[0]}">
<#if visitType[0]=="minifeed">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="紫豆" class="iVip" />
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<#if userID[0] == fID[0]>
						<a class="x-to-hide" href="javascript:void(0);" onclick="javascript:readThisFeed('${id[0]}','${userID[0]}','minifeed');"></a>
					</#if>
				</span>
				<h4>
					<a href="http://renren.com/profile.do?id=${fID[0]}&ref=minifeed" target="_blank">${fName[0]}</a> 成功邀请
					<#list uName as n>
						<a target="_blank" href="http://renren.com/profile.do?id=${uID[n_index]}">${n}</a>
					</#list>
						来校内玩投票获赠一个月<a href="http://i.renren.com/index.action?ref=minifeed">尊贵紫豆用户</a>身份！你也想成为尊贵紫豆用户么？快来邀请好友玩<a href="http://abc.renren.com/knowabc/investigation/KnowVotingList.do?showType=all&ref=minifeed">投票</a>吧
					<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
	</div>
<#else>
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="紫豆" class="iVip" />
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<a class="x-to-hide" href="javascript:void(0);" onclick="readHomeFeed('${id[0]}')"></a>
				</span>
				<h4>
					<a href="http://renren.com/profile.do?id=${fID[0]}&ref=newsfeed" target="_blank">${fName[0]}</a> 成功邀请
					<#list uName as n>
						<a target="_blank" href="http://renren.com/profile.do?id=${uID[n_index]}&ref=newsfeed">${n}</a>
					</#list>
					来校内玩投票获赠一个月<a href="http://i.renren.com/index.action?ref=newsfeed">尊贵紫豆用户</a>身份！你也想成为尊贵紫豆用户么？快来邀请好友玩<a href="http://abc.renren.com/knowabc/investigation/KnowVotingList.do?showType=all&ref=newsfeed">投票</a>吧
					<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
	</div>
</#if>
</div>
</#compress>