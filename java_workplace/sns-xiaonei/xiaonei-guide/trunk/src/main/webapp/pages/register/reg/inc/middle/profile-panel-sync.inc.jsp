<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<div class="photo">
					<a href="http://photo.${applicationScope.sysdomain}/getalbumprofile.do?owner=${user_id}"><img src="${head_url}" alt="${user_name}" /></a>
				<!-- 没有头像 -->
				<c:if test="${empty is_head_url || is_head_url == ''}">
					<p>完善资料才能看到TA的头像</p>
				</c:if>
				</div>
				 
				<div class="tip">
					<div>
						<h3>
							${user_name}
							<span>
								<c:if test="${!empty is_star && is_star != ''}">
									<img alt="星级用户" src="${applicationScope.urlStatic}/imgpro/icons/star.gif" />
								</c:if>
								<c:if test="${!empty footer_num && footer_num != 0}">
									(有${footer_num}人看过)
								</c:if>
							</span>   
						</h3>
						<p>${doing}</p>
					</div>
					<h4>TA在人人网上</h4>
					<ul>
						<c:if test="${!empty friend_num && friend_num != 0}">
							<li class="l_1">共有 <b>${friend_num}个好友</b></li>
						</c:if>
						<c:if test="${!empty gossip_num && gossip_num != 0}">
							<li class="l_2">收到了 <b>${gossip_num}条留言</b></li>
						</c:if>
						<c:if test="${!empty photo_num && photo_num != 0}">
							<li class="l_3">上传了 <b>${photo_num}张照片</b></li>
						</c:if>
						<c:if test="${!empty blog_num && blog_num != 0}">
							<li class="l_4">写了 <b>${blog_num}篇日志</b></li>
						</c:if>
					</ul>
				</div>