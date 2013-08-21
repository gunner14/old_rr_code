<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<script data-src="http://a.xnimg.cn/modules/global-publisher/products/statusdefault/tpl/share-all.js?ver=$revxxx$"
	data-module="
	xn/globalpublisher/products/statusdefault/tpl/share/editor.mustache
	xn/globalpublisher/products/statusdefault/tpl/share/inputer.mustache
	xn/globalpublisher/products/statusdefault/tpl/share/loading.mustache
	xn/globalpublisher/products/statusdefault/tpl/share/success.mustache
	"></script>
<script data-src="http://a.xnimg.cn/modules/global-publisher/products/statusdefault/tpl/music-all.js?ver=$revxxx$"
	data-module="
	xn/globalpublisher/products/statusdefault/tpl/music/editor.mustache
	xn/globalpublisher/products/statusdefault/tpl/music/inputer.mustache
	xn/globalpublisher/products/statusdefault/tpl/music/loadingBox.mustache
	xn/globalpublisher/products/statusdefault/tpl/music/result.mustache
	xn/globalpublisher/products/statusdefault/tpl/music/searching.mustache
	xn/globalpublisher/products/statusdefault/tpl/music/successBox.mustache
	"></script>
<script data-src="http://a.xnimg.cn/modules/global-publisher/products/statusdefault/tpl/video-all.js?ver=$revxxx$"
	data-module="
	xn/globalpublisher/products/statusdefault/tpl/video/editor.mustache
	xn/globalpublisher/products/statusdefault/tpl/video/inputer.mustache
	xn/globalpublisher/products/statusdefault/tpl/video/loading.mustache
	xn/globalpublisher/products/statusdefault/tpl/video/success.mustache
	"></script>
<script data-src="http://a.xnimg.cn/modules/global-publisher/products/statusdefault/tpl/photo-all.js?ver=$revxxx$"
	data-module="
	xn/globalpublisher/products/statusdefault/tpl/photo/editor.mustache
	xn/globalpublisher/products/statusdefault/tpl/photo/success.mustache
	"></script>

<script type="text/javascript">
	var getPublisherPhotoData = function(o){
		window.publisherPhotoData = o;
	};
</script>
<div class="global-publisher-module" data-publishtogroupsapi="http://shell.renren.com/${visiter.id}/channel">
    <c:if test="${olympic_today_no_show==false}">
        <div class="today-guess" style="display:none;">
            [奥运话题]
            <a href="#nogo" onclick="return false;" class="topic" data-topic='${olympic_guide["olympic_guide_sharpdoc"]}'>
                ${olympic_guide["olympic_guide_doc"]}
            </a>
            <span class="close-guess">&times;</span>
        </div>
    </c:if>
	<form class="status-global-publisher" action="http://shell.renren.com/${visiter.id}/status" method="post">
		<div id="global-publisher-status-box" class="global-publisher-status-box">
			<article id="global-publisher-status">
			<section>
			<div class="editor-box">

				<div class="status-editor">
					<div class="status-inputer">
						<div class="status-textarea">
							<textarea name="content" class="status-content" defaultpos="${guideTextBlackPosition}" defaultmodule="${guideTextWithOffset}" placeholder="${onlineText}"></textarea>
						</div>
						<dl class="global-publisher-selector">
							<dd class="global-publisher-selector-photo"><a class="global-publisher-photo-trigger" href="javascript:;" title="照片"></a></dd>
							<dd class="global-publisher-selector-video"><a class="global-publisher-video-trigger" href="javascript:;" title="视频"></a></dd>
							<c:if test="${!host.page}">
								<dd class="global-publisher-selector-music"><a class="global-publisher-music-trigger" href="javascript:;" title="音乐"></a></dd>
							</c:if>
							<dd class="global-publisher-selector-share"><a class="global-publisher-share-trigger" href="javascript:;" title="分享"></a></dd>
							<%--<dd class="global-publisher-selector-blog"><a class="global-publisher-blog-trigger" href="javascript:;"><img src="http://xnimg.cn/modules/global-publisher/res/blog.png" alt="日志" /></a></dd>--%>
						</dl>
						<div class="status-toolbar" style="display:none">
							<a class="at-button" href="#at" title="点名"></a>
							<a class="emotion-button" href="#emotion" title="表情"></a>
							<input class="speech-button" style="display:none;" type="text" x-webkit-speech="speech" lang="zh_CN" title="语音输入" />
							<div class="chars-info" style="display:none"><span class="chars-remain">240</span></div>
						</div>
					</div>
					<div class="global-publisher-modules-box">
						<menu>
							<li><a class="close" style="display:none;" href="#" onclick="return false;">关闭</a></li>
						</menu>

						<%--2011/12/12隐藏发日志功能--%>
						<%--<div id="global-publisher-blog-box" class="global-publisher-module-box" style="display:none">--%>
							<%--<article id="global-publisher-blog" class="global-publisher-blog">--%>

							<%--<section>--%>
							<%--<div class="blog-editor-box"></div>--%>
							<%--</section>--%>
							<%--</article>--%>
						<%--</div>--%>

						<div id="global-publisher-video-box" class="global-publisher-module-box" style="display:none">
							<article id="global-publisher-video" class="global-publisher-share">
							<section>

							<div class="input-link-box" style="display:none">
							</div>
							<div class="input-comment-box" style="display:none">
							</div>
							</section>
							</article>
						</div>

						<div id="global-publisher-music-box" class="global-publisher-module-box" style="display:none">
							<article class="global-publisher-music">
							<section>
							<div class="input-search-box" style="display:none">
							</div>
							<div class="publish-song-box" style="display:none">
							</div>
							</section>
							</article>
						</div>

						<div id="global-publisher-share-box" class="global-publisher-module-box" style="display:none">

							<article id="global-publisher-share" class="global-publisher-share">
							<section>
							<div class="input-link-box" style="display:none">
							</div>
							<div class="input-comment-box" style="display:none">
							</div>
							</section>
							</article>
						</div>

						<div id="global-publisher-photo-box" class="global-publisher-module-box" data-action="http://shell.renren.com/${visiter.id}/photo" style="display:none">
							<article id="global-publisher-photo" class="global-publisher-photo">
							<section>
							<div class="upload-selector" style="display:none">
								<section class="multiple-uploader">
								<a href="javascript:void(0)" onclick="return false;">
									<h4>多张上传</h4>
									<p>上传多张照片</p>
								</a>
								</section>
								<section class="single-uploader">
								<div class="global-publisher-photo-trigger">
									<input type="file" name="file" />
									<c:choose>
										<c:when test="${host.page}">
											<input style="display:none;" type="submit" name="upload" formtarget="global-publisher_upload" formenctype="multipart/form-data" formaction="http://upload.renren.com/upload.fcgi?pagetype=addpublisherpage&hostid=${visiter.id}&callback=window.parent.getPublisherPhotoData" formmethod="post" />
										</c:when>
										<c:otherwise>
											<input style="display:none;" type="submit" name="upload" formtarget="global-publisher_upload" formenctype="multipart/form-data" formaction=	"http://upload.renren.com/upload.fcgi?pagetype=addpublishersingle&hostid=${visiter.id}&callback=window.parent.getPublisherPhotoData" formmethod="post" />
										</c:otherwise>
									</c:choose>
									<input type="hidden" name="hostid" value="${visiter.id}" />
									<iframe style="display:none;" name="global-publisher_upload" src="http://www.renren.com/ajaxproxy.htm" id="global-publisher_upload"></iframe>

								</div>
								<a href="javascript:return false;">
									<h4>快速上传</h4>
									<p>上传一张照片</p>
								</a>
								</section>
								<section class="video-uploader">
								<a href="javascript:void(0)" onclick="return false;">
									<h4>上传视频</h4>
									<p>上传一段视频</p>
								</a>
								</section>
							</div>
							<div class="uploading-module" style="display:none">

								<p><img src="http://xnimg.cn/n/res/icons/indicator.gif" width="16" height="16" />正在上传，请稍候...</p>
							</div>
							<div class="photo-info-box" style="display:none">
							</div>
							</section>
							</article>
						</div>
					</div>
					<div class="global-publisher-footer" style="display:none">
						<%--11/12/7 隐藏发布到功能，联系zheng.lai--%>
						<%--<div class="publish-to-box">--%>
							<%--<h3>发布至：<span class="publish-to-info">全部好友</span> <a class="publish-to-button" href="#publish-to">编辑</a></h3>--%>
							<%--<div class="publish-to-panel-box" style="display:none">--%>
							<%--</div>--%>
						<%--</div>--%>
						<div class="global-publisher-actions">
							<c:choose>
								<c:when test="${host.page}">
									<input class="submit" type="submit" value="发布" title="发布到公共主页" />
								</c:when>
								<c:otherwise>
									<input class="submit" type="submit" value="发布" />
								</c:otherwise>
							</c:choose>
						</div>
						<div class="last-status" style="display:none;">
							<c:choose> 
								<c:when test="${empty requestScope.ugcstatus}">
									<a ui-async="async" title="${requestScope.fullstatus}" href="${applicationScope.urlStatus}/status?id=${requestScope.ubid}"></a>
								</c:when>
								<c:otherwise>
									<a ui-async="async" title="${requestScope.fullstatus}" href="${applicationScope.urlStatus}/status?id=${requestScope.ubid}">
										${updateTitle}:${requestScope.ugcstatus}
									</a>
								</c:otherwise>
							</c:choose>     
						</div>
					</div>

				</div>
			</div>
			</section>
			</article>
		</div>
	</form>
</div>
