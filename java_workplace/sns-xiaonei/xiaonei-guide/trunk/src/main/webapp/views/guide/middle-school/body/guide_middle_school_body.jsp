<%@ page pageEncoding="utf-8" %>
<c:set var="newFeedTitle" value="${visiter.userStateInfo.guideDone || visiter.stage==10}" />
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat" id="gossipReplyCount"></span></h2>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>
<%@ include file="/views/guide/middle-school/body/guide-middle-school-star.jsp" %>
<div id="newsfeed-module-box" class="newsfeed-module-box box">
	<jsp:include page="/inc/n/modules/newsfeed.jsp">
		<jsp:param name="twoCols" value="${isWhiteMouse}" />
		<jsp:param name="twoColsNew" value="${newFeedTitle}" />
		<jsp:param name="showHeader" value="true" />
		<jsp:param name="showCategories" value="true" />
		<jsp:param name="loadOnScroll" value="true" />
		<jsp:param name="canReadAll" value="false" />
	</jsp:include>
</div>
<div class="search-friend">
	<strong>朋友多了才好玩，快来找找老同学，认识新朋友</strong>
	<%@ include file="/views/guide/xf/sen/middle/guide_middle_search.jsp"%>
</div><!-- End search-friend -->
<div class="personMayKnow" id ="personMayKnowInSchool"></div>
<div class="personMayKnow" id ="personMayKnowInCity"></div>
<div id="mayKnowShell">
	<div class="myRR-box SG" style="display:none" id="myknowfriend_user">
		<div class="myRR-box-header">		
			<strong>你可能认识的人</strong><span>已选(<em id="myknowfriend_con_count">0</em>)</span>
	    </div>
		<div class="myRR-box-body">
			<div class="friend-selector">
				<ul id="myknowfriend_con"></ul>
			</div><!--friend-selector end-->
			<div class="add-btn"><input type="button"  id="myknowfriend_btn" class="input-submit" value="＋ 加为好友"></div>
		</div><!--myRR-box-body end-->
	</div><!--myRR-box end-->
</div>



