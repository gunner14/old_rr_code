<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@page import="com.renren.sns.lead.dataproviders.util.StatLogger"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<html xmlns="http://www.w3.org/1999/xhtml">
<!-- subject filter -->
<head><!-- _mu_gum_ -->
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,人人,人人网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
	<%@ include file="/inc/head-guide-flow.inc" %>
	<title>${domainIdentified["siteName_fillinfo"]} - 账号绑定</title><link href="${applicationScope.urlSmallStatic}/favicon-rr.ico" rel="shortcut icon" type="image/x-icon" />
	<script>var guideNextStep = 'preview?new=1&skip=1';</script>
	<link href="${applicationScope.urlSmallStatic}/renren_logo_apple.png" rel="apple-touch-icon" />
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/imgpro/msntab/scripts/swfobject.js?ver=$revxxx$"></script>
	
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
</head>
<body>
	<div class="main_wrap clearfix">

		<div class="reg_logo"></div>
		<div class="guide-reg clearfix">


			<!-- left changed -->
			<div class="main_left">
				<div class="form_wrap clearfix">
					<span class="login"><a href="http://www.renren.com/Logout.do">退出</a></span>

					<div class="user_title clearfix">
						你的糯米账号已在人人网注册，输入验证码即可绑定人人账号。
						<p class="small">糯米账号绑定人人网，联系好友、团购两不误。</p>
					</div>
					<form method="post" id="regform"  name="regform" action="leads">
						<input name="act" value="mu_commit_vcode" type="hidden"></input>
						<div class="mail_title">你的邮箱<strong>${visiter.account}</strong>将收到一封认证邮件，请将邮件内的验证码填写到下方。</div>
						<dl>
							<dt><label for="pwd">邮箱验证码:</label></dt>
							<%
							String msg = "查看邮件，填写验证码";
							String style = "";
							if("2".equals(request.getParameter("e"))){
								msg = "验证码错误，请重试";
								style = "box-error-error";
							}
							else if("1".equals(request.getParameter("e"))){
								msg = "请输入验证码";
								style = "box-error-error";
							}
							%>
							<dd><input type="text" maxlength="20" name="vcode" id="pwd" class="inputtext" /><span id="mmg"></span><span id="pwd_error_span" class="box-error <%=style %>" style="display:inline-block"><b id="pwd_error_span_i"><%=msg %></b></span></dd>
						</dl>
						<div class="bind">
							<input type="submit" class="input-submit" value="立即绑定"/>
							<p class="tabsort" id="pnote">若一直没收到验证邮件，可以<a id="" href="javascript:doSend();">重新发送</a></p>
							<p class="tabsort" id="psend" style="display:none">发送中...请稍候...</a></p>
							<p class="tabsort" id="psucc" style="display:none">已发送成功，请查收邮件</a></p>
						</div>

					</form>
				</div>
			</div>
			<!-- change over -->


			<div class="main_right">
				<div class="youknow">
					<img src="http://a.xnimg.cn/imgpro/reg/new/intro.png" alt="" />
				</div>

				<dl>
					<dt>最热门公共主页</dt>
					<dd>
						<a href="#"><img width="73" src="http://a.xnimg.cn/imgpro/homeAd/zw.png" alt="" /></a>

						<span class="name">赵薇</span>
					</dd>
					<dd>

						<a href="#"><img width="75" src="http://hdn.xnimg.cn/photos/hdn521/20091023/1545/head_ZjIz_28580d019118.jpg" alt="" /></a>
						<span class="name">李开复</span>
					</dd>
					<dd>
						<a href="#"><img width="73" src="http://a.xnimg.cn/imgpro/homeAd/sj.png" alt="" /></a>

						<span class="name">商界</span>
					</dd>

				</dl>
				<dl>
					<dt>他玩的游戏</dt>
					<dd>
						<a href="#"><img width="75" src="http://a.xnimg.cn/imgpro/reg/new/xxzz.png" alt="" /></a>
						<span class="name">小小战争</span>

					</dd>

					<dd>
						<a href="#"><img width="75" src="http://hdn.xnimg.cn/photos/hdn521/20091230/1825/h_head_MWjq_29a400000ef12f74.jpg" alt="" /></a>
						<span class="name">人人农场</span>
					</dd>
				</dl>
			</div>	
			<!-- div class="b_text">
				点击免费开通帐号表示您同意并遵守<a href="http://renren.com/info/agreement.jsp" target="_blank">人人网服务条款</a>

			</div -->
		</div>
	</div>
	<script>
	function doSend(){
		$('pnote').style.display='none';
		$('psucc').style.display='none';
		$('psend').style.display='block';
		new XN.net.xmlhttp({
			url:'leads',
			method:'post',
			data:'act=mu_resend_vcode',
			onSuccess:function(r){
				var res = r.responseText;
				$('pnote').style.display='none';
				$('psucc').style.display='block';
				$('psend').style.display='none';
				XN.DO.alert('邮件已发送，请注意查收哦^_^');
			}
		});
		
		setTimeout(function () {
			$('pnote').style.display='block';
			$('psucc').style.display='none';
			$('psend').style.display='none';
		},8000);
	}
	</script>
</body>


</html>

