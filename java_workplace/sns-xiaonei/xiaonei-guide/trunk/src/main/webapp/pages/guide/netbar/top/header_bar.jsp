<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>	
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
<%@ include file="/inc/wpi-menu.inc" %>
<div id="dropmenuHolder" class="dropmenu-holder ${domainIdentified['siteNameAbbr']}">
	<div id="optiondropdownMenu" class="menu-dropdown optionmenu" style="display:none;">
		<div class="menu-dropdown-border">
			<div class="optionmenu">
				<ul>
					<li><a class="optionaccount" href="${applicationScope.urlWww}/privacyhome.do">隐私设置</a></li>
					<li><a class="optionprivacy" href="${applicationScope.urlProfile}/account.do">帐户设置</a></li>
					<li><a class="optionapplication" href="${applicationScope.urlApp}/apps/editapps.do?origin=118">应用设置</a></li>
					<c:set var="cglink" value="${applicationScope.urlWww}/profile.do?id=${requestScope.visiter.id}&v=info&act=edit" />
					<c:if test="${visiter.selected == 0}"><c:set var="cglink" value="${applicationScope.urlHeadUpload}/Upload.do" /></c:if>
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
					<li><a href="${applicationScope.urlBlog}/?_hua=41" class="title"><img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background:url(${applicationScope.urlSmallStatic}/imgpro/icons/new_app100.png) no-repeat 0 -96px;" /> 日志</a></li>
					<li><a href="${applicationScope.urlPhoto}/?_ua_flag=52" class="title"><img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background: url(${applicationScope.urlSmallStatic}/imgpro/icons/new_app100.png) no-repeat 0 -32px;" /> 相册</a></li>
					<li><a href="${applicationScope.urlShare}" class="title"><img class="icon" width="16" height="16" src="http://s.xnimg.cn/a.gif" style="background: url(${applicationScope.urlSmallStatic}/imgpro/icons/new_app100.png) no-repeat 0 -2560px;" /> 分享</a></li>
					<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" begin="0" end="8"><%@ include file="/inc/bookmark.inc"%></c:forEach>
				</ul>
			</div>
			<div id="navMyApps" class="menu-holder my-app">
				<c:if test="${fn:length(requestScope.userMenu.recentMenu) > 9}">
					<ul class="menu-item">
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
					<li><a href="${applicationScope.urlApp}/apps/apps.do?ref=top_menu&selected=3&type=1&origin=106">浏览更多应用</a></li>
				</ul>
			</div>
		</div>
	</div>
</div>
<div id="navBar" class="menu-bar ${domainIdentified['siteNameAbbr']}">
	<div class="navigation-wrapper">
		<div class="navigation clearfix">
			<c:if test="${empty cleanpage}">
	            <c:if test="${visiter.loginCount > 6}">
	            <c:choose>
	        	    <c:when test="${visiter.stage == 10}">
	                    <%@ include file="/inc/promo-wide-highschool.inc" %>
	            	</c:when>
	                <c:when test="${visiter.stage == 20}">
	            	    <%@ include file="/inc/promo-wide.inc" %>
	                </c:when>
	                <c:when test="${visiter.stage == 30}">
		                <%@ include file="/inc/promo-wide.inc" %>
	            	</c:when>
	                <c:otherwise>
			            <%@ include file="/inc/promo-wide.inc" %>
	                </c:otherwise>
	            </c:choose>
				</c:if>	
			</c:if>			
			<c:choose>
				<c:when test="${INC_HEADER_ISVIP==1}">
					<logo:logotag mode="vip"/>
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
						<div class="menu">
							<div class="menu-title"><a href="${applicationScope.urlFriend}/myfriendlistx.do">好友</a></div>
						</div>
						<div class="menu">
							<div class="menu-title with-arrow"><a href="${applicationScope.urlApp}/apps/apps.do?selected=3&type=1&origin=119" id="showAppMenu">应用</a></div>
						</div>	
						<div class="menu">
							<div class="menu-title"><a href="${applicationScope.urlGame}/">游戏</a></div>
						</div>					
						<div class="menu">
							<div class="menu-title"><a id="global_inbox_link" href="${applicationScope.urlMsg}">站内信<c:if test="${requestScope.newMessageCount>0}"><span class="count">(<c:choose><c:when test="${requestScope.newMessageCount > 99}">99+</c:when><c:otherwise>${requestScope.newMessageCount}</c:otherwise></c:choose>)</span></c:if></a></div>
						</div>
					</div>
				</c:if>
				<div class="nav-other">					
					<c:choose>
						<c:when test="${!empty visiter.id}">												
							<div class="menu">
								<div class="menu-title"><a href="${applicationScope.urlInvite}/Invite.do">邀请</a></div>
							</div>
							<div class="menu">
								<div class="menu-title" id="optionMenuActive"><a href="#nogo" onmouseover="">设置</a></div>
							</div>
									<%
									        String ticket,code="";
									        ticket = com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request,
									                        com.xiaonei.platform.core.opt.base.PassportManager.loginKey_ticket);
									        if(ticket!=null){
									                code+=ticket.hashCode();
									        }
									%>
							<div class="menu last">
								<div class="menu-title"><a href="${applicationScope.urlWww}/Logout.do?get_check=<%=code%>">退出</a></div>
							</div>
							<div id="navSearch">
								<form method="post" id="globalSearchForm" action="${applicationScope.urlBrowse}/searchEx.do?from=opensearch">
									<div id="search-input">
										<input type="text" size="25" maxlength="100" id="navSearchInput" name="q" class="input-text" value="" tabindex="1" />
									</div>
									<div id="search-submit">
										<a id="navSearchSubmit" class="submit" href="${applicationScope.urlBrowse}/os.do?_lu=9">搜索</a><span class="separator"></span><a id="searchMenuAction" class="more" href="#nogo"></a>
									</div>
								</form>
							</div>
						</c:when>
						<c:otherwise>
							<div class="menu">
								<div class="menu-title"><a href="http://wwv.${applicationScope.domain}/xn.do?ss=10113&rt=27">注册</a></div>
							</div>
							<div class="menu">
								<div class="menu-title"><a href="${applicationScope.urlLogin}/GLogin.do">请登录</a></div>
							</div>
							<div class="menu">
								<div class="menu-title"><a href="#nogo" onclick="XN.Browser.addHomePage('${applicationScope.urlWww}')">设为首页</a></div>
							</div>
							<div class="menu">
								<div class="menu-title"><a href="#nogo" onclick="XN.Browser.addBookMark('${applicationScope.urlWww}')">加入收藏</a></div>
							</div>
							<div class="menu last">
								<div class="menu-title"><a href="${applicationScope.urlMain}/info/Help.do">帮助</a></div>
							</div>
						</c:otherwise>
					</c:choose>
				</div>
			</div>
		</div>
	</div>
</div>

