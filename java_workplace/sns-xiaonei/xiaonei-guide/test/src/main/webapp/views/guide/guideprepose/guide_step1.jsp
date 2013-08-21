<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<link href="http://s.xnimg.cn/a33000/n/core/global2-all-min.css" rel="stylesheet" type="text/css" />
	<link href="http://s.xnimg.cn/apps/guide/css/guide-all-min.css" rel="stylesheet" type="text/css" />
	<script src="http://s.xnimg.cn/a33024/n/core/base-all2.js" type="text/javascript"></script>
	<script src="http://s.xnimg.cn/apps/guide/js/combine/guide-step1-all-min.js" type="text/javascript"></script>
</head>
<body id="guide-newflow">
	
	<div class="login-register clearfix">
		<div class="login"></div><!-- 左侧登录 -->
		
		<div class="register">
			<div class="logo"><h2>人人网是中国最真实、最有效的社交平台</h2></div>
			<div class="index clearfix">
				<h3>注册人人网，立即找出好友：</h3>
				<form action="http://reg.renren.com/vk-fill-basic-info" method="post">
					<dl>
						<dt><label for="">性别：</label></dt>
						<dd>
							<input type="radio" name="sex" value="1"> 男&nbsp;&nbsp;
							<input type="radio" name="sex" value="0"> 女
						</dd>
					</dl>
					<dl>
						<dt><label for="">生日：</label></dt>
						<dd>
							<select class="width75" name="birthyear" id="birth-year"><option>--</option></select>年
							<select class="width75" name="birthmonth" id="birth-month"><option>--</option></select>月
							<select class="width75" name="birthday" id="birth-day"><option>--</option></select>日
						</dd>
					</dl>
					<dl>
						<dt><label for="">现在：</label></dt>
						<dd>
							<select name="stage">
								<option>请选择身份</option>
								<option value="work">已经工作了</option>
								<option value="univ">正在上大学</option>
								<option value="mid">正在上中学</option>
								<option value="none">是其他身份</option>
							</select>
						</dd>
					</dl>
					<dl>
						<dt>&nbsp;</dt>
						<dd>
							<input type="submit" class="arrow" value="注册"/>
						</dd>
					</dl>
				</form>
			</div>
			<div class="detail">
				<span>加入人人网你可以：</span>
				<ul class="clearfix">
					<li>联络朋友，了解他们的最新动态</li>
					<li>自由、安全地控制个人隐私</li>
					<li>和朋友分享相片、音乐和电影</li>
					<li>用照片和日志记录生活，展示自我</li>
					<li>找到老同学结识新朋友</li>
				</ul>
			</div>
		</div>
	</div>
	
</body>
</html>
 