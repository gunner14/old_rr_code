<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta name="Description" content="注册开心网。开心网是帮助你与朋友、同事、同学、家人保持更紧密联系的真实社交平台。注册开心网你可以及时了解他们的最新动态；结识更多的新朋友">
	<meta name="Keywords" content="开心网,注册,kaixin,真开心网,同事,白领,在职人士,博客,相册,社区,交友,游戏,休闲,聊天,音乐,视频,开心美女,开心宝贝,开心达人,分享照片,网页游戏,flash游戏,休闲游戏,办公室宠物,开心农场,投票,抢车位,超级大亨,狗狗,咬我吧,开发者,好友买卖,测试,谁最牛,教父,迷你城市,照片经纪人,时尚教主">
	<link href="${applicationScope.urlStatic}/kaixin/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/favicon.ico" rel="shortcut icon" type="image/x-icon" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg-t.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<title>开心网-www.kaixin.com开心网是一个真实的社交网络，联络你和你周围的朋友。</title>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/imgpro/msntab/swfobject.js?ver=$revxxx$"></script>
</head>

<body id="syshome" class="reg new_inv_mail">
	<div class="bg">
		<div class="wrapper clearfix">
			<div class="header">
				<h1><a href="http://www.kaixin.com"><img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/g2logo.png" alt="kaixin.com" title="kaixin.com" /></a></h1>			
			</div>
			<div class="body">
				<div class="widebar">
					<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/g2conner.png" class="top-conner" />
					<div class="regbox">
						<div class="reginfo">
							<p class="info-title">完善你的个人信息，加入开心网</p>
							<p class="info-content">开心网是一个与朋友，同学，同事，家人一起玩的地方；可以通过朋友认识更多人；大家一起养宠物狗狗，抢车位，在开心农场种自己喜欢的蔬菜水果，让朋友相互更加了解，联系更加紧密。</p>
						</div>
						<div class="regform">
						<!--begin: reg form-->
						<%@ include file="/inc/errors.inc" %>
                <form name="regForm" id="regform" name="regForm" method="post" action="/fxmo.do" class="reg_layout">
                <input type="hidden" name="uid" value="${requestScope.uid}" id="uid">
					<fieldset class="account-field">
						<legend>账户信息</legend>
						<table>
							<tr>
								<th>你的Email：</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="text" name="email" tabindex="5" id="email" value="${requestScope.email}">
									<%@ include file="inc/reg_email_recomm.inc" %>
								</td>
								<td>
									<div class="zc_cwts hide" id="email_ok" style="display:none;">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" /></div>
									<div id="email_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>							
						</table>						
					</fieldset>
					<fieldset>
						<legend>详细信息</legend>
						<table>
							<tr>
								<th>你的姓名：</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="text" name="name" tabindex="8" id="name" style="width:200px;height:22px;padding:5px 5px 0;" value="${requestScope.name}">
								</td>
								<td>
									<div class="zc_cwts hide" id="name_ok">
									&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"  />
									</div>
									<div id="name_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr>
								<th>性别：</th>
								<td class="required">
									<span class="required">*</span>
									<label for="male" class="reg_radio"><input type="radio" name="gender" tabindex="9" value="男生" id="male">男生</label>
                                    <label for="woman" class="reg_radio"><input type="radio" name="gender" tabindex="10" value="女生" id="woman">女生</label>
								</td>
								<td>
									<div class="zc_cwts hide" id="gender_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" /></div>
									<div id="gender_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
						</table>
					</fieldset>
					<fieldset style="border-bottom:none">
						<table style="width:80%">
							<tr>
								<td colspan="3" class="colspan" style="height:53px">
									<button id="reg_sub" onclick="$('regform').submit();" style="cursor:pointer;" class="abnotinv_login" type="button"></button>									
								</td>
							</tr>
						</table>
					</fieldset>
					</form>
					<!--end: red form-->
					</div>
					</div>					
					
				</div>
				<div class="sidebar">
					<h2>领养一只狗狗或者猫猫做宠物</h2>
					<div class="dogflash">
						<embed width="220" height="220" quality="high" name="dog" id="dog" wmode="transparent" src="${applicationScope.urlStatic}/kaixin/swf/dogforguide/dog_guide_husky.swf" type="application/x-shockwave-flash"></embed>
					</div>				
					<p class="dogname">哈士奇</p>
					<p class="dogage">年龄：两个月大</p>
				</div>
			</div>
		</div>
	</div>
</body>
</html>