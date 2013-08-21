<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<div class="photo">
	<img src="http://head.xiaonei.com/photos/0/0/men_head.gif"  id="profile-head_url" alt="${user_name}" />
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
		<p id="profile-doing"></p>
	</div>
	<h4>TA在人人网上</h4>
	<ul>
			<li id="profile-friend_num"></li>
			<li id="profile-gossip_num"></li>
			<li id="profile-photo_num"></li>
			<li id="profile-blog_num"></li>
	</ul>
</div>
<script type="text/javascript">
	new ajaxGetUserInfo('${user_id}');
</script>
