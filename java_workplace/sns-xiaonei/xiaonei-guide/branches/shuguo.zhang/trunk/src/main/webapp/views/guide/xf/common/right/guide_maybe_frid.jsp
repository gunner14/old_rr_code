<%@ page pageEncoding="utf-8" %>
<c:if test="${applicationScope.domain=='renren.com'}">
	<c:if test="${!empty requestScope.commonFriend && requestScope.guide_social=='social_bad'}">
		<div class="side-item pymk">
			<div class="side-item-header clearfix">
				<h4>你可能认识他们</h4>
				<div class="option">
					<a href="http://friend.${applicationScope.sysdomain}/myfriendlistx.do?portal=4#item_1">更多</a>
				</div>
			</div>
			<div class="side-item-body clearfix">
				<ul class="recognize" id="commonFriends">
					<c:forEach var="item" items="${requestScope.commonFriend}" begin="0" end="1" varStatus="status">
						<li id="commonFriend_${item.id}" class="clearfix">
	                        <a href="javascript:;" onclick="delCommonFriend('${item.id}')" class="shut" title="关闭"></a>
							<span class="headpichold">
								<a href="${applicationScope.urlWww}/profile.do?portal=2&id=${item.id}" title="查看${item.name}的个人主页">
									<img src="${item.tiny_Url}" width="50" height="50" onload="clipImage(this)" />
								</a>
							</span>
							<span>
								<a href="${applicationScope.urlWww}/profile.do?portal=3&id=${item.id}" class="name">${item.name}</a>
	                               <span class="from"></span>
	                               <p>
								   <a href="javascript:void(0);" onclick="showRequestFriendDialog('${item.id}','${item.name}','${item.tiny_Url}')" class="addfriend_action">
	                                       加为好友
								   </a>
	                              </p>
							</span>
						</li>
					</c:forEach>
				</ul>
			</div>			
		</div>
	</c:if>
</c:if>