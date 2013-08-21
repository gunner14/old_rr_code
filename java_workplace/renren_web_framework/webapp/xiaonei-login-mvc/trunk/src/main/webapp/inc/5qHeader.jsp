<%@ page pageEncoding="GBK" %>
<div id="header">
	<h1 id="logo"><a href="${applicationScope.urlHome}/Home.do" title="${domainIdentified["siteName"]}"><img src="${applicationScope.urlSmallStatic}/img/logo-mini.gif" alt="${domainIdentified["siteName"]}(Logo)" /></a></h1>
	<div id="navigation">
		<h3>主导航:</h3>
		<ul>
			<li><a href="${applicationScope.urlHome}/Home.do">首页</a></li>
			<li><a href="${applicationScope.urlBrowse}/SocialNet.do">随便看看</a></li>
			<li><a href="${applicationScope.urlSchool}/university.do?id=${requestScope.visiter.univ}">学校</a></li>
			<li><a href="${applicationScope.urlBlog}/BlogHome.do">日志</a></li>
			<li><a href="${applicationScope.urlPhoto}/freshalbum.do">相册</a></li>
			<li><a href="${applicationScope.urlGroup}/RandomTribe.do">群<c:if test="${requestScope.visiter.source == 5}">/圈子</c:if></a></li>
			<li><a href="${applicationScope.urlEvent}/GetEventList.do?univ=${requestScope.visiter.univ}">活动</a></li>
			<li><a href="${applicationScope.urlBrowse}/Search.do">搜索</a></li>
			<li><a href="${applicationScope.urlMain}/Invite.do">邀请</a></li>
		</ul>
	</div>
	<div id="utility">
		<c:choose>
			<c:when test="${empty requestScope.visiter}">
				<html:form method="post" action="/Login.do" onsubmit="getEl('email').value=Trim(getEl('email').value);" focus="email">
					<input type="hidden" name="origURL" value="${requestScope.origURL}" />
					<label class="label" for="email">帐号email:</label><html:text property="email" styleClass="inputtext" styleId="email" size="12" maxlength="20" />
					<label class="label" for="password">密码:</label><input type="password" name="password" id="password" class="inputtext" size="12" maxlength="20" />
					<input type="submit" id="login" name="submit" class="inputbutton" value="登录" />
					<input type="button" id="register" name="register" class="inputbutton" value="注册" onclick="document.location='${applicationScope.urlMain}/Register.do'" />
				</html:form>
			</c:when>
			<c:otherwise>
				<%--你好，--%><a href="${applicationScope.urlMain}/BasicInfo.do">${requestScope.visiter.name}</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/Logout.do">退出</a>
			</c:otherwise>
		</c:choose>
	</div>
</div>
