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
			<h2>${wikiTypeName } - Slide编辑</h2>
			<form action="http://wiki.renren.com/adminwiki/operate-index/slide/update" method="post">
			<input type="hidden" name="operateIndexSlide.id" value="${operateIndexSlide.id }">
			<input type="hidden" name="wikiTypeName" value="${wikiTypeName }">
			order:<input type="text" name="operateIndexSlide.order" value="${operateIndexSlide.order }" /><br />
			wikiId:<input type="text" name="operateIndexSlide.wikiId" value="${operateIndexSlide.wikiId }" /><br />
			图片URL:<input type="text" name="operateIndexSlide.imageUrl" value="${operateIndexSlide.imageUrl }" />
			<input type="submit" value="修改" />
			</form>
		</div><!-- right-con end -->
	</div><!-- pagecontent end -->
</body>
</html>