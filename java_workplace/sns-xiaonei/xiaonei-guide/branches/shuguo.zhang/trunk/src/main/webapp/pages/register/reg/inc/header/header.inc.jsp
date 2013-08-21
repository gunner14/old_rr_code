<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ page pageEncoding="UTF-8" %>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<c:choose>
		<c:when test="${page_header_type == 'syshome'}" >
			<meta name="Description" content="人人网（原校内网）是中国最具影响力的真实社交网络。在人人网，找到老同学，结识新朋友，找回你曾经的真情， 了解他们的最新动态；分享相片、音乐和电影；用照片和日志记录生活，展示自我。" />
			<meta name="Keywords" content="人人网,人人网首页,人人,renren,校内,xiaonei,社会性网络,社会化网络,SNS,Social Network Service,应用平台,APP" />
			<title>人人网 人人-登陆首页</title>
		</c:when>
		<c:otherwise>
			<meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
			<meta name="Keywords" content="Xiaonei,人人,人人网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
			<title>人人网 校内 - 注册</title>
		</c:otherwise>
	</c:choose>
	<link href="http://s.xnimg.cn/favicon-rr.ico" rel="shortcut icon" type="image/x-icon" />
	<link href="http://s.xnimg.cn/renren_logo_apple.png" rel="apple-touch-icon" />
