<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head.inc" %>
		<link media="all" type="text/css" rel="stylesheet" href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" />
		<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<script type="text/javascript" src="http://s.xnimg.cn/js/register/autoSubmit.js?ver=$revxxx$" />
		
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
		<title>${domainIdentified["siteName_fillinfo"]}</title>
		<script language="javascript">
			function chkForm() {
				if ($("uAccount").value == "") {
					alert("请输入你的帐号");
					return false;
				}
				if ($("emailpwd").value == "") {
					alert("请输入你的密码");
					return false;
				}
				ajaxLog('ii','submit');

				return true;
			}
		</script>
	</head>
	<body id="pageFriends" class="msninvite-bg">
		<c:set var="hideLink" value="yes" />
		<div id="container" >
			<%@ include file="/inc/header-guide-flow.inc" %>
			<div id="opi" class="page-wrapper clearfix">
				<div class="full-page-holder">
					<div class="full-page clearfix guide-msninvite">
						<div class="compatible">
							<div id="content">
								<div class="guide-invite-steps clearfix">
									<ul class="current1">
										<li class="current">
											<h4>第1步:</h4>
											<p>找到我的朋友</p>
										</li>
										<li>
											<h4>第2步:</h4>
											<p>上传头像</p>
										</li>
									</ul>

								</div>		

								<!--begin:msn-->
								<div class="invite_box">

									<h2 class="emailicon">
										通过MSN找到您认识的人<span class="orange">83%</span><span class="normal">的用户用过这种方式找到了已经注册人人网的朋友！</span>
									</h2>
									<%@ include file="/inc/errors.inc" %>
									<form class="autoSubmit" subtn="add_friends" action="${applicationScope.urlInvite}/regGudDpt.do" id="msnsend_form" method="post" name="msnsend">
										<input type="hidden" name="rurl" value="${rurl}"/>
										<input type="hidden" name="ourl" value="${ourl}"/>
										<div class="cube">
											<div class="msn-account-info">
												<p class="email">
													<label>你的MSN:</label>
													<input type="hidden" name="regss" value="shortform" />
													<input type="text" class="inputtext" name="uAccount" id="uAccount" tabindex="1" value="${user_email}"/> 
												</p>
												<p>
													<label>密码:</label>
													<input id="emailpwd" type="password" class="inputtext" name="pwd" tabindex=2 />
												</p>

											</div>		
											<p class="addingfri">
												<input type="button" onclick="chkForm();" value="寻找朋友" class="btn-5" id="add_friends"/>						
											</p>
											<p class="more-action">
										    <a href="?skip=1" class="float-right">跳过</a>
										    <span>人人网不会记录你的密码，请安心使用。</span>
											</p>
										</div>
										<input type=hidden name="importer" value="@hotmail.com" />
									</form>
								</div>
								<!--end:msn-->
							</div>
						</div>
					</div>
				</div>
			</div>
			<c:set var="hideFooterLink" value="true" />
      <c:set var="noWebPager" value="true" />
			<%@ include file="/inc/footer-noad.inc" %>
		</div>
		<script language="javascript">
			var g_loc;
			function ajaxLog(act,loc){
				var myDate = new Date();
				var url = '/AjaxGuideFlowLog.do';
				var pars = 'act='+act;
				g_loc = loc;

				var myAjax = new Ajax.Request(url, {
						method :'post',
						parameters :pars,
						onComplete :function(){
							if(loc != "submit")
								window.location=g_loc;
							else
								$('msnsend_form').submit();
						}
				});
				return true;
			}
		</script>
		<c:if test="${is_f}">
		<script >
			document.getElementById('emailpwd').focus();
		</script>
		</c:if>
	</body>
</html>
