<div id="publisher_blogContent" style="display:none;">
	<form id="publisher_blogPostForm" name="shit" method="post" action="${applicationScope.urlBlog}/NewEntry.do">
			<div class="blog-composer clearfix">
					<dl>
						<dt><label for="title" class="displayblock">标题:</label></dt>
						<dd><input id="blog_title" name="title" rule="blank" error="请填写日志内容" type="text" class="input-text" value="" /></dd>
						
						<dt><label for="publisher_blogBody" class="displayblock">内容:</label></dt>
						<dd><textarea id="publisher_blogBody" class="publisher-blogBody" name="body" rows="12" rule="blank" error="请填写日志内容"></textarea></dd>
						
						<dt><label class="displayblock">点名:</label></dt>
						<dd><div id="publisher_blogFriendSelector"></div></dd>
						
						<dt><label class="displayblock">隐私:</label></dt>
						<dd><div class="clearfix">
							<div class="privacy-widget clearfix">
								<div class="lock">
									<div class="lockbg"></div>
									<div class="lockbottomborder"></div>
								</div>
								<div class="who-canctab">哪些人可以看到？</div>
								<div class="setprivacy">
									<select name="blogControl">
									 <option value="-1">只有我自己</option>
									 <option value="1">我的好友</option>
									 <option value="3">我的好友及所有同网络的人</option>
									 <option value="99" selected>所有人</option>
									</select>
								</div>
							</div>
							</div></dd>
					</dl>
				
			</div>
			<input type="hidden" name="tsc" id="publisher_blog_tsc" />
			<div class="postbtn"><p><a href="${applicationScope.urlBlog}/NewEntry.do" style="float:left">切换到编辑器模式 »</a></p><input id="publisher_blogSubmit" type="submit" class="input-button" value="发布" /> <input class="input-button gray" onclick="$('publisherModule').hide();$('publisher_blogTab').className='';" type="button" value="取消" /></div>
		</form>
</div>

