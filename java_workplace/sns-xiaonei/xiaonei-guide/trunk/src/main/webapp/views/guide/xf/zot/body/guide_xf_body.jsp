<%@ page pageEncoding="utf-8" %>
<script type="text/javascript">
<%--
资料修改 动态获取的js 要首先在这里获取版本号
分享也要动态获取脚本
--%>
XN.getFileVersion([
    '${applicationScope.urlSmallStatic}/jspro/xn.ui.modifyinfo.js?ver=$revxxx$'
]);

</script>
<div class="search-friend">
	<div class="title"><strong>查找好友</strong></div>
	<%@ include file="/views/guide/xf/sen/middle/guide_middle_search.jsp"%>
</div><!-- End search-friend -->
<!-- 新鲜事部分 -->
<div class="guide-tabs-content" id="guideTabCon1">
    <%--新鲜不再由后台输出，ajax获取 By bo.hu@opi-corp.com 2010年05月31日 星期一 17时39分29秒
        <%@ include file="/views/guide/xf/zottf/middle/guide_news_feed.jsp"%>
    --%>
	<div class="feed-new-title">
		<div class="fliter-box">
			<strong><span>新鲜事</span></strong>
		</div>
	</div>
    <div class="news-feed">        
        <div class="feed-holder" id="feedHolder">
            <jsp:include page="/inc/n/modules/newsfeed.jsp">
            	<jsp:param name="canReadAll" value="false" />   
        		<jsp:param name="loadOnScroll" value="true" />      		
    		</jsp:include>		
        </div>
        <%--新鲜事需要用的数据--%>
        <input type="hidden" id="userIdFP" value="${visiter.id}" />
    </div>
</div><!-- 新鲜事结束 -->
<!-- 加好友部分 -->
<div class="guide-tabs-content" id="guideTabCon2" style="display:none">
	<div class="find-friend-box">		
		<div class="find-friend-body">	
			<div class="users" id="mayknow_user" style="display:none">
				<div class="title">
				<h4><strong>根据你的资料找到的好友</strong> 
					<span class="material"><a id="pro_e_school" href="javascript:;" target="_blank">修改学校</a> | <a id="pro_e_work" href="#nogo" target="_blank">修改公司</a> | <a id="pro_e_other" href="#nogo" target="_blank">其他资料</a></span>
				</h4>
				</div>
				<div class="nav-tab">
					<h2 class="clearfix" id="mayknow_tab"></h2>
					<ul class="other-infor" style="display:none" id="mayknow_tabother"></ul>
					<div class="friend-selector">
						<div class="toolbar">
							<a id="mayknow_pop_order" href="javascript:;" class="bottom" style="display:none"><em>切换到人气排序</em></a>
							<a id="mayknow_normal_order" href="javascript:;" class="bottom" style="display:none">恢复到默认排序</a>
							<a class="more" href="javascript:;" id="mayknow_showmore" style="display:none" target="_blank">查看全部 &raquo;</a>
						</div>
						<ul class="clearfix" id="mayknow_con"></ul>
					</div>
				</div>
				<div class="show-more"><input id="mayknow_btn" type="button" value="加选中的人为好友" class="input-submit"/></div>
			</div>			
		</div><!--find-friend-body end-->
		<div class="feed-btn" style="display:none"><input type="button" value="查看好友新鲜事" class="input-submit" id="guideFeedTipShow_btn" /></div>
	</div><!--find-friend-box end-->
</div>
<!-- 加好友结束 -->
