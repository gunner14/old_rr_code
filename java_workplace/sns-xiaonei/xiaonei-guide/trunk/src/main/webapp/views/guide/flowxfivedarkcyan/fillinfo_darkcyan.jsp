<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType"%><html xmlns="http://www.w3.org/1999/xhtml">

<head><!-- _fi_darkcyan_ -->
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$" media="all" type="text/css" rel="stylesheet" /> 
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-register2.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-modifyinfo-candy.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<!-- <link href="${applicationScope.urlSmallStatic}/csspro/module/modifyinfo_new.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" /> -->
	<title>${domainIdentified["siteName_fillinfo"]} - 完善资料</title>
	<style type="text/css">
		.guide{background:#7F7F7F;}
		.guide #content{background:#7F7F7F url(http://a.xnimg.cn/imgpro/reg/new/gray-bg.png) center top no-repeat}
		.friends-box .pic,
		.friends-box .name,
		.friends-box .online-icon{position:absolute;left:193px;top:37px;}
		.friends-box .online-icon {color: #888;margin-top: 6px;text-align: center;left:250px;top:55px;}
		.friends-box .pic{width:50px;height:50px;overflow:hidden;}
		#footer .copyright{border-top-color:#888888}
		#submit-btn{background:url(http://a.xnimg.cn/imgpro/reg/new/add-done-btn.png) no-repeat;width:178px;height:36px;border:none;text-indent:-9999em}
		.friends-box .name{left:250px;top:37px;}
		.guide .user-data{margin:0 0 0 166px;}.friends-box{height:100px;}
		.modify-popup {left:30px;}
		#footer .copyright {border-top-color:#666666;color:#666;}
	</style>
</head>

<body id="home" class="guide">
		<%@ include file="/inc/header-guide-flow.inc" %>
        <script type="text/javascript">
	    	//var recommandFriends =  XN.json.parse('${requestScope.lead_preview_inviterfriend}');
            <%--XN.json.parse('{"candidate":[{"id":306555611,"netName":"\u5b81\u6ce2\u5927\u5b66","name":"\u738b\u4f73\u4f73","head" :"http:\/\/hdn.xnimg.cn\/photos\/hdn321\/20091223\/1835\/tiny_PvJh_57757p019117.jpg"},{"id":282079587 ,"netName":"\u5317\u4eac\u5e02\u7b2c\u4e8c\u5341\u56db\u4e2d\u5b66","name":"\u5218\u59e5","head":"http :\/\/hdn.xnimg.cn\/photos\/hdn121\/20091201\/1210\/h_tiny_yuHO_7b290000923e2f75.jpg"},{"id":282079695 ,"netName":"\u5357\u5e73\u5e02","name":"\u5f20\u5bb6d","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn321 \/20091201\/1210\/h_tiny_hx03_7b29000092762f75.jpg"},{"id":42033604,"netName":"\u5929\u6d25\u5de5\u5927" ,"name":"\u80e1\u6ce2[Hubot]","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn411\/20090716\/1015\/tiny_Npbe_37364k204237 .jpg"},{"id":282079327,"netName":"","name":"\u674e\u94c1","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn421 \/20091201\/1205\/h_tiny_XruC_7b290000918f2f75.jpg"},{"id":282078819,"netName":"\u6d4e\u5b81\u5e02","name" :"\u674e\u51fa","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091113\/1125\/h_tiny_ZrIA_62f700000fb12f74 .jpg"},{"id":250805736,"netName":"\u5317\u4eac\u5e02","name":"\u6797\u60e0\u534e","head":"http:\/\/hdn .xnimg.cn\/photos\/hdn521\/20091004\/2015\/tiny_SSrQ_99325j019117.jpg"},{"id":282079275,"netName":"California" ,"name":"\u674e\u8087ssss","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091201\/1205\/h_tiny_apxR_7b29000091462f75.jpg"}],"qkey":"friend","groups":["\u5927\u5b66\u540c\u5b66","\u666e\u901a\u670b\u53cb"]}');--%>
        </script>
	<div id="content" style="height:600px">
		<div id="" class="friends-box" style="display:none">
		<%
		%>
	    	<div class="pic"><img src="${visiter.tinyFullUrl }" alt="${visiter.name }" /></div>
		    <span class="name">${visiter.name }</span>
		    <span class="online-icon"><img src="http://a.xnimg.cn/imgpro/icons/online_1.gif" alt="在线" /> 当前在线</span>
		</div>
		<div style="display:none">
		<form id="upload_form"></form>
		</div>
	    <div class="user-data clearfix" style="display:none">
	        <div class="title titleimg">
	            <h1><strong>点击添加你们的共同好友</strong></h1>
	        </div><!--title end-->
	        <div id="friendSelector-container" class="friendSelector-box friendSelector-main"> </div>
	        <div class="btn-box">
	        <form action="/pvbrown-s?from=sg_reg_all" method="post" id="postFriendsForm">
	            <input type="hidden" value="" name="friends"/>
	            <input type="hidden" value="" name="fcount"/>
	            <input type="button" class="input-submit" value="完成 进入首页" id="submit-btn"/>
	            <a href="?new=1&skip=1" id="submit-skip" class="skip">跳过</a>
	        </form>
	        <div id="showScroll"></div>
	        </div>
	    </div><!--user-data end-->
	</div><!--content end-->
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.currentPlace.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolpopdatum.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.pager.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.ui.infoModify.candy.js?ver=$revxxx$" ></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.darkcyan.js?ver=$revxxx$" ></script>

<script type="text/javascript">
	window.userGender = ("${visiter.gender}"=="女生"?"female":"male");
	window.userStage = ${visiter.stage};
	var wpiMenuInfo = {user:{id:81120}};
	    XN.dom.ready(function(){
	//newModifyInfo();
	});
</script>
<%@ include file="/inc/footer-guide-flow.inc" %>
<script type="text/javascript">
	var page_id = "p41";
	XN.dom.ready(function(){
		XN.event.addEvent('submit-skip', 'click', function(){
			sendGuideTrace('ga2'); //
		});
	});
</script>
<%@ include file="/inc/guide_stat.inc" %>
</body>
</html>
