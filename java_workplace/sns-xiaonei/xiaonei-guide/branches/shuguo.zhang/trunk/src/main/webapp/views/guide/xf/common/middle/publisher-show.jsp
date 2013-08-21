<%@ page pageEncoding="utf-8" %>
<div class="status-publisher">
    <form method="post" autocomplete="off" action="${applicationScope.urlUpload}/addphotosingle.do" id="publisher_upload_form" enctype="multipart/form-data" target="publisher_upload">
	    <textarea  style=""  name="status" id="publisher_statusInput" stats="Pub_Status"></textarea>
	    <div class="emotions" id="publisher_emotion" style="display:none">
            <ul class="emotion">
               <c:forEach var="item" items="${ubbList}">
			   <li ${fn:contains(item.src, 'lg-lolipop/faceicon.gif') ? 'class="wider"' : ''}>
			   <a href="#nogo"><img emotion="${item.ubb}" alt="${item.alt}" title="${item.alt}" src="http://s.xnimg.cn/a.gif" rsrc="${staticUrl}${item.src}"/></a>
			   </li>
			   </c:forEach>
            </ul>
	    </div>
		<div style="overflow: hidden; height: 28px; display: block;" id="publisher_tools" class="publisher-footer">	
		    <span class="current-status" id="currentStatus">		    	
                <c:choose>
                	<c:when test="${empty requestScope.ugcstatus && !visiter.userStateInfo.guideDone}">                	
                	</c:when>
                	<c:otherwise>
                		<a title="${requestScope.fullstatus}" href="${applicationScope.urlStatus}/getdoing.do?id=${requestScope.ubid}">
                			${updateTitle}:${requestScope.ugcstatus}
                		</a>
                	</c:otherwise>
                </c:choose>                
		    </span>
		    <div>
			<span class="status-attachments">
				<a class="share" stats="Pub_Share" href="#nogo" id="publisher_share">分享</a>
				<a href="#nogo" class="photo">
					照片<input type="file" name="file" size="1" class="addfile" style="" id="publisher_file" stats="Pub_Photo" />
				</a>
				<iframe style="display: none;" src="${applicationScope.urlProfile}/ajaxProxy.html" name="publisher_upload"></iframe>
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