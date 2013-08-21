			<div id='publisher_albumContent' style="display:none;">
			<form id="publisher_albumPostForm" method="post" action="${applicationScope.urlPhoto}/createalbum.do">
			<div  class="album-composer">
				<div class="album-action">
					<a id="publisher_createAlbumTab" href="javascript:void(0);" onfocus="this.blur();"><span>创建相册</span></a>
					<a id="publisher_selectAlbumTab" href="javascript:void(0);" onfocus="this.blur();"><span>选择相册</span></a>
				</div>
				
				<div style="display:none;" id="publisher_createAlbumContent" class="create-album">
					<dl>
							<dt><label for="a-name">名字:</label></dt>
							<dd><input  id="publisher_albumTitle" rule="blank" error="请填写相册的名称" name="title" class="input-text" /></dd>
							
							<dt><label for="a-description">相册描述:</label></dt>
							<dd><textarea id="publisher_albumDescription" rows="4" name="caption"></textarea></dd>
							
							<dt><label for="select-privacy">隐私:</label></dt>
							<dd>
								<div class="clearfix">
								<div class="privacy-widget clearfix">
									<div class="lock">
										<div class="lockbg"></div>
										<div class="lockbottomborder"></div>
									</div>
									<div class="who-canctab">哪些人可以看到？</div>
									<div class="setprivacy">
										<select id="control" name="control">
											<option value="-1">只有我自己</option>
											<option value="1">我的好友</option>
											<option value="3">我的好友及所有同网络的人</option>
											<option value="99" selected="selected">所有人</option>
										</select>
									</div>
								</div>
								</div>
							</dd>
						</dl>
				</div>
				
				<div id="publisher_selectAlbumContent" class="selectalbum">
						<dl>							
							<dt><label for="selectphoto" class="allalbums">选择相册:</label></dt>
							<dd>
								<select id="publisher_albums" name="allalbums" style="width:320px;" >
									<option>加载中...</option>
								</select>
							</dd>
						</dl>
				</div>
				
			</div>
			<input type="hidden" id="publisher_albumLocation" name="location" />
			<input type="hidden" id="publisher_albumFlag" name="flag" value="0" />
			<div class="postbtn">
				<input id="publisher_ablumSubmit" type="submit" class="input-button" value="确定" />
				<input class="input-button gray" onclick="$('publisherModule').hide();$('publisher_albumTab').className='';" type="button" value="取消" />
			</div>
			
			</form>
			</div>
