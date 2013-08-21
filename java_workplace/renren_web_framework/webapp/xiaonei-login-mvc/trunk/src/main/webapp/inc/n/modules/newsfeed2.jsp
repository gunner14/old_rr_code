<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<%--
@param showHeader 是否显示顶部
@param twoCols 是否有切换热门
@param twoColsNew 是否有切换特别关注
@param showCategories 显示分类切换
@param inited 是否展示默认新鲜事
@param loadOnScroll 是否在滚屏时自动加载
@param canReadAll 是否允许标记已读
@param hostId hostid
@param readMoreAction 加载更多的js接口地址
--%>

<c:set var="readMoreActionAttribute">
<c:if test="${!empty param.readMoreAction}"> data-readmoreaction="${param.readMoreAction}"</c:if>
</c:set>

<div class="feed-module" data-loadonscroll="${param.loadOnScroll}" data-host="${param.hostId}"${readMoreActionAttribute}>

	<%--是否有新鲜事--%>
	<c:set var="noNewsfeed" value="false" />
	<c:if test="${empty requestScope.userFeed}">
		<c:set var="noNewsfeed" value="true" />
	</c:if>

	<c:if test="${param.showHeader == true}">

		<c:set var="hotClass" value=" selected" />
		<c:set var="liveClass" value="" />
		<c:if test="${savedFeedType == 'live'}">
			<c:set var="hotClass" value="" />
			<c:set var="liveClass" value=" selected" />
		</c:if>
			
			<div class="tabview tabview-align-right">
				<h3 stats="V6Feed_refresh" class="title">
					<img src="http://a.xnimg.cn/imgpro/v6/icon/feed-hl.png" class="icon" />
					新鲜事
				</h3>
				<div class="tabview-options" style="padding-top:6px;">
					<c:choose>
						<c:when test="${ visiter.id ==200000521  || visiter.id ==  200000032|| visiter.id ==85432256 || visiter.id == 103113664|| 
						visiter.id ==229406110 || visiter.id ==236235312 || visiter.id == 261431700|| visiter.id ==258276022 || visiter.id == 241610785||
						visiter.id == 260168919|| visiter.id == 104926093|| visiter.id ==227902933 || visiter.id ==40125272 || visiter.id ==  238489851
						|| visiter.id == 233940269 || visiter.id == 128487631|| visiter.id == 303081400|| visiter.id ==227842851  || visiter.id == 447201540
						|| visiter.id == 479286781|| visiter.id ==366962033 || visiter.id == 366549583|| visiter.id ==383495187 || visiter.id == 366682680|| 
						visiter.id ==425640119 || visiter.id ==6290 || visiter.id ==62352993 || visiter.id == 200484432|| visiter.id == 30314|| 
						visiter.id == 15289|| visiter.id ==235365847 || visiter.id ==246065910 || visiter.id ==242513966 || visiter.id ==307897962 
						|| visiter.id == 201276572 || visiter.id == 410853848||visiter.id == 461193456 ||
						visiter.id ==202832841 ||visiter.id == 452208903||visiter.id %10== 9||visiter.id %10== 8||visiter.id %10== 7||visiter.id %10== 6||visiter.id %10== 5}">
							<div class="news-feed-setting-new"  onclick="showSetfeeds();">
								<img style="display:block;" title="新鲜事设置" alt="新鲜事设置" src="http://a.xnimg.cn/imgpro/v6/feed_setting.png" stats="V6Feed_option" >
							</div>
						</c:when>
						<c:otherwise>
							<a class="news-feed-setting" href="${applicationScope.urlWww}/feedprefs.do">
								<img stats="V6Feed_option" src="http://a.xnimg.cn/imgpro/v6/feed_setting.png" alt="新鲜事设置" title="新鲜事设置" style="display:block;" />
							</a>
						</c:otherwise>
					</c:choose>
				</div>	
				<div class="tabview-tabs" <c:if test="${! empty friend_group_data}">	style="display:none;" </c:if> >
					<a stats="V6Feed_attention" href="#nogo" id="feedTabAttention" data-id="attention" onclick="return false;" class="a-tab">特别关注</a>
					<a stats="V6Feed_friendFeed" href="#nogo" id="feedTabFriend" data-id="frcontent" onclick="return false;" class="a-tab ">好友内容</a>
					<a stats="V6Feed_hotFeed" href="#nogo" id="feedTabFeed" data-id="old" onclick="return false;" class="a-tab selected">热点内容</a>
				</div>
			</div>
			
			<!--
			<div class="feed-tools">
				<menu type="toolbar">
					<li class="refresh">
						<a class="reload-feed" href="#nogo" onclick="return false;">刷新</a>
					</li>
					<li class="setting">
						<a class="feed-setting" id="setNewsfeed" href="${applicationScope.urlWww}/feedprefs.do">新鲜事设置</a>
						<a class="feed-setting" id="setAttentionFeed" style="display: none;" href="${applicationScope.urlWww}/feedprefs.do">设置关注名单</a>
					</li>
					<li class="types">
						<menu class="feed-categories category-filter">
							<label id="feedTabCategories" data-id="old" class="s">
								<span class="label">
									<span class="text">新鲜事</span>
								</span>
							</label>
						</menu>
						
						<menu class="feed-attention">
							<label id="feedTabAttention" data-id="attention">
								<span class="label">
									<span class="text">特别关注</span>
								</span>
							</label>
						</menu>
					</li>
				</menu>
			</div>
			-->
	<c:if test="${! empty friend_group_data}">	

	    <div id="feed-fr-group">
			 <div id="group-all-first"><span class="group-name-a">全部</span></div>
			 <div id="group-good-friend" ><span class="group-name-b">仅好友 </span> </div> 
			 <div class=" a-line"><span class="group-name-d">|</span></div>
			 <div id="group-focus" ><span class="group-name-c">特别关注</span></div>
			 <ul id="group-list-first">			 	    
			 	    <c:forEach var="groups" items="${friend_group_data}"  begin="0" end="2">
					    <li id="${groups.groupId}"  data-id="${groups.groupId}" title="${groups.groupName}" class="group-name" >${groups.groupName}</li>
				    </c:forEach>
		     </ul>
		     <c:if test="${ fn:length(friend_group_data)> 3 }"> 
		     	 <input id="list-more" type="button" value="更多" onFocus=this.blur() />
				   
				     <div id="more-group">
				     	<div class='list-box'> 
					        <ul id="more-group-list">
					        	<c:forEach var="groups" items="${friend_group_data}" begin="3">
							       <li id="${groups.groupId}"  title="${groups.groupName}" class="more-group-name" >${groups.groupName}</li>
						        </c:forEach>
				            </ul>	    
				        </div>	
				        <div id="set-group">设置</div>            
					 </div> 
		 	 </c:if>
		     <c:if test="${ fn:length(friend_group_data)<=3 }"> 
		         <input id="set-group-less" type="button" value="设置" />
		 	 </c:if>
	 

		</div>
		<div id="Menubox_group">
          <div class ="left-area">
			<div class="group-all"><span class="group-name-a">全部</span></div>
			<div class="group-all-a"><span class="group-name-b">仅好友</span></div>
			<div class=" a-line"><span class="group-name-d">|</span></div>
			<div class="group-all-b"><span class="group-name-c">特别关注</span></div>
			<ul id="group-list">
				    <c:forEach var="groups" items="${friend_group_data}">
					       <li id="${groups.groupId}"  title="${groups.groupName}" ><span>${groups.groupName}</span></li>
				    </c:forEach>
		    </ul>
		   </div> 
		    <div class="right-area">
		    	<input id="fr-over" type="button" value="完成" /> 
		   	 	<a href="http://friend.renren.com?ref=friendGroup" target="_blank" class="control-group">管理组</a>
        		 
        	</div>	 	   
		</div>

    
	 </c:if> <!--判断好友分组end-->

	</c:if>

	<div class="feed-list">
		<c:if test="${param.inited != false}">
			<c:choose>
				<c:when test="${noNewsfeed == true}"><div class="newsfeed-empty">当你的朋友发表日志、照片或者帖子，这里会有提示。</div></c:when>
				<c:otherwise>
					<script type="text/javascript">
						if (window.UGC && UGC.Network) UGC.Network.ping.normal.start("100001", "sns.home.feedlist");
					</script>
					${fn:trim(requestScope.userFeed)}
					<script type="text/javascript">
						if (window.UGC && UGC.Network) UGC.Network.ping.normal.end("100001", "sns.home.feedlist", 2, 150);
					</script>
				</c:otherwise>
			</c:choose>
		</c:if>
	</div>

	<div id="ad100000000101"></div>

	<c:if test="${param.inited == false || noNewsfeed != true}">
		<div class="feed-footer">
			<div class="feed-loading" style="display:none"><p>新鲜事读取中...</p></div>
			<div class="nomore-feed" style="display:none"><p>没有新鲜事了，<a href="http://explore.renren.com/?from=nofeed">去探索看看</a>更多新鲜精彩内容</p></div>
			<c:if test="${param.showCategories == true}">
				<div class="nomore-feed" data-forcategoryid="5" style="display:none"><p>没有更多状态新鲜事了，你可以看看<a href="http://status.renren.com/?fromType=nomore">好友最近的状态 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="7" style="display:none"><p>没有更多相册新鲜事了，你可以看看<a href="http://photo.renren.com/photo/${visiter.id}/album/friends">好友最近的相册 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="1" style="display:none"><p>没有更多分享新鲜事了，你可以看看<a href="http://share.renren.com/">热门分享 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="6" style="display:none"><p>没有更多日志新鲜事了，你可以看看<a href="http://blog.renren.com/friendsBlog.do">好友的日志 &raquo;</a></p></div>
				<div class="nomore-feed" data-forcategoryid="20" style="display:none"><p>没有更多的公共主页新鲜事了，人人网为你推荐，快来关注吧。</p></div>
			</c:if>			
			<div class="more-feed"><a id="more-feed" href="#">更多新鲜事</a></div>	
			<c:if test="${param.canReadAll != false}"><p class="mark-all-read"><a href="javascript:;">全部标记为已读</a></p></c:if>
		</div>
	</c:if>

</div>
