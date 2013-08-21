<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript">
var is_get=0;
	function get_network_list(obj,l,mm,e){
		if(is_get==1)
			return optional_drop_down_menu(obj,l,mm,e);
		var url  = "/getnetworkajax.do";
	  var myAjax = new Ajax.Request( url, {
		    method: 'get',
		    onComplete: get_ret,
		    onFailure:get_err});
		return optional_drop_down_menu(obj,l,mm,e);
	}
	function get_ret(r){
		var allnets;
		eval("allnets="+r.responseText);
		networks=allnets.networks;
		var top_menu=ge('network_menu');
		
		for(i=0;i<networks.length;i++){
			li=document.createElement('li');
			//取消网络
	//		a=document.createElement('a');
	//		a.innerHTML=networks[i].name;
	//		a.href="http://network.${applicationScope.domain}/go.do?nk="+networks[i].id;
	//		li.appendChild(a);
			//直接用名字
			li.innerHTML=networks[i].name;
			
			top_menu.parentNode.insertBefore(li,top_menu);
			top_menu=li;
		}
		
		is_get=1;
		return optional_drop_down_menu(obj,l,mm,e);
	}
	
	function get_err(r){
		li=document.createElement('li');
		li.innerHTML="不能获取网络列表";
		var top_menu=ge('network_menu');
		top_menu.parentNode.insertBefore(li,top_menu);
		is_get=0;
	}
	
</script>


<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<div id="sidebar">
<c:choose>	
	<c:when test="${empty requestScope.host}">
		<div id="loginpanel">
			<h3>用户登录:</h3>
<%
String loginEmail = "";
try {
	Cookie cookies[]=request.getCookies();
	if(cookies != null) {
		for(int i=0;i<cookies.length;i++){
			if(cookies[i].getName().equals("wh_email")) {
				loginEmail = cookies[i].getValue();
			}
		}
	}
} catch(Exception e) {

}
%>
			<form method="post" id="loginForm"  action="${applicationScope.urlLogin}/Login.do" onsubmit="getEl('email').value=Trim(getEl('email').value);" focus="email">
				<p class="top">
					<label for="email">登录帐号:</label>
					<input type="text" name="email" class="inputtext" id="email" value="<%=loginEmail%>" tabindex="1" />
				</p>
				<p>
					<label for="password">密码:</label>
					<input type="password" id="password" name="password" class="inputtext" tabindex="2" />
				</p>
				<p>
					<label for="autoLogin" class="labelCheckbox"><input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="3" />下次自动登录</label>
				</p>
				<p>
				  <input type="hidden" name="origURL" value="${requestScope.origURL}" />
					<input type="submit" id="login" name="submit" class="subbutton" value="登录" tabindex="4" />
					<a href="${applicationScope.urlMain}/GetPasswd.do">找回密码</a>
				</p>
			</form>
			<a href="${applicationScope.urlReg}/register.do" class="portal"><strong>注册</strong>${domainIdentified["siteName"]}期待你的加入</a>
			<a href="${applicationScope.urlMain}/info/Demo.do" class="portal"><strong>页面介绍</strong>先看看你的${domainIdentified["siteName"]}主页</a>
			<dl id="welcome">
				<dt>在${domainIdentified["siteName"]}你可以:</dt>
				<dd>找到久违的老同学</dd>
				<dd>了解你周围朋友的最新动态</dd>
				<dd>用照片、日志记录你的生活，和朋友们一起分享</dd>
				<dd>和朋友们分享喜欢的音乐、电影、书籍</dd>
			</dl>
		</div>
	</c:when>
	<c:otherwise>
		<div id="my-search">
			<div id="my-search-menu">
				<h2><a class="" href="${applicationScope.urlBrowse}/os.do" id="global_search_link">搜索</a></h2>
				<a href="#nogo" onfocus="this.blur()" onclick="return optional_drop_down_menu(this, ge('global_search_link'), ge('search_options_menu'), event);" class="search_arrow"><span></span></a>
				<ul id="search_options_menu" style="display:none;">
					<li><a href="${applicationScope.urlBrowse}/os.do">基本搜索</a></li>
					<li><a href="${applicationScope.urlBrowse}/ns.do?action=classmates">搜索同学</a></li>
					<li><a href="${applicationScope.urlBrowse}/ns.do?action=coworkers">搜索同事</a></li>	
					<li><a href="${applicationScope.urlBrowse}/advanced.do">高级搜索</a></li>
					<li><a href="${applicationScope.urlBrowse}/b.do">随便看看</a></li>
				</ul>
			</div>
			<div id="my-search-input">
			<form action="${applicationScope.urlBrowse}/os.do?from=opensearch" method="get" onsubmit="if($('openSearch').value==''){alert('请输入查询条件');return false;}">
				<input type="text" name="q" id="openSearch" class="inputtext" onfocus="document.getElementById('mySearchGo').className='my-search-go-focus';document.getElementById('openSearch').value='';" onblur="document.getElementById('mySearchGo').className='my-search-go'" value="找人"/>							
				<input type="submit" class="my-search-go" value="" id="mySearchGo" onclick="this.className='my-search-go-focus'" onblur="this.className='my-search-go'" />
			</form>
			</div>
		</div>
		
		<div id="sidebar_content">
			<c:if test="${editmenu}"><div class="rearrange_message">拖拽改变菜单排序</div></c:if>
			<div class="app_list">
			<div id="app_list">
				<c:forEach var="item" items="${requestScope.userLeftSideMenus.displayMenus}">
					<div class="list_item">
						<div class="container" id="${item.appId}">
						<c:if test="${editmenu}"><a onmousedown="new moveable_app(this.parentNode, null);" class="handle" href="#"/></c:if><a class="link_title" onmousedown="new track_moveable(this.parentNode, this);" href="${item.sideNavUrl}" style="background-image:url(${item.appIcon})">${item.appName}</a></div></div>
				</c:forEach>
			</div>
			</div>
			<c:choose><c:when test="${editmenu}">
			<div class="app_list"></c:when>
			<c:otherwise>
			<div id="expandable_more" class="app_list" style="display:none;"></c:otherwise></c:choose>
			<div class="divider_bar">&nbsp;</div>
			<div id="app_non_nav_list">
				<c:forEach var="item" items="${requestScope.userLeftSideMenus.moreMenus}">
					<div class="list_item">
						<div class="container" id="${item.appId}">
						<c:if test="${editmenu}"><a onmousedown="new moveable_app(this.parentNode, null);" class="handle" href="#"/></c:if><a class="link_title" onmousedown="new track_moveable(this.parentNode, this);" href="${item.sideNavUrl}" style="background-image:url(${item.appIcon})">${item.appName}</a></div></div>
				</c:forEach>
			</div>
			<c:if test="${requestScope.uc.moreMenuList ==''}"><div class="rearrange_message">将上方菜单项拖入即可自动隐藏</div></c:if>
			</div>
			<div class="more_section" id="more_section"><c:if test="${!editmenu}"><a class="edit" href="${applicationScope.urlApp}/apps/editapps.do">编辑</a><a href="#" id="more_link" class="expand_link more_apps" onclick="toggle_more_list();  return false;">更多</a></c:if></div>
		</div>
	</c:otherwise>
</c:choose>
</div>


<div id="widebar">
<c:if test="${empty cleanpage}">
<c:if test="${visiter.stage == 10}">
    <%@ include file="/inc/promo-wide-highschool.inc" %>
</c:if>
<c:if test="${requestScope.visiter.loginCount > 6 && visiter.stage == 20}">
    <%@ include file="/inc/promo-wide.inc" %>
</c:if>
<c:if test="${visiter.stage == 30}">
    <%@ include file="/inc/promo-wide-workplace.inc" %>
</c:if>
</c:if>
<c:if test="${channel!='space'}"></c:if>
<c:choose>
	<c:when test="${empty requestScope.host}">
		<script type="text/javascript">
			function addBookmark(title,url) {
				if( document.all ) {
					window.external.AddFavorite( url, title);
				} else if (window.sidebar) {
					window.sidebar.addPanel(title, url,"");
				} else if( window.opera && window.print ) {
					return true;
				}
			}
		</script>
		<div id="utility"><a href="#" onclick="this.style.behavior='url(#default#homepage)';this.setHomePage('${applicationScope.urlProfile}');">设为首页</a><span class="pipe">|</span><a href="javascript:addBookmark('${domainIdentified["siteName"]} - 因为真实，所以精彩','${applicationScope.urlWww}/');">加入收藏</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a></div><div id="navigation"></div>
	</c:when>
	<c:otherwise>
		<div id="navigation">
			<ul class="nav-left">
				<li><a href="${applicationScope.urlHome}/Home.do">首页</a></li>
				<li><a href="${applicationScope.urlMain}/profile.do?id=${requestScope.hostid}">个人主页</a><a class="edit_link" href="${applicationScope.urlWww}/BasicInfo.do">修改</a></li>
				<li><a href="${applicationScope.urlFriend}/myfriendlistx.do">好友</a></li>
				<c:if test="${requestScope.visiter.stage==10}">
					<li><a href="${applicationScope.urlBrowse}/b.do">随便看看</a></li>
				</c:if>
				<li><a href="${applicationScope.urlBrowse}/os.do">搜索</a></li>
				<li><a href="${applicationScope.urlClass}/">班级</a></li>
				<li>
					<div class="with_arrow">
						网络<a onfocus="this.blur()" onclick="return get_network_list(this, ge('global_networks_link'), ge('global_networks_menu'), event);"  class="global_menu_arrow" href="#"><img align="top" src="${applicationScope.urlSmallStatic}/img/global_menu_space.gif" /></a>
					</div>
					<div style="display:none;" class="navigator_menu" id="global_networks_menu">
						<ul>
							<!--<li><a href="http://network.${applicationScope.domain}/go.do?nk=${visiter.univ}">${visiter.univName}</a></li>-->
							<!--
							<li id="network_menu" class="menu_divider"> </li>
							<li><a href="${applicationScope.urlBlog}/freshblog.do">最新日志</a></li>
							<li><a href="${applicationScope.urlPhoto}/freshalbum.do">最新相册</a></li>
							<c:if test="${requestScope.visiter.stage==20}">
							<li><a href="${applicationScope.urlEvent}/GetEventList.do">最新活动</a></li>
							</c:if>							
							-->
							<li id="network_menu" class="menu_divider"> </li>
							<li ><a href="${applicationScope.urlMain}/regionall.do">浏览所有网络</a></li>
							<li><a href="${applicationScope.urlMain}/showMyNetwork.do">管理我的网络</a></li>
						</ul>
					</div>
				</li>
				<li><a href="${applicationScope.urlMsg}/GetMessageInList.do">站内信</a><c:if test="${requestScope.visiter.newMessageCount>0}"><a class="edit_link new-inbox" href="${applicationScope.urlMsg}/GetMessageInList.do">(${requestScope.visiter.newMessageCount})</a></c:if></li>
			</ul>
			<ul class="nav-right">
<c:if test="${!empty showImLeaveword}">
<%
try {
	com.xiaonei.platform.core.model.User user = (com.xiaonei.platform.core.model.User)request.getAttribute("host");
	int count = mop.hi.oce.adapter.AdapterFactory.getCountCacheAdapter().getImLeavewordCount(user.getId());
	if(count > 0) {
		out.println("<li class=\"paopao\"><a href=\"#nogo\" onclick=\"javascript:talkto(event, " + user.getId() + ", 0, '" + user.getEmail() + "', '')\"><span>(" + count + ")</span></a></li>");
	}
} catch(Exception e) {
	e.printStackTrace();
}
%>
</c:if>
				<li><a href="${applicationScope.urlWww}/personallink.do">帐号</a></li>
				<li><a href="${applicationScope.urlWww}/privacyhome.do">隐私</a></li>
				<li><a href="${applicationScope.urlWww}/Logout.do">退出</a></li>
			</ul>
		</div>
	</c:otherwise>
</c:choose>

