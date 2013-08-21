<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<!doctype html>
<html>
<head>
	<title>wiki运营后台</title>
	<link rel="stylesheet" type="text/css" href="http://wiki.renren.com/views/adminwiki/css/wiki.css" />
</head>
<body>
	<div class="pagehead">
		<div class="headcenter">
			<h1><a href="http://www.renren.com/"><img width=160 height="31" src="http://s.xnimg.cn/imgpro/v6/logo.png" alt="人人网" /></a></h1>
		</div>
	</div>
	<div class="pagecontent clearfix">
		<%@include file="inc/adminwiki-nav.jsp" %>
		<div class="right-con">
			<p class="mb15">当前共有<span class="cur-num">${fn:length(userCacheList)}</span>为运营人员</p>
			<form class="edit-area haveborder" action="http://wiki.renren.com/adminwiki/superAdmin/addSuperAdmin" method="post">
				<input class="inputid" type="text" name="uid" value="输入用户人人ID" />
				<input class="putinId" type="submit" value="设为运营"/>
			</form>
			<ul class="userlist clearfix">
			<c:forEach items="${userCacheList}" var="userCache">
				<li>
					<a class="headerimg" href="http://www.renren.com/${userCache.id }/profile" target="_blank"><img width="50" height="50" src="${userCache.tiny_Url }" alt="${userCache.name }" /></a>
					<a class="headername" href="http://www.renren.com/${userCache.id }/profile" target="_blank" title="${userCache.name }">${userCache.name }</a>
					(<a href="http://wiki.renren.com/adminwiki/superAdmin/removeSuperAdmin?uid=${userCache.id }">移除</a>)
				</li>
			</c:forEach>
			</ul>
		</div>
	</div>
</body>
</html>