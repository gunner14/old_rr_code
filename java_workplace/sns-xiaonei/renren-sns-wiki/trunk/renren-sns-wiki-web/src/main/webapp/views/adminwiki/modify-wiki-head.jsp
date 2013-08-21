<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@include file="inc/modify-wikiid.jsp" %>
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
<%@include file="inc/adminwiki-header.jsp"%>
<%@include file="inc/single-nav.jsp"%>
<div id="preNext">
<a href="http://wiki.renren.com/adminwiki/modifyWikiHead/${wikiId }/pre?light=${light }&wikiType=${wikiType}">上一个</a> | 
<a href="http://wiki.renren.com/adminwiki/modifyWikiHead/${wikiId }/next?light=${light }&wikiType=${wikiType}">下一个</a>
</div>
<h2>修改头像</h2>
<div>wikiId:${wiki.id }<br />
名称:<a href="http://wiki.renren.com/${wiki.id }" target="_blank">${wiki.name }</a><br />
类型: 
<c:choose>
<c:when test="${wiki.type == 1}">
电影
</c:when>
<c:when test="${wiki.type == 2}">
书籍
</c:when>
<c:when test="${wiki.type == 3}">
游戏
</c:when>
<c:when test="${wiki.type == 4}">
景点
</c:when>
</c:choose><br />
<img alt="wiki头像" src="${wiki.largeUrl }"></div>
<!-- 执行上传文件操作的函数 -->
<form method="post" action="http://wiki.renren.com/adminwiki/setHead/${wiki.id }" enctype="multipart/form-data">
<input type="file" id="wikiHeadFile" name="wikiHeadFile" /> <input
	type="submit" value="提交" /></form>
		<br />
		<br />
		<br />
		<p>
		=========直接贴图片url,自动上传的...=========
		</p>
		<form action="http://wiki.renren.com/adminwiki/set-head-by-url" method="post">
			<input type="hidden" name="wikiId" value="${wiki.id}" />
			图片URL:<input name="imageUrl" /><input type="submit" value="修改头像" />		
		</form>
		<br />
		<br />
		</div><!-- right-con end -->
	</div>
</body>
</html>