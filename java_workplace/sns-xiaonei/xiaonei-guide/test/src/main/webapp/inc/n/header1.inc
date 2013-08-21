<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>	
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>

<c:set var="INC_HEADER_GAME" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_SUPPLEMENT" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_MEMBER" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_DRESSUP" value="${visiter.userStateInfo.guideDone}" />

<c:set var="homeUrl" value="${applicationScope.urlHome}/Home.do?id=${requestScope.visiter.id}" />
		<input type="hidden" id="userGuideType" value="${visiter.userStateInfo.guideDone}" />
		<c:if test="${requestScope.isFromIM != true}">
			<div id="dropmenuHolder" class="dropmenu-holder dropmenu-holder-new ${domainIdentified['siteNameAbbr']}">
				<c:if test="${!empty visiter}">
				
					<div id="accountDropDownMenu" class="menu-dropdown" style="display: none">
						<div class="menu-dropdown-border">
							<div class="account-menu">
								<div class="accounts" id="otherAccount" style="display:none"></div>
								
								<div class="optionmenu options">
									<ul>
										<li><a class="optionprivacy" href="http://safe.${applicationScope.sysdomain}/">帐户设置</a></li>
										<li><a href="http://i.renren.com/" class="optionvip">VIP中心</a></li>
										<li><a href="http://pay.renren.com/" class="optionpay">充值中心</a></li>
										<c:if test="${applicationScope.domain == 'renren.com' and channel=='首页'}">
											<c:if test="${!setGray}">
												<li><a class="optiondressup" onclick="switchSkin();return false;" href="#">首页换肤</a></li>
											</c:if>
										</c:if>
										<%-- <li><a class="optionmail" href="http://mail.${applicationScope.sysdomain}/subscribe">邮件订阅</a></li> --%>
									</ul>
								</div>
								
								<a accesskey="0" href="${applicationScope.urlLogin}/Logout.do?requesttoken=${requestScope.postFormSession}" class="logout">退出</a>
							</div>
						</div>
					</div>

					<c:choose>
						<c:when test="${host.page}">
							<div id="profileMenu" class="menu-dropdown" style="display:none;">
								<div class="menu-dropdown-border">
									<div class="optionmenu">
										<ul>
											<c:set var="cglink" value="${applicationScope.urlMain}/profile.do?id=${requestScope.visiter.id}&v=info&act=edit" />
											<li><a href="http://page.renren.com/${host.id}/admin/" class="optionpageadmin">管理后台</a></li>
											<li><a href="http://page.renren.com/mng/entry?pid=${host.id}&biztype=index" class="optionprofileskin">装扮主页</a></li>
											<li><a class="optionprofile" href="http://page.renren.com/${host.id}/info">修改资料</a></li>
											<li><a class="optionlover" href="http://lover.renren.com/home">情侣空间</a></li>
										</ul>
									</div>
								</div>
							</div>
						</c:when>
						<c:otherwise>
							<div id="profileMenu" class="menu-dropdown" style="display:none;">
								<div class="menu-dropdown-border">
									<div class="optionmenu">
										<ul>
											<c:set var="cglink" value="${applicationScope.urlMain}/profile.do?id=${requestScope.visiter.id}&v=info&act=edit" />
											<li><a href="${applicationScope.urlI}/store/view/home?wc=10000" class="optionprofileskin">装扮主页</a></li>
											<li><a class="optionprofile" href="${cglink}">修改资料</a></li>
											<li><a class="optionlover" href="http://lover.renren.com/home">情侣空间</a></li>
										</ul>
									</div>
								</div>
							</div>
						</c:otherwise>
					</c:choose>
					<div id="friendMenu" class="menu-dropdown" style="display:none;">
						<div class="menu-dropdown-border">
							<div class="optionmenu">
								<ul>
									<li><a href="http://friend.renren.com/myfriendlistx.do#item_0" class="optionallfriend">全部好友</a></li>
									<li><a href="http://friend.renren.com/myfriendlistx.do#item_1" class="optionfindfriend">寻找好友</a></li>
									<li><a href="${applicationScope.urlFriend}/myfriendlistx.do#item_5" class="optiontongxunlu">通讯录</a></li>
									<li><a class="optioninvite" href="http://invite.renren.com/Invite.do">邀请朋友</a></li>
								</ul>
							</div>
						</div>
					</div>
					
					<div id="optiondropdownMenu" class="menu-dropdown optionmenu" style="display:none;">
						<div class="menu-dropdown-border">
							<div class="optionmenu">
								<ul>
									<c:set var="cglink" value="${applicationScope.urlMain}/profile.do?id=${requestScope.visiter.id}&v=info&act=edit" />
									<li><a class="optionprofile" href="${cglink}">资料编辑</a></li>
									<li><a class="optionaccount" href="${applicationScope.urlWww}/privacyhome.do">隐私设置</a></li>
									<li><a class="optionapplication" href="${applicationScope.urlApp}/apps/editapps.do?origin=118">应用设置</a></li>
									<li><a class="optionprivacy" href="http://safe.${applicationScope.sysdomain}/">帐户设置</a></li>
									<c:if test="${applicationScope.domain == 'renren.com' and channel=='首页'}">
										<li><a class="optiondressup" onclick="switchSkin();return false;" href="#">首页换肤</a></li>
									</c:if>
									<%-- <li><a class="optionmail" href="http://mail.${applicationScope.sysdomain}/subscribe">邮件订阅</a></li> --%>
								</ul>
							</div>
						</div>
					</div>
					<div id="appsMenuPro" class="menu-dropdown" style="position: absolute; display: block; z-index: 10001; left: -9999px; top: -9999px; ">
                              
            		</div>
				</c:if>
				<c:if test="${empty visiter}">
					<div id="moredownWeb" class="dropdown" style="position:absolute; top:-9999px; left:-9999px;">
						<div class="menu-dropdown-border">
							<div class="weblist">
								<ul class="clearfix">
									<li><a href="${applicationScope.urlShare}">分享</a></li>
                                    <li><a href="http://app.${applicationScope.sysdomain}">应用程序</a></li>
                                    <li><a href="http://page.${applicationScope.sysdomain}">公共主页</a></li>
                                    <li><a href="http://life.${applicationScope.sysdomain}">人人生活</a></li>
                                    <li><a href="http://xiaozu.${applicationScope.sysdomain}/">人人小组</a></li>
                                    <li><a href="http://name.${applicationScope.sysdomain}">同名同姓</a></li>
                                    <li><a href="http://school.${applicationScope.sysdomain}/allpages.html">人人中学</a></li>
                                    <li><a href="http://school.${applicationScope.sysdomain}/daxue/">大学百科</a></li>
                                    <li><a href="http://life.renren.com/hot">人人热点</a></li>
								</ul>
							</div>
						</div>
					</div>
				</c:if>
			</div>
			<div id="topbar"><c:if test="${emailIsConfirmed==false && channel=='首页'}">
					<div id="account-vertify-topbar" style="background:#FFFBC1;border-bottom:1px solid #F9B967;padding:5px;text-align:center">
						<img src="${applicationScope.urlSmallStatic}/imgpro/icons/activemail.png" style="vertical-align:middle" />
						您的帐号还没有通过认证，您将无法使用网站的部分功能，
						<a href="#" onclick="showConfirmDialog();return false;" style="zoom:1;vertical-align:baseline" >马上去认证您的帐号&raquo;</a>
					</div>
				</c:if></div>
			<c:if test="${!NO_EXTRA_DIV}"><div id="container"></c:if><%--匹配在footer.inc中--%>
				<div id="header">
					<div id="navBar" class="site-nav ${domainIdentified['siteNameAbbr']}">
						<div class="navigation-wrapper">
							<div class="navigation navigation-new clearfix">
							
								<c:choose>
									<c:when test="${INC_HEADER_ISVIP==1}">
										<logo:logotag mode="vip"/>
									</c:when>
									<c:otherwise>
									   <div id="logo2">
											<h1>
												<a title="人人网 renren.com - 人人网校内是一个真实的社交网络，联系朋友，一起玩游戏" href="http://www.renren.com"><img width="159" height="35" src="http://s.xnimg.cn/imgpro/logo/logo-rr-159.png" title="人人网 renren.com - 人人网校内是一个真实的社交网络，联系朋友，一起玩游戏" alt="人人网 renren.com - 人人网校内是一个真实的社交网络，联系朋友，一起玩游戏">
												</a>
											</h1>
									   </div>                  
									</c:otherwise>
								</c:choose>

								<div class="nav-body clearfix">
									<c:if test="${!empty visiter.id}">
										<div class="nav-main">
											<c:set var="homeUrl" value="${applicationScope.urlHome}/home?id=${requestScope.visiter.id}" />
											<c:set var="home_url_r">
												<c:choose>
													<c:when test="${channel == '首页'}">${home_url}</c:when>
													<c:otherwise>${homeUrl}</c:otherwise>
												</c:choose>
											</c:set>
											<div class="menu">
												<div class="menu-title"><a stats="V5Hd_home" accesskey="1" href="${home_url_r}"<c:if test="${channel=='首页'}"> ui-async="async"</c:if>>首页<span class="drop-menu-btn"></span></a></div>
											</div>
											<c:choose>
												<c:when test="${host.page}">
													<div class="menu">
														<div id="profileMenuActive" class="menu-title with-drop-menu"><a id="showProfileMenu" accesskey="2" stats="V5Hd_Profile" href="${applicationScope.urlProfile}/profile.do?id=${requestScope.visiter.id}">公共主页<span class="drop-menu-btn" id="profileDropMenu"></span></a></div>
													</div>
												</c:when>
												<c:otherwise>
													<div class="menu">
														<div id="profileMenuActive" class="menu-title with-drop-menu"><a id="showProfileMenu" accesskey="2" stats="V5Hd_Profile" href="${applicationScope.urlProfile}/profile.do?id=${requestScope.visiter.id}">个人主页<span class="drop-menu-btn" id="profileDropMenu"></span></a></div>
													</div>
												</c:otherwise>
											</c:choose>
											<div class="menu">
												<c:choose>
													<c:when test="${visiter.userStateInfo.guideDone}">
														<c:set var="navFriendUrl" value="${applicationScope.urlFriend}/myfriendlistx.do" />
													</c:when>
													<c:otherwise>
														<c:set var="navFriendUrl" value="http://guide.renren.com/headerFriend" />
													</c:otherwise>
												</c:choose>
												<div class="menu-title with-drop-menu" id="friendMenuActive"><a id="showFriendMenu" accesskey="3" stats="V5Hd_frd" href="${navFriendUrl}">好友<span class="drop-menu-btn" id="friendDropMenu"></span></a></div>
											</div>							
											<c:if test="${visiter.stage==10}">
												<div class="menu">
													<div class="menu-title"><a stats="Hd_Random" href="${applicationScope.urlBrowse}/b.do">随便看看<span class="drop-menu-btn"></span></a></div>
												</div>							
											</c:if>
											<div class="menu">
												<c:choose>
													<c:when test="${visiter.id%10==3 && visiter.userState.socialGraphGood==true && visiter.userState.guideDone==false}">
														<div class="menu-title"><a href="${applicationScope.urlBrowse}/search.do">找人<span class="drop-menu-btn"></span></a></div>
													</c:when>
													<c:otherwise>
														<div class="menu-title with-drop-menu">
															<a stats="V5Hd_appCenter" href="${applicationScope.urlApp}/?origin=119" accesskey="4" id="showAppMenu">应用<span class="drop-menu-btn" id="appDropMenu"></span></a>
														</div>
													</c:otherwise>
												</c:choose>								
											</div>
											<c:if test="${applicationScope.domain=='kaixin.com' || requestScope.visiter.stage==20||requestScope.visiter.stage==30}">
												<c:if test="${INC_HEADER_GAME != false}">
													<div class="menu">
														<div class="menu-title"><a stats="V5Hd_webGame" accesskey="5" href="${applicationScope.urlGame}/">游戏<span class="drop-menu-btn"></span></a></div>
													</div>
												</c:if>
											</c:if>						
											
										</div>
									</c:if>
									<div class="nav-other">
										<c:if test="${!empty visiter}">
										
											<div class="menu">
												<div class="menu-title">
													<a id="global_inbox_link" accesskey="6" stats="Hd_msg" href="${applicationScope.urlMsg}">
														<c:if test="${requestScope.newMessageCount>0}"><span class="msg-count"><span class="count"><c:choose><c:when test="${requestScope.newMessageCount > 99}">99+</c:when><c:otherwise>${requestScope.newMessageCount}</c:otherwise></c:choose></span></span></c:if><span<c:if test="${requestScope.newMessageCount>0}"> class="msg-text"</c:if>>站内信</span>
													</a>
												</div>
											</div>
										
											
										</c:if>
										<c:choose>
											<c:when test="${!empty visiter.id}">													
												
												<div class="menu">
													<div class="menu-title" id="searchMenuAction"><a accesskey="7" href="${applicationScope.urlBrowse}/search.do?_sgrf=navi" onmouseover="" stats="Hd_sMenu">搜索</a></div>
												</div>
												<%
												String ticket,code="";
												ticket = com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request,
												com.xiaonei.platform.core.opt.base.PassportManager.loginKey_ticket);
												if(ticket!=null){
												code+=ticket.hashCode();
												}
												%>
												<div id="navSearch">
													<form method="post" id="globalSearchForm" action="${applicationScope.urlBrowse}/searchEx.do?from=opensearch">
														<div id="search-input">
															<input stats="Hd_sInput" type="text" size="25" maxlength="100" id="navSearchInput" accesskey="/" name="q" class="input-text" value="" tabindex="1" />
														</div>
														<div id="search-submit">
															<a stats="Hd_sBtn" id="navSearchSubmit" class="submit" href="${applicationScope.urlBrowse}/search.do?_sgrf=navi">搜索</a>
														</div>
													</form>
												</div>
											
													
														<div class="menu account-action" id="accountMenu">
															<div class="menu-title"><a href="#" onclick="return false;">帐号</a></div>
														</div>
													
											
											</c:when>
											<c:otherwise>
												<div class="menu">
													<div class="menu-title">
														<strong>
															<a id="reg_link" title="注册" stats="homenav_reg" href="http://wwv.${applicationScope.domain}/xn.do?ss=10113&amp;rt=27">注册</a>
														</strong>
													</div>
												</div>
												<div class="menu">
													<div class="menu-title">
														<strong>
															<a title="登录" stats="homenav_login" href="${applicationScope.urlWww}/">登录</a>
														</strong>
													</div>
												</div>
												<c:if test="${false and channel == '登录页'}">
													<div class="menu" id="set-homepage">
														<script type="text/javascript">
															XN.DOM.ready(function(){$('set-homepage').innerHTML='<div class="menu-title"><a href="#" onclick="XN.Browser.addHomePage(\'${applicationScope.urlWww}\');return false;">设为首页</a></div>';});
														</script>
													</div>
													<div class="menu" id="add-bookmark">
														<script type="text/javascript">
															XN.DOM.ready(function(){$('add-bookmark').innerHTML='<div class="menu-title"><a href="#" onclick="XN.Browser.addBookMark(\'${applicationScope.urlWww}\');return false;">加入收藏</a></div>';});
														</script>
													</div>
												</c:if>
												<div class="menu">
													<div class="menu-title">
														<strong>
															<a title="帮助" stats="homenav_help" href="${urlSupport}/helpcenter">帮助</a>
														</strong>
													</div>
												</div>
												<div class="menu">
													<div class="menu-title">
														<strong>
															<a title="给我们提建议" stats="homenav_suggest" href="${urlSupport}/link/suggest">给我们提建议</a>
														</strong>
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
				</div>
		</c:if>
				
		

