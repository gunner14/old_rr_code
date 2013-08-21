<%@ page pageEncoding="utf-8" %>
<script type="text/javascript">
<%--
资料修改 动态获取的js 要首先在这里获取版本号
--%>
XN.getFileVersion(
[
    '${applicationScope.urlSmallStatic}/jspro/xn.ui.modifyinfo.js?ver=$revxxx$'
]);
</script>
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat" id="gossipReplyCount"></span></h2>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>
<div class="guide-tabs">
	<p class="clearfix">
		<a href="javascript:void(0)" id="guideTab1" onfocus="this.blur()">好友新鲜事</a>
		<a href="javascript:void(0)" id="guideTab2" onfocus="this.blur()">查找好友</a>
		<a href="javascript:void(0)" id="guideTab3" onfocus="this.blur()">游戏大厅</a>	
	</p>								
</div><!--guide-tabs end-->
<div class="guide-tabs-content" id="guideTabCon1" style="display:none">
	<%@ include file="/views/guide/xf/zottf/middle/guide_news_feed.jsp"%><%--新鲜事 --%>
</div>
<div class="guide-tabs-content" id="guideTabCon2" style="display:none">
	<%@ include file="/views/guide/xf/sen/middle/guide_middle_search.jsp"%>
	<div class="find-friend-box">		
		<div class="find-friend-body">			
			<h5>先添加好友吧！<span>(<img height="12" width="13"  title="在线用户" src="${applicationScope.urlStatic}/imgpro/icons/online_1.gif" class="on-line"/>为在线用户)</span></h5>
			<div class="users" id="myknowfriend_user" style="display:none"> 
				<h4>你可能认识他们</h4>											
				<div class="friend-selector">
					<div class="toolbar"><a class="more" id="myknowfriend_showmore" href="javascript:;" style="display:none" target="_blank">查看全部 &raquo;</a></div>
					<ul id="myknowfriend_con"></ul>
				</div>
				<div class="show-more" style="display:none;"><input id="myknowfriend_btn" type="button" value="加选中的人为好友" class="input-submit"/></div>	
			</div>				
			<div class="users" id="mayknow_user" style="display:none">
				<h4>根据你的资料找到的好友 
					<span class="material"><a id="pro_e_school" href="#nogo" target="_blank">修改学校</a> | <a id="pro_e_work" href="#nogo" target="_blank">修改公司</a> | <a id="pro_e_other" href="#nogo" target="_blank">其他资料</a></span>
				</h4>
				<div class="nav-tab">
					<h2 class="clearfix" id="mayknow_tab"></h2>
					<ul class="other-infor" style="display:none" id="mayknow_tabother"></ul>
					<div class="friend-selector">
						<div class="toolbar">
							<a id="mayknow_pop_order" href="#nogo" class="bottom" style="display:none"><em>切换到人气排序</em></a>
							<a id="mayknow_normal_order" href="#nogo" class="bottom" style="display:none">恢复到默认排序</a>
							<a class="more" href="javascript:;" id="mayknow_showmore" style="display:none" target="_blank">查看全部 &raquo;</a>
						</div>
						<ul class="clearfix" id="mayknow_con"></ul>
					</div>
				</div>
				<div class="show-more" style="display:none;"><input id="mayknow_btn" type="button" value="加选中的人为好友" class="input-submit"/></div>
			</div>			
		</div><!--find-friend-body end-->
		<div class="feed-btn"><input type="button" value="查看好友新鲜事" class="input-submit" id="guideFeedTipShow_btn"></div>
	</div><!--find-friend-box end-->
</div>
<div class="guide-tabs-content" id="guideTabCon3" style="display:none">
    <div class="game-hall loading">
        <h1>我们猜你还喜欢</h1>
       <ul class="clearfix" id="maylike_con">
        </ul>
    </div><!--game-hall end-->
    <div class="clearfix">
        <div class="game-bang">
            <div class="bang-nav-tabs">
                <ul class="clearfix">
                    <li class="selected" id="mmgg_lbl"><a href="javascript:;" onfocus="this.blur()">MM最爱</a></li>
                    <li id="mmgg_lbl1"><a href="javascript:;" onfocus="this.blur()">GG最爱</a></li>
                </ul>
            </div>
            <div class="game-lists loading">
                <ul id='mmgg_con'> </ul>
                <ul id='mmgg_con1'> </ul>
            </div>
        </div><!--game-bang end-->
        <div class="game-bang margin-left-right">
            <div class="bang-nav-tabs">
                <h4>最流行</h4>
            </div>
            <div class="game-lists loading">
                <ul id="mostpop_con"> </ul>
            </div>
            <div class="game-more">
                <a href="${applicationScope.urlApp}/list?selected=2&amp;origin=3783" target="_blank">更多&raquo;</a>
            </div>
        </div><!--game-bang end-->
        <div class="game-bang">
            <div class="bang-nav-tabs">
                <h4>最活跃</h4>                
            </div>
            <div class="game-lists loading">
                <ul id="tools_con"> </ul>
            </div>
            <div class="game-more">
                <a href="${applicationScope.urlApp}/list?select=3&amp;origin=3743" target="_blank">更多&raquo;</a>
            </div>
        </div><!--game-bang end-->
    </div><!--clearfix end-->
    <div class="friend-nowApp loading">
        <h1>朋友正在使用</h1>
        <ul class="clearfix" id="friendsuse_con"></ul>
    </div><!--friend-nowApp end-->
</div><!--guide-tabs-content end-->

