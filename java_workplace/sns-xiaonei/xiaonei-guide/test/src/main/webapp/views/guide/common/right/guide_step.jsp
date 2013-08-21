<%@page pageEncoding="UTF-8" %>
<%@ page import="java.util.*" %>
<%@ page import="net.sf.json.JSONObject" %>

<c:if test="${newbieHelperStep!=null}">
	<div id="guide-step" class="side-item">
		<div class="side-item-header clearfix">
			<h4>新手帮助</h4>
		</div>
		<div class="side-item-body clearfix">
		<% 
		 	String newbieHelperStep = (String)request.getAttribute("newbieHelperStep");
			JSONObject jsonObject = JSONObject.fromObject(newbieHelperStep);
			String nextStep = jsonObject.getString("nextStep");
			JSONObject jsonMap = jsonObject.getJSONObject("step");
			Iterator keys=jsonMap.keys();
			while(keys.hasNext()){
				String key =(String)keys.next();
				String value = (String)jsonMap.get(key);
				request.setAttribute("key",key);
 				request.setAttribute("value",value);
		%>
			<c:if test="${key == 'UPLOAD_HEAD'}">
				<div class="step">
					<c:choose>
					    <c:when test="${value == 0}">
					   	  	<p class="step-undo" id="upload-head-status">
					    </c:when>
					    <c:when test="${value ==1}">
					     	<p class="step-continue" id="upload-head-status">
					    </c:when>
					    <c:otherwise>
					     	<p class="step-complete" id="upload-head-status">
					    </c:otherwise>      
					</c:choose>
					
					<a  href="#nogo" onclick="return false;" id="upload-head">上传头像</a>
					</p>
					<span>使用真实头像，让朋友认出你</span>
				</div>
			</c:if>
			
			<c:if test="${key == 'FILLI_NFO'}">
				<div class="step">
					<c:choose>
					    <c:when test="${value == 0}">
					   	  	<p class="step-undo" id="input-info-status">
					    </c:when>
					    <c:when test="${value ==1}">
					     	<p class="step-continue" id="input-info-status">
					    </c:when>
					    <c:otherwise>
					     	<p class="step-complete" id="input-info-status">
					    </c:otherwise>      
					</c:choose>
					
					<a onclick="return false;" href="#nogo" id="input-info">完善资料</a>
					</p>
					<span>马上填写资料，让朋友了解你</span>
				</div>
			</c:if>
			<c:if test="${key == 'SEARCH_FRIEND'}">
				<div class="step">
					<c:choose>
					    <c:when test="${value == 0}">
					   	  	<p class="step-undo" id="add-friend-status">
					    </c:when>
					    <c:when test="${value ==1}">
					     	<p class="step-continue" id="add-friend-status">
					    </c:when>
					    <c:otherwise>
					     	<p class="step-complete" id="add-friend-status">
					    </c:otherwise>      
					</c:choose>
					<a href="http://friend.renren.com/myfriendlistx.do#item_1" id="add-friends">寻找好友</a>
					</p>
					<span>立即找到好友，建立相互联系</span>
				</div>
			</c:if>
			
			<c:if test="${key == 'UPLOAD_PHOTO'}">
				<div class="step">
					<c:choose>
					    <c:when test="${value == 0}">
					   	  	<p class="step-undo" id="upload-photo-status">
					    </c:when>
					    <c:when test="${value ==1}">
					     	<p class="step-continue" id="upload-photo-status">
					    </c:when>
					    <c:otherwise>
					     	<p class="step-complete" id="upload-photo-status">
					    </c:otherwise>      
					</c:choose>
					<a onclick="return false;" class="flashUploader" href="#nogo" id="upload-photo">上传照片</a>
					</p>
					<span>立即找到好友，建立相互联系</span>
				</div>
			</c:if>
		<% 
			}
		%>

		</div>
	</div>
</c:if>