<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>	
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
<c:set var="INC_HEADER_GAME" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_SUPPLEMENT" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_MEMBER" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_DRESSUP" value="${visiter.userStateInfo.guideDone}" />
<div id="dropmenuHolder" class="dropmenu-holder ${domainIdentified['siteNameAbbr']}">
	<c:if test="${!empty visiter}">
		<div id="optiondropdownMenu" class="menu-dropdown optionmenu" style="display:none;">
			<div class="menu-dropdown-border">
				<div class="optionmenu">
					<ul>
						<li><a class="optionaccount" href="${applicationScope.urlWww}/privacyhome.do">隐私设置</a></li>
						<li><a class="optionprivacy" href="${applicationScope.urlProfile}/account.do">帐户设置</a></li>
						<li><a class="optionapplication" href="${applicationScope.urlApp}/apps/editapps.do?origin=118">应用设置</a></li>
						<c:set var="cglink" value="${applicationScope.urlWww}/profile.do?id=${requestScope.visiter.id}&v=info&act=edit" />
						<li><a class="optionprofile" href="${cglink }">资料编辑</a></li>
					</ul>
				</div>
			</div>
		</div>
		<div id="searchdropdownMenu" class="menu-dropdown" style="display:none;width:110px;">
			<div class="menu-dropdown-border">
				<div class="search-menu">
					<ul>
						<li><a href="${applicationScope.urlBrowse}/scs.do">搜索同学</a></li>
						<li><a href="${applicationScope.urlBrowse}/wos.do">搜索同事</a></li>
						<li><a href="${applicationScope.urlBrowse}/advanced.do">高级搜索</a></li>						
					</ul>
				</div>
			</div>
		</div>
		<div id="appMenu" class="menu-dropdown" style="display:none;left:-99999px;top:-99999px;">
			<div class="menu-dropdown-border">
				<h4>最近使用&nbsp;</h4>
				<div class="menu-holder recent-app">
					<ul class="menu-item">	
						<li>
							<a href="${applicationScope.urlBlog}/?_hua=41" class="title">
								<img class="icon" width="16" height="16" src="${applicationScope.urlSmallStatic}/a.gif" style="background:url(${applicationScope.urlSmallStatic}/imgpro/icons/new_app100.png) no-repeat 0 -96px;" /> 日志
							</a>
						</li>
						<li>
							<a href="${applicationScope.urlPhoto}/?_ua_flag=52" class="title">
								<img class="icon" width="16" height="16" src="${applicationScope.urlSmallStatic}/a.gif" style="background: url(${applicationScope.urlSmallStatic}/imgpro/icons/new_app100.png) no-repeat 0 -32px;" /> 相册
							</a>
						</li>
						<li>
							<a href="${applicationScope.urlShare}" class="title">
								<img class="icon" width="16" height="16" src="${applicationScope.urlSmallStatic}/a.gif" style="background: url(${applicationScope.urlSmallStatic}/imgpro/icons/new_app100.png) no-repeat 0 -2560px;" /> 分享
							</a>
						</li>
						<c:set var="origin" value="104"/>
						<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" begin="0" end="8"><%@ include file="/inc/bookmark.inc"%></c:forEach>	
					</ul>
				</div>
				<div id="navMyApps" class="menu-holder my-app">
					<c:if test="${fn:length(requestScope.userMenu.recentMenu) > 9}">
						<ul class="menu-item">
							<c:set var="origin" value="104"/>
							<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" begin="9" end="19">
								<%@ include file="/inc/bookmark.inc"%>
							</c:forEach>
							<li class="show-more"><a href="${applicationScope.urlApp}/apps/editapps.do?origin=114">全部应用 &raquo;</a></li>
						</ul>
					</c:if>
				</div>
				<div class="separator"></div>
				<div class="app-actions">
					<ul>
						<li><a href="${applicationScope.urlApp}/apps/editapps.do?origin=105">管理我的应用</a></li>	
						<li><a href="${applicationScope.urlApp}/app/apps/list?origin=106">浏览更多应用</a></li>	
					</ul>
				</div>
			</div>
		</div>
	</c:if>
<c:otherwise>
	<div id="moredownWeb" class="dropdown" style="display:none;">
			<div class="menu-dropdown-border">
				<div class="weblist">
					<ul class="clearfix">
						<li><a href="${applicationScope.urlPhoto}">相册</a></li>
						<li><a href="${applicationScope.urlShare}">分享</a></li>
						<li><a href="${applicationScope.urlBlog}">日志</a></li>
						<li><a href="http://life.${applicationScope.sysdomain}">人人生活</a></li>
						<li><a href="http://app.${applicationScope.sysdomain}">应用列表</a></li>
						<li><a href="http://page.${applicationScope.sysdomain}">公共主页</a></li>
						<li><a href="http://zhaopin.${applicationScope.sysdomain}">人人招聘</a></li>
						<li><a href="http://club.${applicationScope.sysdomain}">人人论坛</a></li>
						<li><a href="http://name.${applicationScope.sysdomain}">同名同姓</a></li>
						<li><a href="http://school.${applicationScope.sysdomain}/allpages.html">人人中学</a></li>
						<li><a href="http://movie.${applicationScope.sysdomain}">人人影评</a></li>
						<li><a href="http://m.${applicationScope.sysdomain}">手机人人网</a></li>
					</ul>
				</div>
			</div>
		</div>
	</div>
</c:otherwise>
<div id="navBar" class="menu-bar ${domainIdentified['siteNameAbbr']}">
	<div class="navigation-wrapper">
		<div class="navigation clearfix">
			<c:choose>
				<c:when test="${INC_HEADER_ISVIP}">
					<logo:logotag mode="vip" />
				</c:when>
				<c:otherwise>
					<logo:logotag mode="trunk"/>
				</c:otherwise>
			</c:choose>		
			<div class="nav-body clearfix">
				<c:if test="${!empty visiter.id}">
					<div class="nav-main">
						<div class="menu">
							<div class="menu-title"><a href="${applicationScope.urlHome}/Home.do?id=${requestScope.visiter.id}">首页</a></div>
						</div>
						<div class="menu" style="margin:0">
							<div class="menu-title"><a href="${applicationScope.urlProfile}/profile.do?id=${requestScope.visiter.id}">个人主页</a></div>
						</div>
						<c:if test="${applicationScope.domain != 'kaixin.com'}">						
							<c:if test="${INC_HEADER_DRESSUP != false}">
								<div class="menu info-edit">
									<div class="menu-title">
										<a href="http://i.${applicationScope.domain}/click.action?from=30011&url=%2Ftpl_free.action">装扮</a>
									</div>
								</div>
							</c:if>
						</c:if>
						<div class="menu">
							<div class="menu-title"><a href="${applicationScope.urlFriend}/myfriendlistx.do">好友</a></div>
						</div>
						<div class="menu">
							<div class="menu-title with-arrow"><a href="${applicationScope.urlApp}/app/apps/list?origin=119" id="showAppMenu">应用</a></div>
						</div>
						<c:if test="${requestScope.visiter.stage==20||requestScope.visiter.stage==30}">
							<c:if test="${applicationScope.domain != 'kaixin.com'}">
								<c:if test="${INC_HEADER_GAME != false}">
									<div class="menu">
										<div class="menu-title"><a href="${applicationScope.urlGame}/">游戏</a></div>
									</div>
								</c:if>
							</c:if>
						</c:if>						
						<div class="menu">
							<div class="menu-title">
								<a id="global_inbox_link" href="${applicationScope.urlMsg}">站内信
									<c:if test="${requestScope.newMessageCount>0}">
										<span class="count">
											(<c:choose><c:when test="${requestScope.newMessageCount > 99}">99+</c:when><c:otherwise>${requestScope.newMessageCount}</c:otherwise></c:choose>)
										</span>
									</c:if>
								</a>
							</div>
						</div>
					</div>
				</c:if>
				<div class="nav-other">
					<c:if test="${!empty visiter}">
						<c:if test="${!empty visiter && visiter.stage != 10}">
							<c:if test="${applicationScope.domain != 'kaixin.com'}">
								<c:if test="${INC_HEADER_MEMBER != false}">
									<div class="menu">
										<div class="menu-title">											
											<c:choose>
												<c:when test="${visiter.vipMember ==true}">
													<a href="http://i.${applicationScope.domain}/click.action?from=10041&url=%2F">VIP中心</a>
												</c:when>
												<c:otherwise>
													<a href="${applicationScope.urlI}/pay/upgrade?wc=10000">升级VIP</a>
												</c:otherwise>
											</c:choose>	
										</div>
									</div>
								</c:if>
							</c:if>
						</c:if>
						<c:if test="${INC_HEADER_SUPPLEMENT != false}">
							<div class="menu">
								<div class="charge menu-title"><a href="${applicationScope.urlPay}">充值</a></div>
							</div>
						</c:if>
					</c:if>
					<c:choose>
						<c:when test="${!empty visiter.id}">							
							<div class="menu">
								<div class="menu-title"><a href="${applicationScope.urlInvite}/Invite.do">邀请</a></div>
							</div>
							<div class="menu">
								<div class="menu-title" id="optionMenuActive"><a href="${cglink}" onmouseover="">设置</a></div>
							</div>
							<div class="menu last">
								<div class="menu-title"><a href="${applicationScope.urlWww}/Logout.do">退出</a></div>
							</div>
							<div id="navSearch">
								<form method="post" id="globalSearchForm" action="${applicationScope.urlBrowse}/searchEx.do?from=opensearch">
									<div id="search-input">
										<input type="text" size="25" maxlength="100" id="navSearchInput" name="q" class="input-text" value="" tabindex="1" />
									</div>
									<div id="search-submit">
										<a id="navSearchSubmit" class="submit" href="${applicationScope.urlBrowse}/os.do?_lu=9">搜索</a>
										<span class="separator"></span><a id="searchMenuAction" class="more" href="#nogo"></a>
									</div>
								</form>
							</div>
						</c:when>
						<c:otherwise>
							<div class="menu">
								<div class="menu-title">
										<a title="注册" stats="homenav_reg" href="http://wwv.${applicationScope.domain}/xn.do?ss=10113&rt=27">注册</a>
								</div>
							</div>
							<div class="menu">
								<div class="menu-title">
										<a title="登录" stats="homenav_login" href="${applicationScope.urlLogin}/GLogin.do">登录</a>
								</div>
							</div>
							<div class="menu">
								<div class="menu-title">
										<a title="帮助" stats="homenav_help" href="${applicationScope.urlMain}/info/Help.do">帮助</a>
								</div>
							</div>
							<div class="menu">
								<div class="menu-title">
										<a title="给我们提建议" stats="homenav_suggest" href="${applicationScope.urlSupport}/GetGuestbookList.do?action=suggest&stage=-1">给我们提建议</a>
								</div>
							</div>
							<c:if test="${applicationScope.domain!='kaixin.com'}">
							<div class="menu last">
								<div class="menu-title" stats="homenav_more" id="moreWeb"><a href="#" onclick="this.blur();">更多</a></div>
							</div>
							</c:if>
						</c:otherwise>
					</c:choose>
				</div>
			</div>
		</div>
	</div>
</div>
