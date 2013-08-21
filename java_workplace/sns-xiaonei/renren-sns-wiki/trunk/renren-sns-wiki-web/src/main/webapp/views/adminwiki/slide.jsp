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
			<h2>${wikiTypeName } - Slide运营</h2>
			<div>
			<ul>
				<li><a href="http://wiki.renren.com/adminwiki/operate-index/slide/page/movie">电影</a></li>
				<li><a href="http://wiki.renren.com/adminwiki/operate-index/slide/page/book">书籍</a></li>
				<li><a href="http://wiki.renren.com/adminwiki/operate-index/slide/page/place">景点</a></li>
				<li><a href="http://wiki.renren.com/adminwiki/operate-index/slide/page/game">游戏</a></li>
			</ul>
			</div>
			<c:if test="${not empty msg}">
			<div><strong>${msg }</strong></div>
			</c:if>
			<div class="clearfix"></div>
			<div>
				<form action="http://wiki.renren.com/adminwiki/operate-index/slide/add" method="post">
				<input type="hidden" name="wikiTypeName" value="${wikiTypeName }" />
				order:<input type="text" name="order" value="10" size="2" />
				wikiId:<input type="text" name="wikiId" />
				图片URL:<input type="text" name="imageUrl" value="http://fmn.rrimg.com/fmn062/20120821/1745/original_Av4F_572500001e63125c.jpg" />
				<input type="submit" value="添加" />
				</form>
			</div>			
			<div>
			<table>
			<c:forEach items="${operateIndexSlideList}" var="operateIndexSlide" varStatus="status">
			<tr>
			<td>${status.count }</td>
			<td>${operateIndexSlide.order }</td>
			<td>${operateIndexSlide.wikiId }</td>
			<td><a href="http://wiki.renren.com/${operateIndexSlide.wikiId}" target="_blank">${operateIndexSlide.wiki.name }</a></td>
			<td><a href="${operateIndexSlide.imageUrl }" target="_blank">查看图片</a>(${operateIndexSlide.imageWidth },${operateIndexSlide.imageHeight })</td>
			<td>
			<a href="http://wiki.renren.com/adminwiki/operate-index/slide/edit?wikiTypeName=${wikiTypeName }&wikiId=${operateIndexSlide.wikiId }">编辑</a> | 
			<a onclick="return confirm('确定删除 ${operateIndexSlide.wiki.name }?');" href="http://wiki.renren.com/adminwiki/operate-index/slide/delete?wikiTypeName=${wikiTypeName }&wikiId=${operateIndexSlide.wikiId }">删除</a></td>
			</tr>
			</c:forEach>			
			</table>			
			</div>
		</div><!-- right-con end -->
	</div><!-- pagecontent end -->
</body>
</html>