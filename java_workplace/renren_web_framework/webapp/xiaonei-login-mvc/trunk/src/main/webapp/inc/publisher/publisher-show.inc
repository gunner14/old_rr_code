<%@page pageEncoding="UTF-8" %>
<%--
    String value = com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request, "sd");
	request.setAttribute("sd",value);
--%>

<%-- publisher 动态默认文本 --%>
<script>
    XN.widgets.publisher.dDefaultValue = '${onlineText}';
</script>

<c:set var="publisherHighVersion" value="${visiter.userStateInfo.guideDone}" />

<div class="publisher-highlight">
    <%--<a href="http://music.renren.com/star/gala?frhome" target="_blank" id="dressPublisherSkin" style="background:url(http://xnimg.cn/imgpro/home/gaga-band.png) 0 0 no-repeat;width:114px;height:20px;top:-10px;<c:if test="${!visiter.userStateInfo.guideDone}">display:none</c:if>">GAGA乐队专访</a>--%>
    
	<div class="status-publisher">
		<form method="post" autocomplete="off" action="${applicationScope.urlUpload}/addphotosingle.do" id="publisher_upload_form" enctype="multipart/form-data" target="publisher_upload">
			<textarea accesskey="s" style=""  name="status" id="publisher_statusInput" stats="Pub_Status"></textarea>
			<div class="emotions" id="publisher_emotion" style="display:none">
				<ul class="emotion">
				   <c:forEach var="item" items="${ubbList}">
				   <li ${fn:contains(item.src, '-wide') ? 'class="wider"' : ''}>
				   <a href="javascript:;"><img emotion="${item.ubb}" alt="${item.alt}" title="${item.alt}" src="http://s.xnimg.cn/a.gif" rsrc="${staticUrl}${item.src}"/></a>
				   </li>
				   </c:forEach>
				</ul>
			</div>
			<div style="overflow: hidden;display: block;" id="publisher_tools" class="publisher-footer">	<span class="current-status" id="currentStatus">
					<c:choose> <c:when test="${empty requestScope.ugcstatus && !visiter.userStateInfo.guideDone}">                	
						</c:when>
						<c:otherwise>
							<a ui-async="async" title="${requestScope.fullstatus}" href="${applicationScope.urlStatus}/getdoing.do?id=${requestScope.ubid}">
								${updateTitle}:${requestScope.ugcstatus}
							</a>
						</c:otherwise>
					</c:choose>   
				</span>
				<div id="publisher_attach" style="display:none">
					<span class="status-attachments">
						<a href="javascript:;" class="pub-emtion" id="publisher_emobtn" onfocus="this.blur();" style="display:none;">表情</a>
						<a class="share-video" stats="Pub_Share" href="javascript:;" id="publisher_share_video">视频</a>
						<a class="share-link" stats="Pub_Share" href="javascript:;" id="publisher_share_link">链接</a>
						<a class="share" stats="Pub_Share" href="javascript:;" id="publisher_share" style="display:none;">分享</a>
						<a href="javascript:;" class="photo">
							照片<input type="file" name="file" size="1" class="addfile" style="*margin-left:-20px;*margin-top:-22px;" id="publisher_file" stats="Pub_Photo" />
						</a>
						<iframe id="publisher_upload" style="display: none;" src="${applicationScope.urlProfile}/ajaxProxy.html" name="publisher_upload"></iframe>
					</span>
				</div>				
			</div>
			<div style="position:relative;overflow: hidden; height: 0px;" id="publisher_action_frame">
				<div class="status-attachment" id="publisher_action_rframe">
					<h4><img src="${applicationScope.urlSmallStatic}/a.gif" id="publisher_action_title_img" class="iPhoto icon" /> <span id="publisher_action_title" href="#">照片</span></h4>
						<a id="publisher_action_close" class="close" href="#">关闭</a>
						<div class="attachment" id="publisher_action_content"></div>
				</div>
			</div>
			<input stats="Pub_Send" type="submit" value="发布" disabled="disabled" class="submit" id="publisher_submit" autoComplete="false"/>			
			<input type="hidden" id="publisher_form_ticket" value="${requestScope.pubisherCode}" />		
		</form>	
		
	</div>
	

</div>
