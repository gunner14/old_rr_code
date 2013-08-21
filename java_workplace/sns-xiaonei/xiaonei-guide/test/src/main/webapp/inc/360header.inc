<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<div class="rr-nav">
	<c:choose>
		<c:when test="${empty visiter}">
			<div class="btnbox">
				<a href="#nogo" class="btn" onclick="showPopup()">登录</a>
				<a href="http://reg.renren.com/xn6212.do?ss=17043&rt=1&g=${origin}" class="btn" target="_blank">注册</a>
			</div>
		</c:when>
		<c:otherwise>
			<div class="btnbox">
				<a href="http://www.renren.com/profile.do?id=${visiter.id}" target="_blank">${visiter.name}</a>
				<a href="${applicationScope.urlLogin}/Logout.do?origURL=${gongcheng}" class="btn">退出</a>
			</div>
		</c:otherwise>
	</c:choose>
	<img class="rrlogo" src="http://a.xnimg.cn/actimg/360/img/rr-logo.png" alt="人人网" width="84" height="22" />
</div>

<div id="loginPopup" class="login-mod" style="display:none">
	<div class="popup-head">
		<a href="#nogo" class="popup-close" onclick="closePopup()">关闭</a>
		登录人人网
	</div>
	<div class="login-rr">	
		<form action="http://www.renren.com/RL.do" class="loginrr-form" id="loginForm" method="post">
			<p>请使用你的人人帐号登录</p>
			<p class="top clearfix">
				<label for="email">用户名:</label>
				<input type="text" tabindex="1" id="email" class="text-input" name="email" />
				<a href="http://reg.renren.com/xn6212.do?ss=17043&rt=1&g=${origin}" target="_blank">注册</a>
			</p>
			<p class="clearfix">
				<label for="password">密　码:</label>
				<input type="password" tabindex="2" class="text-input" error="请输入密码" name="password" id="password" />
				<a href="http://safe.renren.com/findPass.do" target="_blank">忘记密码</a>
			</p>
			<div class="caps-lock-tips" id="capsLockMessage" style="display:none"></div>
			<p class="savepassword">
				<label class="labelCheckbox" for="autoLogin" title="为了确保您的信息安全，请不要在网吧或者公共机房勾选此项！">
					<input type="checkbox" tabindex="3" value="true" id="autoLogin" name="autoLogin" />下次自动登录
				</label>
			</p>
			<p>                                        
				<input type="hidden" value="${origURL}" name="origURL" />
				<input type="hidden" value="${errURL} "name="errURL" />
				<input type="hidden" name="domain" value="${applicationScope.sysdomain}" />
				<input type="submit" tabindex="4" value="登录" class="login-btn" id="login" />
			</p>
			<div id="errorMessage" class="errors_div" style="display:none;"></div>
		</form>
	</div>
</div>
