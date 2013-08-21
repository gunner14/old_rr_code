<%@page pageEncoding="UTF-8" %>
<%@ page import="java.util.*" %>
<%@ page import="net.sf.json.JSONObject" %>

<c:if test="${newbieHelperStep!=null}">
	<style type="text/css">
                    #guide-step {
                        background:#fffadf;
                        border: 1px solid #ebe3b3;
                        padding:10px 15px 0 15px;
                    }
					#guide-step .side-item-header{
						padding:0 0 8px;
					}
                    #guide-step .step{
                        margin-bottom: 10px;
                    }
                    #guide-step p {
                        line-height:18px;
                    }
                    #guide-step .step-complete{
                        background:url('http://a.xnimg.cn/imgpro/bg/guide-step.png') no-repeat 0 0 #fffadf;
                    }
                    #guide-step .step-continue{
                        background:url('http://a.xnimg.cn/imgpro/bg/guide-step.png') no-repeat 0 -18px #fffadf;
                    }
                    #guide-step .step-undo{
                        background:url('http://a.xnimg.cn/imgpro/bg/guide-step.png') no-repeat 0 -36px #fffadf;
                    }
                    #guide-step p a{
                        font-weight: bold;
                    }
                    #guide-step p, #guide-step span{
                        padding-left:24px;
                    }
					
					#guide-step1{
						width:264px;
						height:134px;
						background:url('http://a.xnimg.cn/imgpro/bg/guide-friend1.png') no-repeat 0 0;
						position:absolute;
					}
					#guide-step1 .close{
						position:absolute;
						display:block;
						width:20px;
						height:20px;
						left:228px;
						top:10px;
					}
					#guide-step1 .next{
						position:absolute;
						display:block;
						width:90px;
						height:25px;
						left:159px;
						top:81px;
					}
					#guide-step2{
						width:284px;
						height:135px;
						background:url('http://a.xnimg.cn/imgpro/bg/guide-friend2.png') no-repeat 0 0;
						position:absolute;
					}
					#guide-step2 .close{
						position:absolute;
						display:block;
						width:20px;
						height:20px;
						left:248px;
						top:10px;
					}
					#guide-step2 .next{
						position:absolute;
						display:block;
						width:90px;
						height:25px;
						left:180px;
						top:84px;
					}
					#guide-step3{
						position:absolute;
						display:block;
						width:178px;
						height:45px;
						background:url('http://a.xnimg.cn/imgpro/bg/friend-back-home.png') no-repeat 0 0;
						z-index:9999;
					}
					#guide-step3 .close{
						position:absolute;
						display:block;
						width:9px;
						height:9px;
						left:156px;
						top:13px;
					}
                </style>
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