<%@ page contentType="text/html;charset=UTF-8" language="java" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<!DOCTYPE html>
<html>
	<head>
		<%@ include file="/views/inc/wiki-header.inc" %>
		<meta charset="utf-8" />
		<link rel="stylesheet" href="http://s.xnimg.cn/apps/wiki/css/index.css?ver=$revxxx$"/>
		<script src="http://s.xnimg.cn/apps/wiki/js/wiki_model.js?ver=$revxxx$"></script>
		<script src="http://s.xnimg.cn/apps/wiki/js/render.js?ver=$revxxx$"></script>
		<script src="http://s.xnimg.cn/apps/wiki/js/index.js?ver=$revxxx$"></script>
		<title>电影</title>
	</head>
	<body>
		<%@ include file="/inc/n/header2.inc"%>
		<div class="wiki_feed_back"><a href="http://support.renren.com/support/message?type=1000394" target="_blank"><img src="http://a.xnimg.cn/apps/wiki/cssimg/feedback.png"></a></div>
		<div class="movie_con clearfix">
			<div class="sidebar">
				<h2>电影</h2>
				<article>
					<h3>上映年份</h3>
					<section stype="2012">
						<span class="movie-year">2012</span>
						<span class="movie-num">${movieCount2012}部</span>
					</section>
					<section  stype="2011">
						<span class="movie-year">2011</span>
						<span class="movie-num">${movieCount2011}部</span>
					</section>
				</article>
				<article>
					<h3>我的电影</h3>
					<section stype="collect">
						<span class="movie-year">看过</span>
						<span class="movie-num">${movieCountCollect}部</span>
					</section>
					<section stype="wish">
						<span class="movie-year">想看</span>
						<span class="movie-num">${movieCountWish}部</span>
					</section>
				</article>
			</div>
			<div class="main">
			</div>
		</div>
		<%@ include file="/inc/n/footer.inc"%>
	</body>
</html>