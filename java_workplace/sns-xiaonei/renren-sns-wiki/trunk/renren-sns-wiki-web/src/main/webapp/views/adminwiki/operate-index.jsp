<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<!doctype html>
<html>
<head>
	<title>wiki运营后台-首页运营</title>
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
			<div>
				<h2>正在热映运营</h2>
				<div>
					<a href="http://wiki.renren.com/adminwiki/operate-index/movie/latest">电影</a> | <a href="http://wiki.renren.com/adminwiki/operate-index/book/latest">图书</a> | <a href="http://wiki.renren.com/adminwiki/operate-index/place/latest">景点</a> | <a href="http://wiki.renren.com/adminwiki/operate-index/game/latest">游戏</a> | 	
				</div>
				<h2>热门佳片运营</h2>
				<div>
					<a href="http://wiki.renren.com/adminwiki/operate-index/movie/hot">电影</a> | <a href="http://wiki.renren.com/adminwiki/operate-index/book/hot">图书</a> | <a href="http://wiki.renren.com/adminwiki/operate-index/place/hot">景点</a> | <a href="http://wiki.renren.com/adminwiki/operate-index/game/hot">游戏</a> | 	
				</div>
				<div>
				<h2>
				<c:choose>
					<c:when test="${operateType eq 'latest'}">正在热映-</c:when>
					<c:when test="${operateType eq 'hot'}">热门-</c:when>
				</c:choose>
				<c:choose>
					<c:when test="${wikiTypeName eq 'movie'}">电影</c:when>
					<c:when test="${wikiTypeName eq 'book'}">图书</c:when>
					<c:when test="${wikiTypeName eq 'place'}">景点</c:when>
					<c:when test="${wikiTypeName eq 'game'}">游戏</c:when>
				</c:choose>
				</h2>
				<c:if test="${not empty msg}">
				<div>${msg }</div>
				</c:if>
				<form action="http://wiki.renren.com/adminwiki/operate-index/add" method="post">
				<input type="hidden" name="wikiTypeName" value="${wikiTypeName }" />
				<input type="hidden" name="operateType" value="${operateType }" />
				顺序: 
				<input name="order" value=10 />
				wikiId:<input name="wikiId" />
				<input type="submit" value="添加">
				</form>
				<c:if test="${not empty operateIndexList}">
				<table>
				<c:forEach items="${operateIndexList}" var="operateIndex" varStatus="status">
				<tr>
				<td>${status.count}</td>
				<td><input name="order" value="${operateIndex.order }" /></td>
				<td>${operateIndex.wikiId }</td>
				<td>${operateIndex.wiki.name }</td>
				<td><a onclick="return confirm('确定删除 ${operateIndex.wiki.name }?');" href="http://wiki.renren.com/adminwiki/operate-index/delete/?wikiTypeName=${wikiTypeName }&operateType=${operateType}&wikiId=${operateIndex.wikiId }">删除</a></td>
				</tr>
				</c:forEach>				
				</table>
				</c:if>
				</div>
				
			</div>
			
		</div><!-- right-con end -->
	</div><!-- pagecontent end -->
</body>
</html>