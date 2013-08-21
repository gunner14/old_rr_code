<div id="publisher_shareContent" style="display:none;">
<form id="publisher_sharelinkForm" name="sharelink"  action="${applicationScope.urlShare}/share/ShareOperate.do"  method="post" onsubmit="return false;">
	<div class="share-composer">
		<dl>
			<dt><label for="publisher_shareWeblink">分享:</label></dt>
			<dd><input name="weblink" id="publisher_shareWeblink" class="input-text" rule="blank#sharelink" error="请填写合适的分享链接" value="http://" /></dd>
			<dt><label for="publisher_shareComment">添加评论:</label></dt>
			<dd><textarea id="publisher_shareComment" name="sharecomment" rows="4"></textarea></dd>
			<input type="hidden" name="action" value="sharelink"/>
			<dt><label for="select-privacy">隐私:</label></dt>
			<dd>
				<div class="clearfix">
				<div class="privacy-widget clearfix">
					<div class="lock"><div class="lockbg"></div></div>
					<div class="who-canctab">哪些人可以看到？</div>
					<div class="setprivacy">
						<select name="share-auth">
							<option value="99" selected="selected">所有人</option>
							<option value="0">我的好友</option>
							<option value="-1">自己收藏</option>
						</select>
					</div>
				</div>
				</div>
			</dd>
		</dl>
	</div>
	<div class="postbtn" >
	<input type="hidden" name="tsc" id="publisher_share_tsc" />
	<input id="publisher_shareSubmit" type="submit" class="input-button" value="确定"/>
	<input class="input-button gray" onclick="$('publisherModule').hide();$('publisher_shareTab').className=''" type="button" value="取消" /></div>
</form>
</div>