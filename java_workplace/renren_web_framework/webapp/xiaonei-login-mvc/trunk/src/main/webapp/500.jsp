<%@ page contentType="text/html;charset=UTF-8" language="java" session="false"%>
<!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
		<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
		<title>人人网 - 抱歉，出错了</title>
		<%@ include file="/inc/n/head2.inc"%>
		<style type="text/css">
			.wrapper{margin:0 auto;width: 1050px;}
			.wrapper .wrapper-main{margin:20px auto 20px auto;}
			.wrapper .wrapper-main img{margin:0 73px;}
			.wrapper .wrapper-main p{text-align: center;font-size: 13px;color:#666666;}
			.wrapper .wrapper-main a{font-weight:bold;}
		</style>
	</head>
	<body>
		<%@ include file="/inc/n/header2.inc"%>
		<div class="wrapper">
			<div class="wrapper-main">
				<img src="http://s.xnimg.cn/xnapp/music/images/404/20121023/500.png" alt="" title="" width='834' height='237'>
				<p>抱歉，内部错误。&nbsp;<a href="http://www.renren.com">返回首页</a>或者<a href="javascript:history.go(-1)">返回上一页</a></p>
			</div>
			<iframe src="http://music.renren.com/404_ad_3.html" frameborder="no" border="0" height="270" width="1050"></iframe>	
		</div>
		<%@ include file="/inc/n/footer.inc"%>
	</body>
</html>
