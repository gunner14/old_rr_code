<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%
	response.sendRedirect("/im-090729.do?skip=1");
	%>
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

<script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.form.validator.js?ver=$revxxx$"></script>

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
<title>${domainIdentified["siteName_fillinfo"]} - 寻找朋友</title>
</head>
<body>
	
	<div id="container">
		<%@ include file="inc/menu_withif.inc" %>
		<div id="opi" class="page-wrapper clearfix">
			<div class="full-page-holder">
				<div class="full-page clearfix">
					<div class="compatible">
						<div id="content">
						
							<div class="new-guide">
								<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，<%=(String)request.getAttribute("user_name") %></h3>
								<%@ include file="inc/navbar_3msn.inc" %>
								<div class="personal-info">
									<h4>快速寻找${domainIdentified["siteName"]}网上的好友</h4>
									<p class="description">你可能有很多朋友已经在这里了，搜索你的MSN或邮箱帐号联系人是最快找到他们的方法。</p>
									<p id="notice" class="show-error" style="display:none">帐号或密码错误，请尝试重新输入。</p>
									<div class="pinfo-form import-msn">
										<p>
											<label class="hidden">邮箱或MSN :</label>
											<input type="radio" id="email" name="emailmsn" checked="true" />
											<label for="email" class="for-radio">Email</label>
											<input type="radio" id="msn" name="emailmsn" />
											<label for="msn" class="for-radio">MSN</label>
										</p>
										<form target="loadingf" class="" method="post" id="webmail" action="${applicationScope.urlInvite}/regGudDpt.do" >
											<input type="hidden" name="rurl" value="${rurl}"/>
											<input type="hidden" name="ourl" value="${ourl}"/>
											<input type="hidden" name="regss" value="shortform" />
											<input type="hidden" name="appId" value=""/>
											<input type="hidden" name="url" value=""/>
											<input type="hidden" name="app_html" value=""/>
											<input type="hidden" name="inv_id" value=""/>
											<input type="hidden" name="header_type" value="${isappguide}"/>
											<p>
												<label for="">Email :</label>
												<input id="email_num" type="text" class="input-text" name="uAccount" rule="blank" />
												<span class="atsign">&#64;</span>
												<select id="email_type" name="importer" class="select-short" rule="blank">
													<option value="">请选择邮箱</option>
													<option value="@163.com">163.com</option> 
													<option value="@126.com">126.com</option>
													<option value="@sina.com">sina.com</option>
													<option value="@hotmail.com">hotmail.com</option>
													<option value="@yahoo.com.cn">yahoo.com.cn</option>
													<option value="@gmail.com">gmail.com</option>
													<option value="@live.cn">live.cn</option>
													<option value="@live.com">live.com</option>
													<option value="@msn.com">msn.com</option>
													<option value="@msn.cn">msn.cn</option>
													<option value="@xnmsn.cn">xnmsn.cn</option>
													<option value="@yahoo.cn">yahoo.cn</option>
													<option value="@yahoo.com">yahoo.com</option>
												</select>
											</p>
											<p>
												<label for="">密码 :</label>
												<input id="email_pwd" type="password" name="pwd" class="input-text" rule="blank" />
												<span class="description">${domainIdentified["siteName"]}网不会记录你的密码，请放心使用。</span>
											</p>
											<p class="form-action">
												<input id="submit_email" class="input-submit" type="button" value="搜索朋友"/>
												<a id="skip_btn_email" href="?skip=1" class="jumpover">跳过</a>
												<img id="load_img_email" class="loadingimg" src="${applicationScope.urlStatic}/img/indicator.gif" />
											</p>
										</form>
										<form target="loadingf" class="autoSubmit" subtn="add_friends" action="${applicationScope.urlInvite}/regGudDpt.do" id="msnsend_form" method="post" name="msnsend" style="display:none">
											<input type="hidden" name="rurl" value="${rurl}"/>
											<input type="hidden" name="ourl" value="${ourl}"/>
											<input type="hidden" name="regss" value="shortform" />
											<input type="hidden" name="importer" value="@hotmail.com" />
											<input type="hidden" name="header_type" value="${isappguide}"/>
										
											<p>
												<label for="">MSN :</label>
												<input id="msn_num" type="text" class="input-text" name="uAccount" tabindex="1" value="${user_email}" rule="blank"/>
											</p>
											<p>
												<label for="">密码 :</label>
												<input id="msn_pwd" type="password" class="input-text" name="pwd" tabindex="2" rule="blank"/>
												<span class="description">${domainIdentified["siteName"]}网不会记录你的密码，请放心使用。</span>
											</p>
											<p class="form-action">
												<input id="submit_msn" class="input-submit" type="button" value="搜索朋友"/>
												<a id="skip_btn_msn" href="?skip=1" class="jumpover">跳过</a>
												<img id="load_img_msn" class="loadingimg" src="${applicationScope.urlStatic}/img/indicator.gif" />
											</p>
										</form>
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

	<iframe id = "loadingf" name="loadingf" src="http://www.renren.com/ajaxProxy.html" width="1" height="1" style="visibility:hidden">
	</iframe>

<!-- 好吧, 脚本就放在这里 -->
<!-- 组件 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>

<!-- 特定于页面的代码 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.importMSNMaill.js?ver=$revxxx$"></script>
</body>
</html>