<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<%@include file="inc/modify-wikiid.jsp"%><!doctype html>
<html>
<head>
<title>wiki运营后台</title>
<link rel="stylesheet" type="text/css"
	href="http://wiki.renren.com/views/adminwiki/css/wiki.css" />
</head>
<body>
<div class="pagehead">
<div class="headcenter">
<h1><a href="http://www.renren.com/"><img width=160 height="31"
	src="http://s.xnimg.cn/imgpro/v6/logo.png" alt="人人网" /></a></h1>
</div>
</div>
<div class="pagecontent clearfix"><%@include
	file="inc/adminwiki-nav.jsp"%>
<div class="right-con"><%@include file="inc/adminwiki-header.jsp"%>
<%@include file="inc/single-nav.jsp"%>
<div id="preNext"><a
	href="http://wiki.renren.com/adminwiki/modifyWikiWord/${wikiId }/pre?light=${light }&wikiType=${wikiType}">上一个</a>
| <a
	href="http://wiki.renren.com/adminwiki/modifyWikiWord/${wikiId }/next?light=${light }&wikiType=${wikiType}">下一个</a>
</div>
<h2>切词词库</h2>
<div>wikiId:${wiki.id }<br />
名称:<a href="http://wiki.renren.com/${wiki.id }">${wiki.name }</a><br />
类型: <c:choose>
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
<!-- 执行上传文件操作的函数 -->
<div>
<form method="post"
	action="http://wiki.renren.com/adminwiki/wikiWord/add"><input
	type="hidden" name="light" value=${light}> <input type="hidden"
	name="wikiType" value=${wikiType}> <input type="hidden"
	name="wikiId" value="${wiki.id }" /> <input type="text" name="name"
	value="${wiki.name }"> <input type="submit" value="添加" /></form>
</div>
<div>
<table>
	<c:forEach items="${wikiWordList}" var="wikiWord">
		<td>${wikiWord.name } <a onclick="return confirm('是否删除');"
			href="http://wiki.renren.com/adminwiki/wikiWord/delete/?wikiId=${wiki.id}&wikiWordId=${wikiWord.id}&light=${light}&wikiType=${wikiType}">删除</a>
		</td>
		<tr>
		</tr>
	</c:forEach>
</table>
</div>


</div>
</div>
</body>
</html>