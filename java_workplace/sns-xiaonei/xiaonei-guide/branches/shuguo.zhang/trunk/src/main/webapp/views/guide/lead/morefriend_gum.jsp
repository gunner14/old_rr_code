<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.renren.sns.lead.dataproviders.util.StatLogger"%>
<%@page import="com.xiaonei.platform.core.model.User"%><html xmlns="http://www.w3.org/1999/xhtml">
<%
try{
	Object cnto = request.getAttribute("lead_preview_friend_count");
	if(cnto == null || (cnto!=null && (""+cnto).equals("0"))){
		RequestDispatcher disp = request.getRequestDispatcher("/morefriend?skip=1");
		disp.forward(request,response);
	}
	String user_type = (String)request.getAttribute("lead_user_type");
	User host = (User)request.getAttribute("visiter");
	StatLogger.info(host.getId(), ""+"type"+":"+user_type+"",StatLogger.S_ARV_ENTERINTOO_MF_,(String)request.getAttribute("lead_user_type_string"));
}
catch(Throwable e){
	
}

%>
<head><!-- _mf_gum_ -->
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,人人,人人网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
	<%@ include file="/inc/head-guide-flow.inc" %>
	<title>${domainIdentified["siteName_fillinfo"]} - 找到朋友</title><link href="${applicationScope.urlSmallStatic}/favicon-rr.ico" rel="shortcut icon" type="image/x-icon" />
	<script>var guideNextStep = 'morefriend?new=1&skip=1';</script>
	<link href="${applicationScope.urlSmallStatic}/renren_logo_apple.png" rel="apple-touch-icon" />
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/imgpro/msntab/scripts/swfobject.js?ver=$revxxx$"></script>

	<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.guide.darkcoral.js?ver=$revxxx$"></script>
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
</head>
<body>
	<div class="main_wrap clearfix">
		<div class="reg_logo">10秒找到你所有朋友</div>
		<div class="guide-main-new clearfix">
			<span class="login"><a href="http://www.renren.com/Logout.do" >退出</a></span>
			<div class="steps second clearfix">
				<p>1.填写个人信息</p>

				<p class="online">2.找到朋友，完成</p>
			</div>
			<script type="text/javascript">
				var Guide_reg_stage = '${host.stage}';
				var Guide_added_friend = ${lead_preview_confirmlist};
				var Guide_recommended_friend = ${lead_preview_friend };
				var Guide_recommended_count = ${lead_preview_friend_count };
				var guideTrace_pageId = 'p22';
			</script>
			<div id="" class="guide-content">
				<div class="friendslist friends-addbox">
					<ul id="friendList">
						
					</ul>
				</div>
				<div id="" class="side-bigmod">
					<div id="friendPool">
						
					</div>
					<div class="larger center">
						<input type="submit" id="submit" class="input-submit" value="完成 进入首页" id="">
						<a href="morefriend?skip=1" id="skip">跳过</a>
					</div>

				</div>
			</div>
			<div class="b_text">
			</div>
		</div>
	</div>
	<script type="text/javascript">
		MoreFriend.init();
		var page_id = "p22";  //统计用的页面id
	</script>
	<%@ include file="/inc/guide_stat.inc" %>
</body>
<!-- ${lead_preview_flag}  -->
</html>

