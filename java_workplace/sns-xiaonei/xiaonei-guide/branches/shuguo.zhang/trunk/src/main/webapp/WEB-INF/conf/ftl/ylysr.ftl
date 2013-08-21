<#compress>
<div class="feed img-quote feed-gift expand" id="${id[0]}">
<#if visitType[0]=="minifeed">
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="伊利优酸乳" class="iActs"/>
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<#if userID[0] == fID[0]>
						<a class="x-to-hide" href="javascript:void(0);" onclick="javascript:readThisFeed('${id[0]}','${userID[0]}','minifeed');"></a>
					</#if>
				</span>
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}&ref=minifeed" target="_blank">${fName[0]}</a>  收到了 <a href="http://renren.com/profile.do?id=${uID[0]}&ref=minifeed" target="_blank">${uName[0]}</a>
				赠送的伊利优酸乳 <a href="http://yiliysr.renren.com/display.jsp?workid=${workid[0]}&ref=minifeed" target="_blank">与周杰伦共舞</a> 电子音乐贺卡！你也去上传歌曲，制作贺卡吧！
				</h4>
			</div>
		</div>
		<div class="feedbody clearfix">
			<div class="photo-box">
				<a href="http://yiliysr.renren.com/display.jsp?workid=${workid[0]}&ref=minifeed" target="_blank"><img src="http://xnimg.cn/imgpro/icons/yiligift.gif" /></a>
			</div>
		</div>
	</div>
<#else>
	<div class="feed-icon">
		<img src="http://xnimg.cn/a.gif" alt="伊利优酸乳" class="iActs" />
	</div>
	<div class="feed-content">
		<div class="headline">
			<div class="title">
				<span class="share-n-hide">
					<a class="x-to-hide" href="javascript:void(0);" onclick="readHomeFeed('${id[0]}')"></a>
				</span>
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}&ref=newsfeed" target="_blank">${fName[0]}</a> 收到了 <a href="http://renren.com/profile.do?id=${uID[0]}&ref=newsfeed" target="_blank">${uName[0]}</a>
				赠送的伊利优酸乳 <a href="http://yiliysr.renren.com/display.jsp?workid=${workid[0]}&ref=newsfeed" target="_blank">与周杰伦共舞</a> 电子音乐贺卡！你也去上传歌曲，制作贺卡吧！
				</h4>
			</div>
		</div>
		<div class="feedbody clearfix">
			<div class="photo-box">
				<a href="http://yiliysr.renren.com/display.jsp?workid=${workid[0]}&ref=newsfeed" target="_blank"><img src="http://xnimg.cn/imgpro/icons/yiligift.gif" /></a>
			</div>
		</div>
	</div>
</#if>
</div>
</#compress>