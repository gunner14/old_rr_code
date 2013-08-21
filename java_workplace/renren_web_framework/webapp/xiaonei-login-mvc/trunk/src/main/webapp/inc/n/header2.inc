<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>	
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>

<c:set var="INC_HEADER_GAME" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_SUPPLEMENT" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_MEMBER" value="${visiter.userStateInfo.guideDone}" />
<c:set var="INC_HEADER_DRESSUP" value="${visiter.userStateInfo.guideDone}" />

<c:set var="homeUrl" value="${applicationScope.urlHome}/Home.do?id=${requestScope.visiter.id}" />
<c:set var="showNewNav" value="${(visiter.userState.doubleAccount || visiter.id%1000==289) && applicationScope.domain=='renren.com'}" />

		<div id="showNewNav" style="display:none"></div>
		<c:if test="${requestScope.isFromIM != true}">
			<div id="dropmenuHolder" class="dropmenu-holder dropmenu-holder-new ${domainIdentified['siteNameAbbr']}">
				<c:if test="${!empty visiter}">
					<div id="accountDropDownMenu" class="menu-dropdown" style="display: none">
						<div class="menu-dropdown-border">
							<div class="account-menu">
								<div class="accounts" id="otherAccount" style="display:none"></div>
								
								<c:if test="${!host.page}">
									<div class="optionmenu options">
										<ul>
											<li><a class="optionprivacy" href="http://safe.${applicationScope.sysdomain}/">帐户设置</a></li>
											<li><a href="http://i.renren.com/" class="optionvip">VIP中心</a></li>
											<li><a href="http://pay.renren.com/" class="optionpay">充值中心</a></li>
											<c:if test="${applicationScope.domain == 'renren.com' and channel=='首页'}">
												<c:if test="${!setGray&&visiter.userStateInfo.guideDone}">
													<li><a class="optiondressup" onclick="switchSkin();return false;" href="#">首页换肤</a></li>
												</c:if>
											</c:if>
											<%-- <li><a class="optionmail" href="http://mail.${applicationScope.sysdomain}/subscribe">邮件订阅</a></li> --%>
										</ul>
									</div>
								</c:if>
								<div class="optionsupport"><a href="http://help.renren.com/helpcenter">联系客服</a></div>
								<a accesskey="0" href="${applicationScope.urlLogin}/Logout.do?requesttoken=${requestScope.postFormSession}" class="logout">退出</a>
							</div>
						</div>
					</div>
					
					<div id="appsMenuPro" class="menu-dropdown" style="position: absolute; display: block; z-index: 10001; left: -9999px; top: -9999px; "></div>
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
                                    <li><a href="http://school.${applicationScope.sysdomain}/allpages.html">校园百科</a></li>
                                    <li><a href="http://school.${applicationScope.sysdomain}/daxue/">大学百科</a></li>
									<li><a href="http://life.renren.com/hot">人人热点</a></li>
									<li><a href="http://zhan.renren.com/">人人小站</a></li>
                            		<li><a href="http://j.renren.com/">人人逛街</a></li>
                           			<li><a href="http://xiaozhao.renren.com/">人人校招</a></li>
								</ul>
							</div>
						</div>
					</div>
				</c:if>
			</div>
			
			<div class="site-nav-wrapper">
				<div id="navBar" class="site-nav fixed">
					<div id="navigation-for-buddylist">
						<div class="navigation-wrapper">
							<div class="navigation navigation-new clearfix">


								<c:choose>
									<c:when test="${channel=='首页'}">
										<logo:logotag mode="home"/>
									</c:when>
									<c:otherwise>
										<logo:logotag mode="trunk"/>
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
												<div class="menu-title"><a accesskey="1" href="${fn:escapeXml(home_url_r)}"<c:if test="${channel=='首页'}"> ui-async="async"</c:if>><span class="menu-title-text" stats="V6Hd_home">首页</span></a></div>
											</div>
											<c:set var="profileMenuText">
												<c:choose>
													<c:when test="${host.page}">公共主页</c:when>
													<c:otherwise>个人主页</c:otherwise>
												</c:choose>
											</c:set>
											<div class="menu">
												<div id="profileMenuActive" class="menu-title"><a id="showProfileMenu" accesskey="2" href="${applicationScope.urlProfile}/profile.do?id=${requestScope.visiter.id}"><span stats="V6Hd_Profile" class="menu-title-text">${profileMenuText}</span><span class="nav-drop-menu-btn"></span></a></div>
											</div>
											
											<c:if test="${!host.page}">
												<div class="menu">
													<c:choose>
														<c:when test="${visiter.userStateInfo.guideDone}">
															<c:set var="navFriendUrl" value="${applicationScope.urlFriend}/myfriendlistx.do" />
														</c:when>
														<c:otherwise>
															<c:set var="navFriendUrl" value="http://guide.renren.com/headerFriend" />
														</c:otherwise>
													</c:choose>
													<div class="menu-title" id="friendMenuActive"><a id="showFriendMenu" accesskey="3" href="${navFriendUrl}"><span class="menu-title-text" stats="V6Hd_frd">好友</span></a></div>
												</div>
											</c:if>							
											
											<c:choose>
												<c:when test="${visiter.id%10==3 && visiter.userState.socialGraphGood==true && visiter.userState.guideDone==false}">
												<div class="menu">
													<div class="menu-title"><a href="${applicationScope.urlBrowse}/search.do"><span class="menu-title-text">找人</span></a></div>
												</div>				
												</c:when>
												<c:otherwise></c:otherwise>
											</c:choose>
											<c:if test="${!host.page}">
												<div class="menu">
													<div class="menu-title" id="showAppMenu">
														<a href="http://app.renren.com?origin=40182" target="_blank"><span class="menu-title-text">应用</span><span class="nav-drop-menu-btn"></span>
														</a>
													</div>
												</div>
											</c:if>
											<c:set var="DO_NOT_SHOW_EXPLORE" value="false" /> <%-- 显示探索入口的开关，值改为true就可以关掉入口了 --%>
											<c:if test="${!DO_NOT_SHOW_EXPLORE}">
												<div class="menu">
													<div class="menu-title">
														<a href="http://explore.renren.com/">
															<span class="menu-title-text" stats="V6Hd_exploer">探索</span>
														</a>
													</div>
												</div>
											</c:if>
										
										</div>
									</c:if>
									<div class="nav-other">
										<div id="navMessage">
											<span class="remind" title = "提醒">
											<a href="#nogo" onclick="return false;" hidefocus="true">&nbsp;</a>
											<i style='display:none;'><u><b>&nbsp;</b><var>0</var></u><em>&nbsp;</em></i>
											</span>
											<span class="apply" title = "请求">
											<a href="#nogo" onclick="return false;" hidefocus="true">&nbsp;</a>
											<i style='display:none;'><u><b class="apply">&nbsp;</b><var>0</var></u><em>&nbsp;</em></i>
											</span>
											<span class="notice" title = "通知">
											<a href="#nogo" onclick="return false;" hidefocus="true">&nbsp;</a>
											<i style='display:none;'><u><b class="notice">&nbsp;</b><var>0</var></u><em>&nbsp;</em></i>
											</span>
											<div id="showMessage" style="display:none;">
											<div id="loadTip">&nbsp;</div>
											<iframe name="showMessageIframe" id="showMessageIframe" src="about:blank" frameborder="0" allowtransparency='true' scrolling="no" width="294" height="0"></iframe>
											</div>
										</div>
										<c:choose>
											<c:when test="${!empty visiter.id}">
												<div id="navSearch">
													<form method="post" id="globalSearchForm" action="${applicationScope.urlBrowse}/searchEx.do?from=opensearch">
														<div id="search-input">
															<input stats="Hd_sInput" type="text" size="25" maxlength="100" id="navSearchInput" accesskey="/" name="q" class="input-text" value="" tabindex="1" />
														</div>
														<a href="javascript:;" id="searchBtnAC">搜索</a>
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
															<a title="帮助" stats="homenav_help" href="${applicationScope.urlMain}/info/Help.do">帮助</a>
														</strong>
													</div>
												</div>
												<div class="menu">
													<div class="menu-title">
														<strong>
															<a title="给我们提建议" stats="homenav_suggest" href="${applicationScope.urlSupport}/GetGuestbookList.do?action=suggest&amp;stage=-1">给我们提建议</a>
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
							
							<div class="nav-drop-menu-holder" id="navDropMenuHolder">
								<c:if test="${!empty visiter.id}">
									<c:set var="profileEditLink">
										<c:choose>
											<c:when test="${host.page}">http://page.renren.com/${host.id}/info</c:when>
											<c:otherwise>${applicationScope.urlMain}/profile.do?id=${requestScope.visiter.id}&amp;v=info&amp;act=edit</c:otherwise>
										</c:choose>
									</c:set>
									<c:set var="profileDressLink">
										<c:choose>
											<c:when test="${host.page}">http://page.renren.com/mng/entry?pid=${host.id}&amp;biztype=index</c:when>
											<c:otherwise>${applicationScope.urlI}/store/view/home?wc=10000</c:otherwise>
										</c:choose>
									</c:set>
									<div class="nav-drop-menu nav-drop-menu-profile" id="navProfileDropMenu">
										<c:if test="${host.page}">
										<a href="http://page.renren.com/${host.id}/admin/" class="nav-drop-menu-item"><img src="http://a.xnimg.cn/imgpro/icons/application_2.gif" class="icon" /> 管理后台</a>
										</c:if>
										<a class="nav-drop-menu-item" href="${profileDressLink}"><img src="http://a.xnimg.cn/imgpro/icons/Vipdraw.gif" class="icon" /> 装扮主页</a>
										<a class="nav-drop-menu-item" href="${profileEditLink}"><img src="http://a.xnimg.cn/imgpro/icons/account.gif" class="icon" /> 修改资料</a>
										<c:if test="${!host.page}">
											<c:if test="${ !(empty homeLeftSites||fn:length(homeLeftSites)==0) }">
												<c:set var="hasZhan" value="true" />
											</c:if>
											<c:if test="${ !(empty managePageList||fn:length(managePageList)==0) }">
												<c:set var="hasPage" value="true" />
											</c:if>
											<c:if test="${ !(empty manageLoverPageList||fn:length(manageLoverPageList)==0) }">
												<c:set var="hasLoverSpace" value="true" />
											</c:if>
											<c:if test="${ !(empty manage_groups_data||fn:length(manage_groups_data)==0) }">
												<c:set var="hasGroup" value="true" />
											</c:if>
											<c:if test="${hasZhan||hasPage||hasLoverSpace || hasGroup}">
											<div class="my-admins">
												<h4>我管理的</h4>
												<c:if test="${hasPage}">
													<c:forEach var="item" items="${managePageList}">
														<a class="my-admins-item" title="${item.name}" href="http://page.renren.com/${item.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/follow.gif" /> ${fn:substring(item.name, 0, 6)}</a>
													</c:forEach>
												</c:if>
												<c:if test="${hasGroup}">
													<c:forEach var="item" items="${manage_groups_data}">
														<a class="my-admins-item" title="${item.name}" href="http://xiaozu.renren.com/xiaozu/${item.id}?ref=profile
" target="_blank"><img class="icon" src="http://app.xnimg.cn/application/20120607/17/50/LQNJ859k018153.gif" /> ${fn:substring(item.name, 0, 6)}</a>
													</c:forEach>
												</c:if>
												<c:if test="${hasZhan}">
													<c:forEach var="item" items="${homeLeftSites}">
														<a class="my-admins-item" title="${item.name}" href="http://zhan.renren.com/zhan/${item.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/icons/zhan.png" /> ${fn:substring(item.name, 0, 6)}</a>
													</c:forEach>
												</c:if>
												<c:if test="${hasLoverSpace}">
													<c:forEach var="item" items="${manageLoverPageList}">
														<a class="my-admins-item" title="${item.name}" href="${applicationScope.urlLover}/${item.id}" target="_blank"><img class="icon" src="${applicationScope.urlStatic}/imgpro/page/love/lover-heart.png" /> ${fn:substring(item.name, 0, 6)}</a>
													</c:forEach>
												</c:if>
											</div>
											</c:if>
										</c:if>
									</div>
									<%--应用的下拉框--%>
									<c:if test="${!host.page}">
										<div class="nav-app-drop-menu nav-app-drop-menu-a" id="appsMenuWrap"></div>
									</c:if>
								</c:if>
							</div>
									
						</div>
						
					</div>
				</div>
			</div>
			<c:if test="${applicationScope.domain == 'renren.com' and channel=='首页'}">
				<c:if test="${!setGray && visiter.userStateInfo.guideDone}">
					<a stats="home_dressup_new" href="#nogo" onclick="switchSkin('fr=1');return false;" class="dressup-home-btn <c:if test="${!empty homeTipCode}">has-dressedup</c:if>" id="dressupHomeBtn">首页换肤</a>			
				</c:if>
			</c:if>
						
			<div id="container-for-buddylist"><c:if test="${!NO_EXTRA_DIV}"><div id="container"></c:if><%--匹配在footer.inc中--%>
				<div id="topbar"><c:if test="${emailIsConfirmed==false && channel=='首页'}">
						<div id="account-vertify-topbar" style="background:#FFFBC1;border:1px solid #F9B967;border-top:0 none;padding:5px;text-align:center;">
							<img src="${applicationScope.urlSmallStatic}/imgpro/icons/activemail.png" style="vertical-align:middle" />
							您的帐号还没有通过认证，您将无法使用网站的部分功能，
							<a href="#" onclick="showConfirmDialog();return false;" style="zoom:1;vertical-align:baseline" >马上去认证您的帐号&raquo;</a>
						</div>
					</c:if></div>
				<c:if test="${INC_PUBLISHER_IN_HEADER}">
					<%@ include file="/inc/n/site-header.inc" %>
				</c:if>
		</c:if>