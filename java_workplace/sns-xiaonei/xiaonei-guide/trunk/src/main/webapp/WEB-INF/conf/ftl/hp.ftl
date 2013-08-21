<#compress>
<div class="feed img-quote feed-gift expand ${readed[0]}" id="${id[0]}">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" class="iActs" />
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
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a> 在惠普“我的电脑 我的舞台”创作了一首混搭民歌和摇滚的嘻哈作品，<a href="http://mopgg.mop.com/adclick.jsp?castID=14734&url=${url[0]}" target="_blank">快去欣赏一下吧！</a>
					<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
		<div class="feedbody clearfix">
			<div class="photo-box">
				<a href="http://mopgg.mop.com/adclick.jsp?castID=14734&url=${url[0]}" target="_blank"><img src="http://xnimg.cn/imgpro/hp/hp_head2.jpg"/></a>
			</div>
			<p class="quote">
				<q><a href="http://mopgg.mop.com/adclick.jsp?castID=14734&url=${url[0]}" target="_blank">${works[0]}</a></q>
			</p>
		</div>
	</div>
</div>
</#compress>
