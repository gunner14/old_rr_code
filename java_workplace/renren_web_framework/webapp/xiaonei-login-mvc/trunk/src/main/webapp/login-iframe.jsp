<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<link href="${applicationScope.urlSmallStatic}/n/apps/login/compatible/login-box-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<script src="${applicationScope.urlSmallStatic}/n/core/base-all.js?ver=$revxxx$" type="text/javascript"></script>
<script type="text/javascript">
XN.config.parentDomain = true;
</script>
<script src="${applicationScope.urlSmallStatic}/n/apps/login/login-box.js?ver=$revxxx$" type="text/javascript" ></script>
</head>
<body>
<iframe name="iframeParent" id="iframeParent" width="0" height="0" frameborder="0" allowtransparency="true" scrolling="no" style="display:none;"></iframe>
<div id="yellow_error" style="display:none;" class="yellow-error">
	<a href="javascript:closeError();" class="close"></a>
	<p class="wrong">您的用户名和密码不匹配</p>
	<p class="worp">为了帐号安全，已向您的邮箱： <strong id="sendemail"></strong>发送了一封确认信，请通过邮件内链接登录。</p>
	<p class="m-26"><a target="_blank" href="#" id="gotoEmail">打开邮箱查收确认信</a></p>
	<p class="m-26"><a href="javascript:closeError();">重新输入</a></p>
</div>
<div id="account_stop" style="display:none;" class="yellow-error">
	<a href="javascript:closeStop();" class="close"></a>
	<p class="wrong"></p>
	<p class="center">您的帐号已停止使用，如有疑问请联系<a style="background:none;padding-left:0;" target="_blank" href="http://support.renren.com/helpcenter">客服</a></p>
</div>
<div id="account_lock" style="display:none;" class="yellow-error">
	<a href="javascript:closeLock();" class="close"></a>
	<p class="wrong">您的帐号由于以下某种原因需要解锁才能登录</p>
	<ol>
		<li>删除过帐号</li>
		<li>长时间没有登录网站</li>
		<li>安全原因</li>
	</ol>
	<p class="center"><a href="http://safe.renren.com/relive.do" target="_blank">立即解锁</a></p>
</div>
<form method="post" id="loginForm" class="login-form" action="login-iframe.jsp">
	<dl class="top clearfix">
		<dt><label for="email">帐号:</label></dt>
		<dd>
			<span id="errorMessage" class="errors_div" style="display:none;"></span>
			<c:set var="email" value="${_de}" />
			<input type="text" name="email" class="input-text" id="email" tabindex="1" value="${_email}"/>
		</dd>
	</dl>
	<dl class="pwd clearfix">
		<dt><label for="password">密码:</label></dt>
		<dd>
			<input type="password" id="password" name="password" error="请输入密码" class="input-text" tabindex="2" />
			<a class="forgetPwd" id="forgetPwd" href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword" target="_blank">忘记密码？</a>
		</dd>
	</dl>
	<div class="caps-lock-tips" id="capsLockMessage" style="display:none"></div>
	<dl id="code" class="code clearfix">
		<dt><label for="code">验证码:</label></dt>
		<dd>
			<input type="text" name="icode" id="icode_login" class="input-text" tabindex="3" autocomplete="off" />
		</dd>	
	</dl>
	<dl id="codeimg" class="codeimg clearfix">
		<dt></dt>
		<dd>
			<img id="verifyPic_login" src="http://icode.renren.com/getcode.do?t=login&rnd=<%= new java.util.Random().nextInt()%>"/>
			<a href="javascript:refreshCode_login();">换一张</a>
		</dd>	
	</dl>
	<dl class="savepassword clearfix">
		<dt>
			<label title="为了确保您的信息安全，请不要在网吧或者公共机房勾选此项！" for="autoLogin" class="labelCheckbox">
				<input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="4" />下次自动登录
			</label>
		</dt>
		<dd>
			<span class="getpassword" id="getpassword"><a href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword" target="_blank">忘记密码？</a></span>
		</dd>
	</dl>
	<dl class="bottom">
		<input type="hidden" name="origURL" value="<%=com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent(request.getParameter("origURL"))%>" />
		<input type="hidden" name="domain" value="${applicationScope.sysdomain}" />
		<input type="hidden" name="key_id" value="1" />
		<input type="hidden" name="captcha_type" id="captcha_type" value="login" />
		<input type="hidden" name="https"  value="true" />
		<input type="submit" id="login" class="input-submit login-btn" value="登录人人网" tabindex="5"/>
	</dl>
</form>
</body>
</html>