<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta name="Description" content="开心是一个真实的社交网络，联络你和你周围的朋友。加入开心网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="kaixin,开心,开心网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/favicon.ico" rel="shortcut icon" type="image/x-icon" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/apps/home.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlStatic}/kaixin/csspro/module/navigationpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlStatic}/kaixin/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlStatic}/kaixin/csspro/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<title>开心网</title>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/js/yui/yahoo-dom-event/yahoo-dom-event.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/js/yui/autocomplete/autocomplete-min.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/js/fillinfo.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/js/inCityArray.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/renren.reg.js?ver=$revxxx$"></script>
	<script language="javascript">
	function refreshCode() {
		$('verifyPic').src = $('verifyPic').src + Math.random();
	}
	</script>
	<style type="text/css">
#navigation {background:transparent url(${applicationScope.urlStatic}/kaixin/imgpro/bg/v2_head_bg.png) repeat-x scroll left top;width:841px;height:37px;}
#navigation .nav-right {background:transparent url(${applicationScope.urlStatic}/kaixin/imgpro/bg/v2_head_bg_r.png) no-repeat scroll right top;padding-right:20px;margin-right:0;padding-top:3px;}
#navigation .nav-right li a{padding-bottom:12px;*padding-bottom:13px;text-decoration:none;color:#FEF6E6;}
#syshome.login #navigation{width:760px;}
*+html #navigation .nav-right{padding-top:14px;}
*+html #navigation .nav-right{height:31px;overflow:hidden;}
*+html #navigation .nav-right li a{padding-bottom:11px;}
</style>
</head>
<body>
<div id="syshome" class="login">
	<%@ include file = "inc/reg_navigate_bar.inc" %>
	<!--head END-->
	<div class="mainbox">	
    <div id="sidebar">
		<div class="cornerlt"></div>
		<div class="cornerlb"></div>
		<div class="cornerrt"></div>
		<div class="cornerrb"></div>
    	<div id="loginsidebar">
				<div id="errorMessage" class="errors_div" style="display:none;"></div>
				<form method="post" id="loginForm" action="${applicationScope.urlPassport}/PLogin.do" >
					<p>
						<label>Email:</label>
						<input type="text" name="email" rule="blank#loginName" class="input-text" value="${_de}" id="email" tabindex="1" />
					</p>
					<p>
						<label>密码:</label>
						<input type="password" id="password" name="password" rule="blank" error="请输入密码" class="input-text" tabindex="2"  />
						<input type="hidden" id="login_type" name="login_type"  value="1004" />
					</p>
					<p class="autolog">
						<input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="3" /><label for="autologin" class="font-normal" id="autologlabel">记住登录状态</label>
                        <a href="${applicationScope.urlMain}/GetPasswd.do" class="findpwd">找回密码</a>
					</p>
					<p class="logbtn">
						<input type="hidden" name="origURL" value="http://home.kaixin.com/Home.do" />
						<button tabindex="4" id="login" class="inputsubmit" type="submit">登录</button>
					</p>
				</form>
			</div>
    </div><!--sidebar END-->

    <div id="widebar">
        <!--headpro.inc end-->
       	<div class="content">
       		
            <div class="single-column"> 
				<div id="content"> 
					<div class="graybox"> 
						<div class="boxbody"> 
							<p>开心网目前还处于测试期，暂不开放注册。</p> 
							<p>如果您想加入开心网，请您向已经在开心网注册的同事、朋友索取邀请链接，然后从该链接注册。</p> 
							<p>谢谢您的支持！</p> 
							<a href="${applicationScope.urlMain}" class="return">返回</a> 
						</div> 
					</div> 
				</div> 
				<!--content END--> 
			</div> 
            
       	</div><!--content END-->
         
    </div><!--widebar end-->

</div><!--container END-->
</div>
<div id="footer">
		<div class="copyright">
			<%-- 
			<span class="float-right">
				<a href="${applicationScope.urlMain}/info/About.do">关于</a><span class="pipe">|</span><a href="${applicationScope.urlSupport}/GetGuestbookList.do">客服</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/PrivacyClaim.do">隐私声明</a>
			</span>
			--%>
			<span>开心网 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2010</span>
		</div>
</div><!--footer END-->
</body>
</html>