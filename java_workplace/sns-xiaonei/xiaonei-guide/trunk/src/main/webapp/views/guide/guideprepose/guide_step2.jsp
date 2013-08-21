<%@ page contentType="text/html;charset=UTF-8" session="false" %>

<!doctype html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>guide</title>
		<link href="http://s.xnimg.cn/a33000/n/core/global2-all-min.css" rel="stylesheet" type="text/css" />
		<link href="http://s.xnimg.cn/a11667/csspro/module/school-selector.css" rel="stylesheet" type="text/css" />
		<link href="http://s.xnimg.cn/apps/guide/css/guide-all-min.css" rel="stylesheet" type="text/css" />
		<script src="http://s.xnimg.cn/a33024/n/core/base-all2.js" type="text/javascript"></script>
		<script src="http://s.xnimg.cn/a25436/jspro/xn.ui.schoolSelectors.js" type="text/javascript"></script>
		<script src="http://s.xnimg.cn/apps/guide/js/combine/guide-all-min.js" type="text/javascript"></script>
	</head>
	<body id="guide-newflow">
		<div class="login-register clearfix">
			<div class="login"></div><!-- 左侧登录 -->
			
			<div class="register" id="register">
				<div class="step2 first"> <!--如果是2步，就是step2，如果是3步，就step3.  第一步添加class="first"  第二步添加class="second" 第三步添加class="third"-->
					<p class="on">找同学、好友</p> <!-- 选中的字体为白色，添加class="on" -->
					<p>完善信息</p>
				</div>
				<div class="title">
					<h3>搜索你的同学、好友</h3>
					<a href="#"> &lt;&lt; 返回上一页</a>
				</div>
				<div class="select-stage clearfix">
					<dl class="tab-change">
						<dt><label for="">毕业学校：</label></dt>
						<dd>
							<input type="radio" name="school-grp" checked="checked"> 大学
							<input type="radio" name="school-grp" /> 高中
							<input type="radio" name="school-grp" /> 中专
							<input type="radio" name="school-grp" /> 初中
							<input type="radio" name="school-grp" /> 小学
							<input type="radio" name="school-grp" /> 工作
						</dd>
					</dl>
					<form action="" method="post" id="guide-form">
						<%@ include file="/views/guide/guideprepose/info-colleage.inc" %>
						<%@ include file="/views/guide/guideprepose/info-high.inc" %>
						<%@ include file="/views/guide/guideprepose/info-tech.inc" %>
						<%@ include file="/views/guide/guideprepose/info-junior.inc" %>
						<%@ include file="/views/guide/guideprepose/info-elem.inc" %>
						<%@ include file="/views/guide/guideprepose/info-company.inc" %>
					</form>
				</div>
				
				<div class="friendslist clearfix" id="rec-friend">
					<div class="title">
						<h3>这些人可能是你认识的好友</h3>
					</div>
					<ul class="clearfix"></ul>
					<div class="next fixed" id="attach-bottom">
						<div class="friend-num">
							<span>已经发送<strong id="addedFriendNum">0</strong>个好友申请</span><input class="next-btn" value="进入下一项"/>
						</div>
					</div>
				</div>
				
			</div>
			<!-- 重点结束 -->
		</div>
		<div id="tip"></div>
	</body>
</html>