<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn" %>
<%@ page import="com.xiaonei.platform.component.tools.UserUtil" %>
<%
com.xiaonei.platform.core.model.User user = null;
%>
<%
try {
	user = (com.xiaonei.platform.core.model.User)request.getAttribute("host");	
} catch(Exception e) {
}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>	    
	  	<link href="${applicationScope.urlSmallStatic}/csspro/apps/home-all.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.1.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
        <link href="${applicationScope.urlSmallStatic}/csspro/module/news-feeds.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
        <link href="${applicationScope.urlSmallStatic}/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
		<c:if test="${not empty requestScope.host}">
			<script type="text/javascript">
					var wpiMenuInfo = {recentMenu : [<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" varStatus="status" begin="0" end="8">{"id":"${item.appId}", "name":"${item.appName}", "icon":"${item.appIcon}", "href":"${item.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"}${!status.last ? ',' : ''}</c:forEach>],
						favoriteMenu : [<c:forEach var="item" items="${requestScope.userMenu.favoriteMenu}" varStatus="status">{"id":"${item.appId}", "name":"${item.appName}", "icon":"${item.appIcon}", "href":"${item.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"}${!status.last ? ',' : ''}</c:forEach>],
						user : {"id" : ${requestScope.host.id}},
						currentApp : [<c:if test="${!empty requestScope.currentApp}"> {"id":"${requestScope.currentApp.appId}","name":"${requestScope.currentApp.appName}", "icon":"${requestScope.currentApp.appIcon}","href":"${requestScope.currentApp.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"} </c:if>]
					};
			</script>
        </c:if>        
        <style type="text/css">		
			#guidebar {
				background: #dee8f8;
				padding: 10px;
				margin: 10px 0;
			}
			#guidebar #ifocus {
				position: relative;
				height: 380px;
				overflow: hidden;
			}
			#guidebar #ifocus_pic {
				position: relative;
				width: 670px;
				float: left;
				height: 380px;
				overflow: hidden;
			}
			#guidebar #ifocus_piclist {
				z-index: 1;
				position: absolute
			}
			#guidebar #ifocus_piclist li {
				width: 670px;
				height: 380px;
				overflow: hidden;
				position:relative;								
			}
			#ifocus_piclist li a{display:block;width:670px;height:380px;background:#000;position:absolute;top:0;left:0;z-index:999;filter: alpha(opacity=0);opacity:0;}
			#guidebar #ifocus_btn {
				float: right;
			}
			#guidebar #ifocus_btn li {
				height: 120px;
				overflow: hidden;
				margin-bottom: 7px;								
				width: 260px;
				position: relative;
				cursor: pointer;
				padding: 1px;
				background: #dee8f8;
			}
			#guidebar #ifocus_btn li span.mask-bg {
				height: 122px;
				overflow: hidden;
				width: 262px;
				position: absolute;
				background: #112233;
				display: block;
				top: 0;
				left: 0;
				z-index: 1;
				opacity: 0.5;
				filter: alpha(opacity=50);
			}
			#guidebar #ifocus_btn .current span.mask-bg {
				display: none;
			}
			#guidebar #ifocus_btn img {
				display: block;
				width: 260px;
				height: 120px;
			}
			#guidebar #ifocus_btn .current span.mask-icon {
				background: url(http://s.xnimg.cn/imgpro/guide/mask.png) no-repeat !important;
				_background: none;
				_filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='http://s.xnimg.cn/imgpro/guide/mask.png');
				position: absolute;
				display: block;
				width: 10px;
				height: 20px;
				top: 50%;
				left: 0;
				margin-top: -10px;
				z-index: 2;
			}
			#guidebar #ifocus_btn .current {
				background: #336699;
			}
			#ifocus_piclist object{display:block;width:670px;height:380px;overflow:hidden;}

		</style>
        <c:set var="INC_COMMON_CSS" value="true" />
        <%@ include file="/inc/headpro.inc"%>	  	
	  	<title>${domainIdentified["siteName_fillinfo"]} - ${requestScope.host.name}</title>
	</head>
<body id="dashboardPage" class="home">
		<c:set var="cleanpage" value="ture" /> 		
		<%@ include	file="/inc/header-guide-flow.inc"%>
		<%@ include	file="/inc/wrapper.inc"%>
		<div style="width: 960px;margin: 0 auto;overflow: hidden;">
			<h2>请选择你最感兴趣的游戏开始玩：</h2>
			<div class="slide">
				<div id="guidebar" class="clearfix">
					<div id="ifocus" >
						<div id="ifocus_pic">
							<div style="" id="ifocus_piclist">
								<ul>
									<li style="display:none"><a href="http://www.renren.com" target="_blank" style="display:block"></a><div></div></li>
									
									<li style="display:none"><a href="http://www.renren.com" target="_blank" style="display:block"></a><div></div></li>
									
									<li style="display:none"><a href="http://www.renren.com" target="_blank" style="display:block"></a><div></div></li>
								</ul>
							</div>
						</div>
						<div id="ifocus_btn">
							<ul>
								<li class="normal"><img src="" alt="" /><span class="mask-icon"></span><span class="mask-bg"></span>
								</li>
								<li class="normal"><img src="" alt="" /><span class="mask-icon"></span><span class="mask-bg"></span>
								</li>
								<li class="normal"><img src="" alt="" /><span class="mask-icon"></span><span class="mask-bg"></span>
								</li>
							</ul>
						</div>
					</div>
				</div>
				<p>
					<span style="float:left;padding-left:20px;">现在有 <strong>4420392</strong> 用户正在人人网玩上面3个游戏哦</span><a href="/guide.do?ref=guide_later_play">以后再玩>></a>
				</p>
			</div>
		</div>
		<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/slider.js?ver=$revxxx$"></script>
		<c:set var="noWebPager" value="true" />		
		<%@ include file="/inc/footer-noad.inc"%>	
	</body>
					
</html>
