<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
		<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
		<meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
		<meta name="Keywords" content="Xiaonei,人人,人人,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
		<link href="http://s.xnimg.cn/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="${applicationScope.urlSmallStatic}/favicon-${domainIdentified["siteNameAbbr"]}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />
		<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
		<!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/csspro/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
		<link href="http://s.xnimg.cn/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css">
		<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<link href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$" ></script>
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
		<script type="text/javascript">
		XN.getFileVersion(
		[
	        'http://s.xnimg.cn/csspro/apps/profile.css?ver=$revxxx$',
	        'http://s.xnimg.cn/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
	        'http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$',
	        'http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
	        'http://s.xnimg.cn/csspro/module/friendSelector.css?ver=$revxxx$'
		]);

		</script>
		<style type="text/css">
			.friendSelectorBox .selector{ height:362px; overflow:hidden; }
			.friendSelectorBox .selector ul li{ width:148px;}
		</style>
		<title>${domainIdentified["siteName_fillinfo"]} - 寻找朋友</title>

	</head>
	<body>
		<c:set var="hideLink" value="yes" />
		<%@ include file="inc/menu_withif.inc" %>
		<div id="opi" class="page-wrapper clearfix">
			<div class="full-page-holder">
				<div class="full-page clearfix">
					<div class="compatible">
						<div id="content">
						
							<div class="new-guide">
								<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，<%=(String)request.getAttribute("user_name") %></h3>
								<%@ include file="inc/navbar_2preview.inc" %>								
<div class="personal-info">
									<h4>选择你认识的人</h4>
									<p class="description">把你认识的人加为好友，随时获得好友动态。</p>
									<div id="fsb-holder">
									</div>
									<div class="confirm-friends clearfix">
										<span class="float-right">
											<input id="add_friends" type="submit" class="input-submit" value="把选中的人加为好友" />
											<a href="/gdo.do?u=2&skip=user" class="jumpover">跳过</a>
										</span>
									</div>		
								</div>
							</div>
						
						</div>
					</div>
				</div>
			</div>
		</div>

		<c:set var="hideFooterLink" value="true" />
    <c:set var="noWebPager" value="true" />
			<%@ include file="/inc/footer-noad.inc" %>
		</div>
<input type="hidden" id="next_url" value="${next_url}" />
<input type="hidden" id="this_url" value="${this_url}" />
		<!-- 特定于页面的代码 -->
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.selectFriends.js?ver=$revxxx$"></script>
	</body>
</html>
