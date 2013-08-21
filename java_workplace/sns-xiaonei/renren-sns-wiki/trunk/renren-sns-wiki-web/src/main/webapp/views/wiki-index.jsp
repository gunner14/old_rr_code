<%@ page contentType="text/html;charset=UTF-8" language="java" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
	<script type="text/javascript" src="http://s.xnimg.cn/apps/wiki/js/lib-all.js?ver=$revxxx$"></script>
	<title>${wiki.name}的主页</title>
	<%@ include file="/views/inc/wiki-header.inc" %>
</head>
<body class="wiki-body">
	<%@ include file="/views/inc/wiki-body-header.inc" %>
	<div id="index_container" style="display:none">
	</div>
	<div class="wiki-container">
		<div class="wiki-header">
			<div class="wiki-header-banner <c:if test="${empty wikiCover.extraOption }"> banner-customize</c:if>" style="background:url('${wikiCover.largeUrl}')">
				<!-- Basic Info -->
				<%@ include file="/views/widgets/basic-info.jsp" %>
			</div>
			
			<div class="wiki-intro-box clearfix">
				<!-- Detail Info -->
				<%@ include file="/views/widgets/detail-info.jsp" %>
				<!-- High Lights Zone -->
				<%@ include file="/views/widgets/high-lights.jsp" %>
			</div>
		</div>
		<div class="wiki_feed_back"><a href="http://support.renren.com/support/message?type=1000394" target="_blank"><img src="http://a.xnimg.cn/apps/wiki/cssimg/feedback.png"></a></div>
		<!-- Floating publisher -->
		<div id="wiki-content">
			<div class="wiki-tool clearfix">
				<div class="w-sort-box">
					<a href="http://wiki.renren.com/${wiki.id}" class="sort-return">返回</a>
					<a href="javascript:void(0)" class="w-sort-btn" id="sort_button"><span class="w-sort-all" id="sort_button_text">全部内容</span><span class="sort-arrow"></span>
					</a>
					<div class="w-btnbox">
						<div id="tag_newhot">
						<a href="http://wiki.renren.com/${wiki.id}/feed/hot/?type=${itemType}" class="w-gray-btn left-btn w-done" id="hot_tag">最热</a>
						<a href="http://wiki.renren.com/${wiki.id}/feed/latest/?type=${itemType}" class="w-gray-btn right-btn" id="lasted_tag">最新</a>
						</div>
						<div id="tag_user" style="display:none">
						<a href="http://wiki.renren.com/${wiki.id}/user/follow" class="w-gray-btn left-btn w-done" data-act="follow" id="follow_tag">喜欢</a>
						<a href="http://wiki.renren.com/${wiki.id}/user/wish" class="w-gray-btn right-btn" data-act="wish" id="wish_tag">想做</a>
						<a href="http://wiki.renren.com/${wiki.id}/user/do" class="w-gray-btn right-btn" data-act="do" id="do_tag">正做</a>
						<a href="http://wiki.renren.com/${wiki.id}/user/collect" class="w-gray-btn right-btn" data-act="collect" id="collect_tag">做过</a>
						</div>
						<ul class="w-sort-list" style="display:none" id="menu_list">
							<li type="video"><a href="http://wiki.renren.com/${wiki.id}/feed/${wikiFeedType}/?type=video" >视频</a></li>
							<li type="photo"><a href="http://wiki.renren.com/${wiki.id}/feed/${wikiFeedType}/?type=photo" >照片</a></li>
							<li type="blog"><a href="http://wiki.renren.com/${wiki.id}/feed/${wikiFeedType}/?type=blog" >日志</a></li>
							<li type="status"><a href="http://wiki.renren.com/${wiki.id}/feed/${wikiFeedType}/?type=status" >状态</a></li>
							<li type="all"><a class="sort-selected" href="http://wiki.renren.com/${wiki.id}/feed/${wikiFeedType}/?type=all" >全部内容</a></li>
							<li class="justfriend"><input type="checkbox" id="justFriend"><label for="justFriend">只看好友</label></li>
						</ul>
					</div>
				</div>
				<div class="wiki-publisher">
					<div id="global-publisher" class="global-publisher-box">
						<div class="global-publisher-module">
							<dl class="global-publisher-selector">
								<dd class="global-publisher-selector-status"><a class="global-publisher-status-trigger trigger" href="javascript:;" title="状态"><img src="http://xnimg.cn/modules/global-publisher/res/photo2.png" stats="V6Pub_photo" data-openedsrc="http://xnimg.cn/modules/global-publisher/res/photo.png" alt="状态">发状态</a><span class="arrow"></span></dd>
								<dd class="pipeline"></dd>
								<dd class="global-publisher-selector-photo"><a class="global-publisher-photo-trigger trigger" href="javascript:;" title="照片"><img src="http://xnimg.cn/modules/global-publisher/res/photo2.png" stats="V6Pub_photo" data-openedsrc="http://xnimg.cn/modules/global-publisher/res/photo.png" alt="照片">传照片</a><span class="arrow"></span></dd>
								<dd class="pipeline"></dd>
								<dd class="global-publisher-selector-video"><a class="global-publisher-video-trigger trigger" href="javascript:;" title="视频"><img src="http://xnimg.cn/modules/global-publisher/res/video2.png" stats="V6Pub_video" data-openedsrc="http://xnimg.cn/modules/global-publisher/res/video.png" alt="视频">转视频</a><span class="arrow"></span></dd>
								<dd class="pipeline"></dd>
								<dd class="global-publisher-selector-blog"><a class="global-publisher-blog-trigger trigger" href="javascript:;" title="日志"><img src="http://xnimg.cn/modules/global-publisher/res/link2.png" stats="V6Pub_shareLink" data-openedsrc="http://xnimg.cn/modules/global-publisher/res/link.png" alt="日志">发日志</a><span class="arrow"></span></dd>
							</dl>
							<div id="global-publisher-status-box" class="global-publisher-status-box">
								<article id="global-publisher-status" class="global-publisher-status">
									<section>
										<div class="editor-box" style="display:none">
											<form class="status-global-publisher" action="#" method="post">
												<div class="status-inputer">
													<div class="status-textarea">
														<textarea name="content" class="status-content" defaultpos="" defaultmodule="" placeholder="你正在干嘛？"></textarea>
													</div>
													<div class="publisher-toolbar" style="display:none">
														<a class="at-button" href="#at" title="点名"><img src="http://xnimg.cn/modules/global-publisher/res/at-button.png"></a>
														<a class="emotion-button" href="#emotion" title="表情"><img src="http://xnimg.cn/modules/global-publisher/res/emotion-button.png"></a>
														<input class="speech-button" style="display:none;" type="text" x-webkit-speech="speech" lang="zh_CN" title="语音输入" />
														<div class="chars-info" style="display:none"><span class="chars-remain">240</span></div>
													</div>
												</div>
												<div class="global-publisher-modules-box">
												</div>
												<div class="global-publisher-footer" style="display:none">
													<div class="global-publisher-actions">
														<input type="hidden" name="channel" value="renren" />
														<input class="submit" type="submit" value="发布" />
													</div>
													<div class="last-status" style="display:none;">
													</div>
												</div>
												
											</form>
										</div>
										<div id="iframe-box"  style="display:none"><iframe id="publisher-iframe" frameborder="0" allowtransparency='true' scrolling="no" src="about:blank"></iframe></div>
										<a onclick="return false;" href="#" id="w-editor-close" class="w-editor-close" style="display:none">关闭</a>
									</section>
								</article>
								
							</div>
							
						</div>
					</div>
				</div>
				
			</div>
			<div class="wiki-tool-bgimg"></div>
		</div>

		<div id="wiki-placeholder"></div>
		<div class="wiki-main-content" >
			<!-- Feed Container -->
			<div class="wiki-feed-box" id="wiki_container"></div>
			<div class="wiki-loading"><img src="http://s.xnimg.cn/apps/wiki/cssimg/wiki-loading.gif">正在加载，请等候...</div>
			<div class="wiki-nothing">
				<p class="wiki-none"><b></b>这里暂时没有内容哦：）</p>
				<p class="wiki-readout"><b></b>阅读完所有内容啦！</p>
			</div>
			<div class="wiki-guide wiki-guide-hot"><!--最热提示信息（阅读完/无内容）-->
				<div></div>
				<h3>你已经阅读完所有“最热”内容啦！</h3>
				<p>在“最新”分类中挑选喜欢的内容并标注<i></i>，<br/>此内容便会在“最热”出现。</p>
			</div>
			<div class="wiki-guide wiki-guide-new"><!--最新提示信息（阅读完/无内容）-->
				<div></div>
				<h3>你已经阅读完所有“最新”内容啦！</h3>
				<p>可以通过上方<i></i>发布器分享更多的信息。<br/>一起将<b></b>内容丰富起来吧！</p>
			</div>
			<div class="wiki-guide wiki-guide-user"><!--用户模块提示信息（阅读完/无内容）-->
				<div></div>
				<h3>物以类聚，人以群分</h3>
				<p>你可以通过<i></i>，标记你经历的事物。<br/>找到和你有相同态度的人。</p>
			</div>
			<div class="wiki-error"><!--加载错误提示信息-->
				<p>呃... 我可能没反应过来，点击刷新再次询问</p>
				<p style="width:70px;margin:0 auto;"><b></b><a href="#" id="reget">刷新</a></p>
			</div>
			<div class="wiki-page"><!--分页按钮-->
				<div class="page-container">
					<a href="javascript:void(0)" class="w-gray-btn left-btn">上一页</a>
					<a href="javascript:void(0)" class="w-gray-btn right-btn">下一页</a>
				</div>
			</div>
		</div>

	</div>
	
	<script>
		<c:if test="${not empty isIndex}">
		var isIndex = ${isIndex};
		</c:if>
		<c:if test="${isSuperAdmin}">var isSuperAdmin=${isSuperAdmin};</c:if>
		var curPage = "${curPage}";
		var pageType = "${pageType}";
		<c:choose>
		<c:when test="${pageType eq 'feed'}">
		var wikiFeedType = "${wikiFeedType}";//hot,latest
		var itemType = "${itemType}";//feed类型
		var wikiFriendFromServ = ${friendsInfo};
		var wikiFriendContentFromServ = ${friendsContent};
		</c:when>
		<c:when test="${pageType eq 'user'}">
		var wikiFeedType = "hot";
		var action = "${action}";
		var countInfo = ${countInfo};
		</c:when>
		</c:choose>

		var wikiIdFromServ = "${wiki.id}";
		var wikiTypeFromServ = "${wiki.type}";// 电影1/书籍2/游戏3/景点4
		var wikiInfoFromServ = ${wikiInfoFromServ};
		var USER={
			id:"${user.id}",
			name:"${user.name}",
			pic:"${user.tinyFullUrl}"
		};
	
	</script>
	
	<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.app.status.js?ver=$revxxx$"></script>
	<script src="http://s.xnimg.cn/modules/global-publisher/wiki-publisher.js?ver=$revxxx$"></script>
	<script data-src="http://a.xnimg.cn/modules/global-publisher/products/wiki/video-all.js?ver=$revxxx$" data-module="
	xn/globalpublisher/addons2/share
	xn/globalpublisher/products2/wiki/tpl/video/box.mustache
	xn/globalpublisher/products2/wiki/tpl/video/editor.mustache
	xn/globalpublisher/products2/wiki/tpl/video/inputer.mustache
	xn/globalpublisher/products2/wiki/tpl/video/loading.mustache
	xn/globalpublisher/products2/wiki/tpl/video/success.mustache
	"></script>
	<script data-src="http://a.xnimg.cn/modules/global-publisher/products/wiki/photo-all.js?ver=$revxxx$" data-module="
	xn/globalpublisher/addons2/photo
	xn/globalpublisher/products2/wiki/tpl/photo/box.mustache
	xn/globalpublisher/products2/wiki/tpl/photo/editor.mustache
	xn/globalpublisher/products2/wiki/tpl/photo/success.mustache
	"></script>
	<script src="http://s.xnimg.cn/apps/wiki/js/wiki-all-min.js?ver=$revxxx$"></script>
	<%@ include file="/views/inc/wiki-footer.inc" %>
</body>
</html>
