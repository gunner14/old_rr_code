<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
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
	href="http://wiki.renren.com/adminwiki/modifyWikiInfo/${wikiId }/pre?light=${light }&wikiType=${wikiType}">上一个</a>
| <a
	href="http://wiki.renren.com/adminwiki/modifyWikiInfo/${wikiId }/next?light=${light }&wikiType=${wikiType}">下一个</a>
</div>
<h2>修改资料</h2>
<form
	action="http://wiki.renren.com/adminwiki/setWikiInfoProperty/${wiki.id}"
	method="post">属性名称:<input name="name" /> 属性值:<input
	name="value" /> <input type="submit" value="设置"></form>
<c:choose>
	<c:when test="${not empty wiki}">
		<table>
			<tr>
				<td>wikiId:</td>
				<td>${wiki.id }</td>
				<td>&nbsp;</td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td>wiki名称:</td>
				<td><a href="http://wiki.renren.com/${wiki.id}" target="_blank">${wiki.name }</a></td>
				<td>&nbsp;</td>
				<td>&nbsp;</td>
			</tr>
			<c:forEach items="${wikiInfoPropertylist}" var="wikiInfoProperty">
				<form
					action="http://wiki.renren.com/adminwiki/setWikiInfoProperty/${wiki.id}"
					method="post">
					<input type="hidden" name="name"
								value="${wikiInfoProperty.name }">
				<tr>
					<td>${wikiInfoProperty.name }:</td>
					<td><c:choose>
						<c:when test="${wikiInfoProperty.name eq '简介'}">
							<textarea name="value" style="width:300px;height:100px;">${wikiInfoProperty.value }</textarea>
						</c:when>
						<c:otherwise>
							<input type="text" name="value"
								value="${wikiInfoProperty.value }" size="50">
						</c:otherwise>
					</c:choose></td>
					<td><input type="submit" value="确认修改"> </td>
					<td>
					<c:if test="${wikiInfoProperty.id > 0}">
						<a onclick="return confirm('是否删除');"
							href="http://wiki.renren.com/adminwiki/deleteWikiInfoProperty/${wiki.id}/${wikiInfoProperty.id}">删除</a>
					</c:if>
					&nbsp;
					</td>
				</tr>
				</form>
			</c:forEach>
		</table>
		<br />
		<br />
		<br />
		<p>
		=========从豆瓣导入资料=========
		</p>
		<form action="http://wiki.renren.com/adminwiki/douban/get-info" target="_blank">
			<input type="hidden" name="wikiId" value="${wiki.id}" />
			对应的豆瓣地址:<input name="doubanUrl" /><input type="submit" value="获取资料" />		
		</form>
		<br />
		<br />
	</c:when>
	<c:otherwise>
		wiki不存在
	</c:otherwise>
</c:choose>
</div><!-- right-con end -->
</div>
</body>
</html>