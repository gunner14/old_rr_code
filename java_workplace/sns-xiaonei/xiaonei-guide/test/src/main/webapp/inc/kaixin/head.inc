<%@ page pageEncoding="UTF-8" %>
<%-- make Struts XHTML-compliant --%>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
	<meta name="Description" content="开心是一个真实的社交网络，联络你和你周围的朋友。加入开心网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,开心,开心网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园" />
	<link href="${applicationScope.urlSmallStatic}/kaixin/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlSmallStatic}/kaixin/csspro/module/navigationpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/favicon.ico" rel="shortcut icon" type="image/x-icon" />
	<link href="${applicationScope.urlSmallStatic}/kaixin/csspro/module/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css">
	<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlSmallStatic}/kaixin/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlSmallStatic}/kaixin/csspro/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/kaixin/js/prototype.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/kaixin/js/compact.js?ver=$revxxx$"></script>	
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/kaixin/jspro/xn.app.status.js?ver=$revxxx$"></script>
	<!--[if lte IE 6]><script type="text/javascript" src="${applicationScope.urlSmallStatic}/kaixin/jspro/patch/ie6.js?ver=$revxxx$"></script><![endif]-->
    <%-- 用于动态加载的伪标签,由于新的加好友策略,很多页面会用到推荐好友--%>
    <script type="text/javascript">
    XN.getFileVersion(
    [
        '${applicationScope.urlSmallStatic}/kaixin/csspro/apps/profile.css?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/kaixin/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/kaixin/jspro/xn.ui.pager.js?ver=$revxxx$',
		'${applicationScope.urlSmallStatic}/kaixin/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/kaixin/csspro/module/friendSelector.css?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/kaixin/jspro/xn.app.status.js?ver=$revxxx$'
    ]);    
    </script>