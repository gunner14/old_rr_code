<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
	
	<div id="navBar" class="menu-bar ${domainIdentified['siteNameAbbr']}">
		<div class="navigation-wrapper">
			<div class="navigation clearfix">
				<div class="blank-holder"></div>
				<logo:logotag mode="trunk"/>
				<div class="nav-body clearfix">
					<div class="nav-other">
						<div class="menu">
							<div class="menu-title"><a href="${applicationScope.urlReg}/register.do">注册</a></div>
						</div>
						<div class="menu">
							<div class="menu-title"><a href="${applicationScope.urlLogin}/GLogin.do">请登录</a></div>
						</div>
						<div class="menu">
							<div class="menu-title"><a href="#nogo" onclick="XN.Browser.addHomePage('${applicationScope.urlWww}');return false;">设为首页</a></div>
						</div>
						<div class="menu">
							<div class="menu-title"><a href="#nogo" onclick="XN.Browser.addBookMark('${applicationScope.urlWww}');return false;">加入收藏</a></div>
						</div>
						<div class="menu last">
							<div class="menu-title"><a href="${applicationScope.urlMain}/info/Help.do">帮助</a></div>
						</div>
					</div>
				</div>
			</div>
			<div class="nav-shadow"></div>
		</div>
	</div>
