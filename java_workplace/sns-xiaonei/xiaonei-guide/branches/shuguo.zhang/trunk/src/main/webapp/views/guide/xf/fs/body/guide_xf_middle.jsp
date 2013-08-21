<%@ page pageEncoding="utf-8" %>
<%@ include file="/views/guide/xf/sen/middle/guide_middle_search.jsp"%>
<div class="find-friend-box" id="myknowfriend_user" style="display:none">
	<div class="find-friend-body">
		<h5>选择认识的人</h5>
		<div class="users" > 
			<h4>他们是<strong>你可能认识的人</strong>
				<a class="more" id="myknowfriend_showmore" href="javascript:;" style="display:none" target="_blank">查看全部 &raquo;</a>
			</h4>											
			<div class="friend-selector">
				<ul id="myknowfriend_con"></ul>				
			</div>
			<div class="show-more"><input id="myknowfriend_btn" type="button" value="加选中的人为好友" class="input-submit"/></div>	
		</div>	
	</div>
</div>
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat" id="gossipReplyCount"></span></h2>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>	
<%@ include file="/views/guide/xf/sen/middle/guide_news_feed.jsp"%><%--新鲜事 --%>
<div class="find-friend-box">	
	<c:if test="${!empty requestScope.userFeed}">
		<h3>加更多好友，看更多新鲜事</h3>
	</c:if>	
	<div class="find-friend-body">
		<div class="feed-arrow"></div>				
		<div class="users" id="mayknow_user" style="display:none">
			<h4>根据你的资料找到的好友 
				<a href="${applicationScope.urlWww}/profile.do?id=${requestScope.visiter.id}&amp;v=info&amp;act=edit&amp;pma=p_guide_pub_m_friend_a_info" class="material" target="_blank">修改资料 </a>
				<a class="more" href="javascript:;" id="mayknow_showmore" style="display:none" target="_blank">查看全部 &raquo;</a>
			</h4>
			<div class="nav-tab">
				<h2 class="clearfix" id="mayknow_tab"></h2>
				<ul class="other-infor" style="display:none" id="mayknow_tabother"></ul>
				<div class="friend-selector">
					<ul class="clearfix" id="mayknow_con"></ul>					
				</div>
			</div>
			<div class="show-more"><input id="mayknow_btn" type="button" value="加选中的人为好友" class="input-submit"/></div>
		</div>
		<div class="users" id="another_user" style="display:none">
			<h4>随便看看</h4>
			<div class="nav-tab">
				<h2 class="clearfix" id="another_tab">					
				</h2>
				<div class="friend-selector">
					<ul class="clearfix" id="another_con"></ul>					
				</div>
			</div>
			<div class="show-more"><input id="another_btn" type="button" value="加选中的人为好友" class="input-submit"/></div>
		</div>	
		<c:if test="${domainIdentified['siteNameAbbr']=='rr'}">	
			<div class="users pages" id="guidepage_user" style="display:none">
				<h4>关注真实明星，立即获取他们的日志、相册、状态等...
					<a class="more" href="javascript:;" id="guidepage_showmore" style="display:none" target="_blank">查看更多 &raquo;</a>
				</h4>
				<div class="friend-selector" >
					<ul class="clearfix" id="guidepage_con"></ul>
				</div>
				<div class="show-more"><input id="guidepage_btn" type="button" value="关注选中的明星" class="input-submit"/></div>
			</div>	
		</c:if>
	</div><!--find-friend-body end-->
</div><!--find-friend-box end-->
<%@ include file="/views/guide/xf/sen/middle/guide_hot_share.jsp"%>


                            	
