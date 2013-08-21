<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<c:set var="newsFeedUrl" value="http://www.renren.com/home?from=homeleft" /> 
<c:if test="${!visiter.userStateInfo.guideDone}">
	<c:set var="newsFeedUrl" value="http://guide.renren.com/guide?from=homeleft" /> 
</c:if>
<c:set var="newFeedTitle" value="${visiter.userStateInfo.guideDone || requestScope.newStageTen}" />
<div class="site-menu-user-box box">
	<article class="site-menu-user">
		<div class="figure">
			<a href="${applicationScope.urlPhoto}/getalbumprofile.do?owner=${visiter.id}"><img src="${userBasic.tiny_Url}" width="50" height="50" alt="${visiter.name}" class="boxes" onload="clipImage(this);" /></a>
		</div>
        <div class="site-menu-user-info">
			<p class="name-and-icons">
				<a class="name" href="${applicationScope.urlWww}/profile.do?id=${visiter.id}" title="${visiter.name}">${visiter.name}</a>
				<span class="icons">
					<c:if test="${requestScope.isVip==1}">
						<a style="${requestScope.homeVipIcon}" class="vip-level" href="${applicationScope.urlI}/icon" title="点击更换VIP图标">${VipLevel}</a>
					</c:if>
					<c:if test="${requestScope.isVip==0}">
						<a class="expired vip-level" href="${applicationScope.urlI}/icon" title="点击更换VIP图标">${VipLevel}</a>
					</c:if>
					<c:if test="${requestScope.isVip!=0 && requestScope.isVip!=1 && visiter.id%10 == 9}">
						<a class="notvip vip-level" href="${applicationScope.urlI}/icon" title="点击设置图标"></a>
					</c:if>
				</span>
			</p>
			<c:choose>
				<c:when test="${host.page}">
					<div class="page-admin-wrapper">
						<a href="http://page.renren.com/${host.id}/admin/" class="link"><img src="http://a.xnimg.cn/imgpro/page/page-admin-left.png"></a>
					</div>
				</c:when>
				<c:otherwise>
            <c:if test="${!empty myLevel}">
                <a onmouseover="$('gradeHover').show();" onmouseout="$('gradeHover').hide();" class="level" href="http://sc.${applicationScope.domain}/scores/myscore"><c:forEach var="icon" items="${scoreIcons}"><img class="${icon}" src="http://a.xnimg.cn/a.gif"></c:forEach></a>
				<span class="description" id="homeUserScore">${loginCDays}天</span>
				<div class="grade-hover" id="gradeHover" style="display: none;">
					<div class="arrow"></div>
					<span class="grade-num">${myLevel}级</span>
					<span class="grade-cur">${currentScore}/${nextLevelScore}</span>
					<div class="slide-bar">
						<span class="slide-bar-cur" style="width: ${(scorelevels.score-scorelevels.lastLevelScore)*100/(scorelevels.nextLevelScore-scorelevels.lastLevelScore)}%;"></span>
					</div>
					<p class="live-days">连续登录${loginCDays}天</p>
				</div>
			</c:if>
			
			<c:if test="${showAwardCount}">
				<div class="user-score clearfix" id="" style="display:none">
					<a class="award" href="http://sc.${applicationScope.domain}/scores/award">奖励(${awardSize})</a>
					<c:if test="${showTip}">
						<div id="scoreTipDiv" class="side-item-body" style="position:absolute;left:-9999px;">
							<div class="new-point-tips" style="width:150px;">
								<div class="left-arrow" style="top:8px;"></div>
								<div id="" class="tipsbody">
									<span><a href="http://sc.${applicationScope.domain}/scores/award" style="background:url(http://a.xnimg.cn/imgpro/home/score.png) 0 2px no-repeat;padding-left:15px;" onclick="if(socialFeedStats){socialFeedStats('201101_reward');}">你有${awardSize}个奖励未领取</a></span>
								</div>
							</div>  
						</div>
						<script type="text/javascript">
							XN.dom.readyDo(function() {
								setTimeout(function(){
									var scroeTip = new XN.ui.fixPositionElement(
									{
										id: 'scoreTipDiv',
										alignWith : 'homeUserScore',
										alignType: '2-1',
										tagName : 'div',
										offsetX: 10,
										offsetY: -5
									});
									setTimeout(function(){$('scoreTipDiv').hide();},3000);
								},10000);
							});
						</script>
					</c:if>
				</div>
			</c:if>	

			
			<c:if test="${showTip}">
				<div class="got-score">
					<div class="increase-tips" style="display:block" id="loginTipDiv">
						<div class="arrow-top"></div>
						<div class="tip-content">
							<p>连续登录${loginCDays}天</p>
							<p><c:if test="${requestScope.isVip==1}">VIP</c:if>每天<em>+<fmt:formatNumber value="${loginscore}" pattern="#.#" /></em>分</p>
						</div>
					</div>
					<script type="text/javascript">
						XN.dom.readyDo(function() {
						setTimeout(function(){$('loginTipDiv').hide();},3000);
						});
					</script>
				</div>
			</c:if>
				</c:otherwise>
			</c:choose>

		</div>
	</article>
</div>

<div class="site-menu-nav-box box">
	<article id="site-menu-nav" class="site-menu-nav">
		<section>
			<div class="nav-item">
				<div class="item-title<c:if test="${newFeedTitle}"> single-item</c:if>">
					<span class="item-main">
					<c:choose>
						<c:when test="${visiter.userStateInfo.guideDone}">
							<a href="${home_url}" accesskey="n" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/res/icons/newsfeed.png" /> 新鲜事</a>
						</c:when>
						<c:otherwise>
							<a href="${newsFeedUrl}" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/res/icons/guidev2.gif" /> 玩转人人</a>
						</c:otherwise>
					</c:choose>
					</span>
				</div>
				<c:if test="${!newFeedTitle}">
					<ul>
						<li><a href="http://www.renren.com/homeAttention?from=homeleft" ui-async="async" id="attentionFeeds" class="with-count">特别关注</a></li>
					</ul>
				</c:if>
			</div>
			<div class="nav-item">
				<div class="item-title"><span class="item-main"><a stats="V5Homeleft_blog" href="http://blog.renren.com/blog/0/friendsNews?from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/res/icons/blog.png" /> 日志</a></span>
					<span class="item-operation"><a stats="V5Homeleft_compose" href="http://blog.renren.com/NewEntry.do" accesskey="p">发表</a></span>
				</div>
				<ul>
					<li><a stats="V5Homeleft_myBlog" href="http://blog.renren.com/blog/0?from=homeleft" ui-async="async">我的日志</a></li>
					<li><a stats="V5Homeleft_drafts" href="http://blog.renren.com/blog/0/drafts?from=homeleft" ui-async="async">草稿箱</a></li>
					<li><a stats="V5Homeleft_myRelatives" href="http://blog.renren.com/blog/0/myRelatives?from=homeleft" ui-async="async">与我相关的日志</a></li>
				</ul>
			</div>
			<div class="nav-item">
				<div class="item-title"><span class="item-main"><a stats="V5Homeleft_album" href="http://photo.renren.com/photo/${visiter.id}/album/friends?from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/res/icons/photo.png" /> 相册</a></span>
					<span class="item-operation"><a stats="V5Homeleft_uploadPhoto" class="flashUploader" href="http://upload.renren.com/index.do" accesskey="u">上传</a></span>
				</div>
				<ul>
					<li><a stats="V5Homeleft_myAlbum" href="http://photo.renren.com/photo/${visiter.id}?from=homeleft" ui-async="async">我的相册</a></li>
					<li><a stats="V5Homeleft_circleMe" href="http://photo.renren.com/photo/${visiter.id}/tag?from=homeleft" ui-async="async">圈有我的照片 <img src="http://a.xnimg.cn/imgpro/icons/new.png" class="new" /></a></li>
					<li><a stats="V5Homeleft_onlineActive" href="http://event.renren.com/?from=homeleft" ui-async="async">线上活动</a></li>
				</ul>
			</div>
            <div class="nav-item">
				<div class="item-title"><span class="item-main"><a stats="V5Homeleft_music" href="http://music.renren.com/?from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/res/icons/music.png" /> 音乐</a></span>
					<span class="item-operation"><a stats="V5Homeleft_openMusicBox" href="javascript:openMusicBox();" accesskey="m">听歌</a></span>
				</div>
				<ul>
					<li><a stats="" href="http://music.renren.com/fm?from=homeleft" target="_blank">人人电台</a></li>
					<li><a stats="" href="http://music.renren.com/personal?from=homeLeft" ui-async="async">我的音乐</a></li>
				</ul>
			</div>
			<div class="nav-item">
				<div class="item-title"><span class="item-main"><a stats="V5Homeleft_share" href="http://share.renren.com/share/hot?from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/res/icons/share.png" /> 分享</a></span></div>
				<ul>
					<li><a stats="V5Homeleft_frdShare" href="http://share.renren.com/share/friend?from=homeleft" ui-async="async">好友的分享</a></li>
					<li><a stats="V5Homeleft_myShare" href="http://share.renren.com/share/${visiter.id}?from=homeleft" ui-async="async">我的分享</a></li>
					<li><a stats="V5Homeleft_myCollection" href="http://share.renren.com/share/collection?from=homeleft" ui-async="async">我的收藏</a></li>
				</ul>
			</div>
			
			<div class="nav-item">
				<div class="item-title"><span class="item-main"><a stats="V5Homeleft_zhan" href="http://zhan.renren.com/home?from=homeTab&id=${visiter.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/zhan.png" /> 小站 <img src="http://a.xnimg.cn/imgpro/icons/new.png" class="new" /></a></span></div>
			</div>
			
			<div class="nav-item">
				<div class="item-title"><span class="item-main"><a stats="V5Homeleft_xiaozu" href="http://xiaozu.renren.com/xiaozu/index?from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/n/apps/group/res/xz_ico.png" /> 小组</a></span></div>
				<ul>
					<li><a href="http://xiaozu.renren.com/xiaozu/friends?from=homeleft" ui-async="async">好友的小组</a></li>
					<li><a href="http://xiaozu.renren.com/xiaozu/search?from=homeleft" ui-async="async">全部小组</a></li>
				</ul>
			</div>
			
			<c:if test="${not show_nuomi_menu }">
				<div class="nav-item">
					<div class="item-title"><span class="item-main"><a href="http://class.renren.com/sf/myclass/show?from=homeleft" ui-async="async"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/alumna.png" /> 校友录</a></span></div>
				</div>
			</c:if>
			
			<c:if test="${show_univ_2011_menu}">
				<div class="nav-item">
					<div class="item-title single-item"><span class="item-main"><a href="http://school.renren.com"><img class="icon" src="http://app.xnimg.cn/application/20100601/18/30/LhSrm99n018153.gif" /> 校园频道</a></span></div>
				</div>
			</c:if>
			
            <c:if test="${show_nuomi_menu }">
			   <div class="nav-item">
					<div class=""><span class="item-main"><a href="http://www.nuomi.com?utm_source=renren&utm_medium=left-tuangou&utm_campaign=renren&cid=003208" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/nuomi.gif" /> 团购</a></span></div>
				</div>
			</c:if>	
			
			
		</section>
		<section class="minigroups">
			<c:forEach var="item" items="${requestScope.miniGrouplist}" begin="0" end="2">
				<div class="nav-item">
					<div class="item-title">
						<span class="item-main">
							<a href="http://qun.renren.com/qun/ugc/home/${item.id}?from=homeleft" class="with-count" id="mg_${item.id}" ui-async="async">
								<img class="${item.icon} icon" src="http://a.xnimg.cn/a.gif" /> 
								${item.name}
								<c:if test="${item.newFeedsNum>0}">
									<span class="nav-item-count">${item.newFeedsNum}</span>
								</c:if>
							</a>
						</span>
					</div>
				</div>
			</c:forEach>
			<c:if test="${fn:length(requestScope.miniGrouplist)>3}">
				<div id="site-menu-minigroups-all" style="display:none">
					<c:forEach var="itema" items="${requestScope.miniGrouplist}" begin="3" end="9">
						<div class="nav-item">
							<div class="item-title">
								<span class="item-main">
									<a href="http://qun.renren.com/qun/ugc/home/${itema.id}?from=homeleft" class="with-count" id="mg_${itema.id}" ui-async="async">
										<img class="${itema.icon} icon" src="http://a.xnimg.cn/a.gif" /> 
										${itema.name}
									</a>
								</span>
							</div>
						</div>
					</c:forEach>
					<div class="minigroup-create">
						<script type="text/javascript">
							XN.getFileVersion([
								'http://s.xnimg.cn/n/apps/minigroup/core/js/manage.js?ver=$revxxx$'
							]);
						</script>
						<a href="#" onclick="if(!XN.app.miniGroup){XN.loadFile('http://s.xnimg.cn/n/apps/minigroup/core/js/manage.js',function(){XN.app.miniGroup.createGroup();});}else{XN.app.miniGroup.createGroup();}return false;">创建群</a>
					</div>
				</div>
			</c:if>
			<c:if test="${fn:length(requestScope.miniGrouplist)<=3 || empty requestScope.miniGrouplist }">
				<div class="minigroup-create">
					<script type="text/javascript">
						XN.getFileVersion([
							'http://s.xnimg.cn/n/apps/minigroup/core/js/manage.js?ver=$revxxx$'
						]);
					</script>
					<a href="#" onclick="if(!XN.app.miniGroup){XN.loadFile('http://s.xnimg.cn/n/apps/minigroup/core/js/manage.js',function(){XN.app.miniGroup.createGroup();});}else{XN.app.miniGroup.createGroup();}return false;">创建群</a>
				</div>
			</c:if>
			<c:if test="${fn:length(requestScope.miniGrouplist)>3}">
			<div class="clearfix">
					<div class="minigroups-more" id="site-menu-minigroups-more">
						<a href="#" onclick="return false;">展开</a>
					</div>
					<div class="minigroups-less" id="site-menu-minigroups-less" style="display:none">
						<a href="#" onclick="return false;">收起</a>
					</div>
					<div class="minigroups-link" style="display:none" id="all-minigroups-link">
						<a href="http://qun.renren.com/qun/index?from=homeleft" ui-async="async">我的所有群</a>
					</div>
			</div>
			</c:if>
		</section>
				
	</article>
	
	<article class="site-menu-apps">
    
        <section>
            <div id="appsItem_26" class="apps-item">
                <div class="item-title">
                    <span class="item-main">
                        <c:set var="requestCount" value="0" />
                        <c:forEach items="${requestScope.userCountMap}" var="entry"> 
                            <c:set var="key" value="${entry.key}"></c:set>
                            <c:if test="${key==0 || key==40}"><%--0代表邀请，40代表礼物--%>
                                <c:set var="requestCount" value="${requestCount + entry.value}" />
                            </c:if>
                        </c:forEach>
                        <c:set var="appCenterUrl">
                            <c:choose>
                                <c:when test="${requestCount>0}">http://app.${applicationScope.sysdomain}/app/appRequestList?origin=700045</c:when>
                                <c:otherwise>http://app.${applicationScope.sysdomain}/?origin=101</c:otherwise>
                            </c:choose>
                        </c:set>
                        <c:set var="appCenterText">
                            <c:choose>
                                <c:when test="${requestCount>0}">应用请求</c:when>
                                <c:otherwise>应用中心</c:otherwise>
                            </c:choose>
                        </c:set>
                        <c:if test="${requestCount>0}">
                        <a href="${appCenterUrl}" onclick="if($('allAppsInvite')) $('allAppsInvite').remove();" target="_blank" class="with-count"><img width="16" height="16" src="http://app.xnimg.cn/application/20101018/16/35/L1MGd11n018153.gif" class="icon">  ${appCenterText}
                            <c:if test="${requestCount>99}">
                                <c:set var="requestCount" value="99+" />
                            </c:if>
                            <span id="allAppsInvite" class="nav-item-count" title="${requestCount}条应用请求">${requestCount}</span>
                            <c:if test="${requestScope.disAppInvite}">
                                <input type="hidden" id="showAppInviteTip" />
                            </c:if>
                        </a>
                        </c:if>
                    </span>
                </div>
            </div>
			<c:set var="origin">
				<c:choose>
					<c:when test="${visiter.userStateInfo.guideDone}">103</c:when>
					<c:otherwise>103&guide</c:otherwise>
				</c:choose>
			</c:set>
			
			<%-- 置顶的应用 --%>
			<c:forEach var="item" items="${requestScope.userMenu.topMenu}">
				<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
				<c:set var="target" value="" />
				<c:if test="${!empty item.urlTarget}">
					<c:set var="target" value=" target=\"${item.urlTarget}\"" />
				</c:if>
				<c:choose>
					<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
					<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
				</c:choose>
				<div class="apps-item" id="appsItem_${item.appId}">
					<div class="item-title">
						<span class="item-main">
							<a href="${sideNavUrl}${sp}origin=${origin}"${target}>
								<c:if test="${!noDel}">
									<span class="x-to-hide-holder"><span class="x-to-hide" onclick="delHomeApp(event,'${item.appId}','http://apps.renren.com/menu/removeTopApp')"></span></span>
								</c:if>
								<c:choose>
									<c:when test="${!empty appPicItems[item.appId]}">
										<img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background-image:url(${applicationScope.urlSmallStatic}/imgpro/icons/new-app100.png);background-position:0 ${-appPicItems[item.appId]}px;" />
									</c:when>
									<c:otherwise>
										<img class="icon" width="16" height="16" src="${item.appIcon}" /> 
									</c:otherwise>
								</c:choose> 
								${item.appName}
							</a>
						</span>
					</div>
				</div>
			</c:forEach>
			
			<c:set var="currentTime"><%=System.currentTimeMillis() %></c:set>
			<c:set var="showBegin" value="1335891600000" />
			<c:set var="showEnd" value="1336575600000" />
			<c:if test="${currentTime>=showBegin && currentTime<=showEnd}">
			<%-- <div class="apps-item" id="appsItem_189913">
				<div class="item-title">
					<span class="item-main">
						<a href="http://show.renren.com?ref=sitemenu" target="_blank">
							<img class="icon" width="16" height="16" src="http://a.xnimg.cn/imgpro/icons/show.png" /> 
							缤纷五月,欢乐秀
						</a>
					</span>
				</div>
			</div> --%>
			</c:if>
			
            <c:if test="${visiter.stage==10}">
                <div class="apps-item">
                    <div class="item-title">
                        <span class="item-main">
                            <a href="http://mm.${applicationScope.sysdomain}?origin=103<c:if test="${!visiter.userStateInfo.guideDone}">&guide</c:if>" target="_blank">
                                <img src="${applicationScope.urlStatic}/imgpro/icons/xiaohua2.gif" class="icon" width="16" height="16" /> 
                                校花校草
                            </a>
                        </span>
                    </div>
                </div>
                <div class="apps-item">
					<div class="item-title">
						<span class="item-main">
							<a href="http://taohua.renren.com?origin=103<c:if test="${!visiter.userStateInfo.guideDone}">&guide</c:if>" target="_blank">
								<img width="16" height="16" src="http://app.xnimg.cn/application/20101101/16/20/L2aGR88k018153.gif" class="icon" /> 
								桃花朵朵
							</a>
						</span>
					</div>
				</div>
            </c:if>
			
			<c:set var="begin" value="0" />
			<c:set var="end" value="7" />
            <%@ include file="site-menu-apps-item.jsp"%>
        </section>
        <section id="site-menu-apps-all" style="display:none">
			<c:set var="begin" value="8" />
			<c:set var="end" value="100" />
			<c:set var="origin">
				<c:choose>
					<c:when test="${visiter.userStateInfo.guideDone}">50001</c:when>
					<c:otherwise>50001&guide</c:otherwise>
				</c:choose>
			</c:set>
            <%@ include file="site-menu-apps-item.jsp"%>
			
			<c:forEach var="item" items="${requestScope.userMenu.recentMenu}">
				<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
				<c:set var="target" value="" />
				<c:if test="${!empty item.urlTarget}">
					<c:set var="target" value=" target=\"${item.urlTarget}\"" />
				</c:if>
				<c:choose>
					<c:when test="${fn:contains(sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
					<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
				</c:choose>
				<c:choose>
					<%-- 人人视频 --%>
					<c:when test="${item.appId==147394}">
						<c:if test="${visiter.id!=350790771}">
							<div class="apps-item" id="appsItem_${item.appId}">
								<div class="item-title">
									<span class="item-main">
										<a href="${sideNavUrl}${sp}origin=${origin}"${target}>
											<img src="${item.appIcon}" class="icon" width="16" height="16" /> 
											${item.appName} 
										</a>
									</span>
									<span class="item-operation"><a href="http://page.renren.com/media/video/videoUpload?pid=600756710&bizType=videoalbum">上传</a></span>
								</div>
							</div>
						</c:if>
					</c:when>
					<c:otherwise></c:otherwise>
				</c:choose>
			</c:forEach>
			
        </section>
        <section>
            <c:if test="${fn:length(requestScope.userMenu.recentMenu) > 8}">
                <div class="apps-more" id="site-menu-apps-more"><a href="#" onclick="return false;">更多</a></div>
                <div class="apps-less" id="site-menu-apps-less" style="display:none"><a href="#" onclick="return false;">收起</a></div>
            </c:if>
            <div class="apps-link"><a href="http://app.renren.com?origin=101">应用中心</a></div>
        </section>
		
	</article>
	<c:if test="${!empty recentMenuHome || timeStamp<=20111103}">
		<article class="site-menu-apps recommend">
			<h4>推荐应用</h4>
			<section>
				<c:if test="${timeStamp<=20111105}">
					<div class="apps-item">
						<div class="item-title">
							<span class="item-main">
								<a href="http://creditcard.renren.com/">
									<img width="16" height="16" src="http://a.xnimg.cn/imgpro/icons/lmk2.gif" class="icon" /> 招行人人联名卡
								</a>
							</span>
						</div>
					</div>
				</c:if>
				<c:forEach var="item" items="${recentMenuHome}" begin="0" end="2">
				<c:choose>
					<c:when test="${fn:contains(item.sideNavUrl, '?')}"><c:set var="sp" value="&" /></c:when>
					<c:otherwise><c:set var="sp" value="?" /></c:otherwise>
				</c:choose>
				<div class="apps-item">
					<div class="item-title">
						<span class="item-main">
							<a href="${item.sideNavUrl}${sp}origin=50031">
								<img width="16" height="16" src="${item.icon}" class="icon" /> ${item.appName}
							</a>
						</span>
					</div>
				</div>
				</c:forEach>
			</section>
		</article>
	</c:if>
</div>

