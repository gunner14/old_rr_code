<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<form method="post" id="loginForm" action="http://passport.renren.com/PLogin.do" >
	<input type="hidden" id="loginregFrom" name="loginregFrom"  value="msntab_bomb" />
	<input type="hidden" id="inviterId" name="inviterId"  value="${requestScope.inviterId}" />
	<input type="hidden" id="appId" name="appId"  value="${requestScope.appId}" />
	<input type="hidden" id="inviteType" name="inviteType"  value="${requestScope.inviteType}" />
	<input type="hidden" id="e" name="e" value="${requestScope.inviteeEmail}" />
	<input type="hidden" id="fromUrl" name="fromUrl"  value="${requestScope.fromUrl}" />
	<input type="hidden" id="code" name="code"  value="${requestScope.code}" />
	<input type="hidden" id="from" name="from"  value="${requestScope.from}" />
	<input type="hidden" id="appToken" name="appToken"  value="${requestScope.appToken}" />
	<input type="hidden" id="groupName" name="groupName"  value="${requestScope.groupName}" />
	<input type="hidden" id="ss" name="ss"  value="${requestScope.ss}" />
	<input type="hidden" id="ayid" name="ayid" value="${requestScope.ayid}" />
	<input type="hidden" id="iid" name="iid"  value="${requestScope.iid}" /><input type="hidden" name="domain" value="kaixin.com" />
	<input type="hidden" name="origURL" value="http://home.kaixin.com/Home.do" />
	<input type="hidden" name="g" id="g" value="${requestScope.g}" />
	<input type="hidden" name="b" id="b" value="${requestScope.b}" />
	<div class="clear">
		<label for="username">帐号:</label>
		<p><input type="text" id="email" name="email" rule="blank#loginName" class="text" tabindex="12"/></p>
	</div>
	<div class="clear">
		<label for="password">密码:</label>
		<p><input type="password" id="password" name="password" class="text" tabindex="13"/></p>
	</div>
	<div class="clear">
		<label>&nbsp;</label>
		<p><input type="submit" value="" class="login_button" style="cursor:pointer;" tabindex="14"/></p>
	</div>
</form>