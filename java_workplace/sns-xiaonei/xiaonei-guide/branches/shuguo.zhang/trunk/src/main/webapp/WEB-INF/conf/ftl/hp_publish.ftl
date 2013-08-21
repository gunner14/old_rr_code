<#compress>
<script>
XN.DOM.readyDo(function(){
var el = new XN.UI.fixPositionElement({id:'feedHP'});
el.hide();
window.swfPlayer = {};
swfPlayer.show = function(id){
el.setContent('<h2 class="clearfix"><a class="float-right large dark" style="background:#fff;" onclick="swfPlayer.hide();return false;">关闭</a></h2>' +
'<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,0,0" width="400" height="350" id="menu2" align="middle"><param name="allowScriptAccess" value="sameDomain" /><param name="allowFullScreen" value="false" /><param name="FlashVars" value="musicid='+id+'"/><param name="movie" value="http://www.hpmystage.com.cn/BlogAvata400350.swf" /><param name="quality" value="high" /><param name="bgcolor" value="#ffffff" />	<embed src="http://www.hpmystage.com.cn/BlogAvata400350.swf" quality="high" bgcolor="#ffffff" width="400" height="350" name="menu2" align="middle" allowScriptAccess="sameDomain"  FlashVars="musicid='+id+'"  allowFullScreen="false" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" /></object>');
el.moveTo(parseInt(((el.alignParent.offsetWidth - el.frame.offsetWidth) / 2 ),10),XN.EVENT.scrollTop() + 200);
};
swfPlayer.hide = function(){
	el.hide();
	el.setContent('');
};
});
</script>
<div class="feed img-quote feed-gift expand ${readed[0]}" id="${id[0]}">
	<div class="feed-icon">
		<img src="http://xnimg.cn/imgpro/icons/acts.gif" alt="我的电脑 我的舞台"/>
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
				<h4><a href="http://renren.com/profile.do?id=${fID[0]}" target="_blank">${fName[0]}</a>在惠普“我的电脑 我的舞台”创作完成了一条嘻哈作品 <a href="http://www.hpmystage.com.cn/#musicid=${link[0]}" target="_blank">${feedTitle[0]}</a> 快去欣赏一下
					<span class="date"><@format>${time[0]}</@format></span>
				</h4>
			</div>
		</div>
		<div class="feedbody clearfix">
			<div class="photo-box">
				<a href="http://www.hpmystage.com.cn/#musicid=${link[0]}" target="_blank">
				<img src="${imageUrl[0]}"
				<#if visitType[0]=="newsfeed">
					cursor: default;
					<#else>
					onclick="swfPlayer.show('$!{link}');"
				</#if>
				/></a>
			</div>
			<#if visitType[0]=="minifeed">
				<div id="feedHP" style="display:none;width:400px;"></div>
			</#if>
			<p class="quote">
				<q>${feedTitle[0]}</q>
			</p>
		</div>
	</div>
</div>
</#compress>
