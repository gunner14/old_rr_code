<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring"%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
<title>Social Wiki</title>
<script type="text/javascript"
	src="http://s.xnimg.cn/apps/wiki/js/lib-index-all.js?ver=$revxxx$"></script>
<%@ include file="/views/inc/wiki-index-header.inc"%>
<!--[if IE 6]>
<script type="text/javascript" src="http://s.xnimg.cn/apps/wiki/js/DD_belatedPNG.js"></script>
<script type="text/javascript" >
    DD_belatedPNG.fix('.png');
</script>
<![endif]-->
</head>
<body>
<%@ include file="/inc/n/header2.inc"%>
<div id="wiki_home_container">
<div id="search_bar" class="box">
<div id="search_input"><input type="text" id="wiki_search_input"
	autocomplete="off" placeholder="探寻你想要了解的事物..." value="探寻你想要了解的事物..."> <a
	href="javascript:void(0);" class="search_btn"></a></div>
<div class="search_tips"></div>
<div class="no_result">抱歉，没有找到相关条目，试试帮我们<a
	href="javascript:void(0);" class="add_item">添加这个条目</a>吧！</div>
<p class="search_info">新来的？别害羞（文案待定）</p>
<p class="search_sub">523只词条，大家贡献了432,435条内容！</p>
</div>
<div id="main" class="box">
<div id="category_menu" class="header">
<ul>
	<!-- <li>
		<a>
			<div id="heat"></div>
			<span>热点</span> 
		</a>
	</li> -->
	<li>
		<a category="movie">
			<div id="movie" class="png"></div>
			<span>电影</span>
		</a>
	</li>
	<li>
		<a category="place">
			<div id="place" class="png"></div>
			<span>地点</span> 
		</a>
	</li>
	<li>
		<a category="book">
			<div id="book" class="png"></div>
			<span>书籍</span> 
		</a>
	</li>
	<li>
		<a category="game">
			<div id="game" class="png"></div>
			<span>游戏</span> 
		</a>
	</li>
</ul>
</div>

<div id="slider_panel">
<div class="slider_container">
<div class="slider_left_btn png"></div>
<div class="slider_right_btn png"></div>
<div class="slider_img">
<ul></ul>
</div>
</div>
<ul class="slider_index"></ul>
</div>

<div id="hot">
<div class="title">正在热映</div>
</div>

<div id="classic">
<div class="title">热门佳片</div>
</div>
</div>
<div id="side_panel" class="box"><c:choose>
	<c:when test="${not empty host}">
		<div class="header"><img src="${host.headFullUrl }"
			class="avatar" onload="adjust(this,90,90)">
		<div class="info_box"><span id="user_name">${host.name }</span>
		<ul>
			<li id="movie"><span class="num">${collectCountMap["movie"]}</span><span
				class="word">电影</span></li>
			<li id="book"><span class="num">${collectCountMap["book"]}</span><span
				class="word">书籍</span></li>
			<li id="place"><span class="num">${collectCountMap["place"]}</span><span
				class="word">地点</span></li>
		</ul>
		</div>
		</div>
		<div class="content">
		<ul>
			<li>
			<div class="category_bar" category="movie"><span class="title">电影</span>
			<hr>
			<span class="switch">收起</span></div>
			<div class="content">
			<div class="user_action_title"><span>想看</span><span class="num">${wishCountMap["movie"]}</span><span>部</span></div>
			<ul class="pic_roll">
			<c:forEach items="${wishWikiListMap['movie']}" var="wiki">
				<li><a href="http://wiki.renren.com/${wiki.id }"><img pic="${wiki.headUrl}"></a></li>
			</c:forEach>
			</ul>
			<div class="user_action_title"><span>看过</span><span class="num">${collectCountMap["movie"]}</span><span>部</span></div>
			<ul class="pic_roll">
			<c:forEach items="${collectWikiListMap['movie']}" var="wiki">
				<li><a href="http://wiki.renren.com/${wiki.id }"><img pic="${wiki.headUrl}"></a></li>
			</c:forEach>
			</ul>
			</div>
			</li>
			<li>
			<div class="category_bar" category="book"><span class="title">书籍</span>
			<hr>
			<span class="switch">收起</span></div>
			<div class="content">
			<div class="user_action_title"><span>想看</span><span class="num">${wishCountMap["book"]}</span><span>部</span></div>
			<ul class="pic_roll">
			<c:forEach items="${wishWikiListMap['book']}" var="wiki">
				<li><a href="http://wiki.renren.com/${wiki.id }"><img pic="${wiki.headUrl}"></a></li>
			</c:forEach>
			</ul>
			<div class="user_action_title"><span>看过</span><span class="num">${collectCountMap["book"]}</span><span>部</span></div>
			<ul class="pic_roll">
			<c:forEach items="${collectWikiListMap['book']}" var="wiki">
				<li><a href="http://wiki.renren.com/${wiki.id }"><img pic="${wiki.headUrl}"></a></li>
			</c:forEach>	
			</ul>
			</div>
			</li>
			<li>
				<div class="category_bar" category="place">
					<span class="title">地点</span>
					<hr>
					<span class="switch">收起</span>
				</div>
				<div class="content">
					<div class="user_action_title">
						<span>去过</span>
						<span class="num">${collectCountMap["place"]}</span>
						<span>部</span>
					</div>
					<ul class="pic_roll">
						<c:forEach items="${collectWikiListMap['place']}" var="wiki">
							<li><a href="http://wiki.renren.com/${wiki.id }"><img pic="${wiki.headUrl}"></a></li>
						</c:forEach>
					</ul>
					<div class="user_action_title">
						<span>想去</span>
						<span class="num">${wishCountMap["place"]}</span>
						<span>部</span>
					</div>
					<ul class="pic_roll">
						<c:forEach items="${wishWikiListMap['place']}" var="wiki">
							<li><a href="http://wiki.renren.com/${wiki.id }"><img pic="${wiki.headUrl}"></a></li>
						</c:forEach>
					</ul>
				</div>
			</li>
		</ul>
		</div>
		<!-- content end -->
	</c:when>
	<c:otherwise>
		<div>未登录</div>
	</c:otherwise>
</c:choose></div>
<!-- side_panel end --></div>
<script
	src="http://s.xnimg.cn/apps/wiki/js/wiki-index-min.js?ver=$revxxx$"></script>
<script type="text/javascript">
var slideInfo = ${slideInfo};
</script>
<%@ include file="/inc/n/footer.inc"%>
</body>
</html>