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
	<title>注册-开心网-www.kaixin.com开心网是一个真实的社交网络，联络你和你周围的朋友。</title>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/renren.pub.reg.js?ver=$revxxx$"></script>
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

						<p class="pls-login">已经注册的用户<a href="http://login.kaixin.com/GLogin.do">请登录</a></p>
						<div class="reginfo">
							<p class="info-title">完善你的个人信息，加入开心网</p>
							<p class="info-content">开心网是一个与朋友，同学，同事，家人一起玩的地方；可以通过朋友认识更多人；大家一起养宠物狗狗，抢车位，在开心农场种自己喜欢的蔬菜水果，让朋友相互更加了解，联系更加紧密。</p>
						</div>
						<div class="regform">
						<!--begin: reg form-->
						<%@ include file="/inc/errors.inc" %>
                <form action="/register.do" class="reg_layout" id="regform" name="regForm" method="post">
				<input type="hidden" name="fromUrl" id="fromUrl" />
				<input type="hidden" name="ss" id="ss" value="${ss}"/>
				<input type="hidden" name="rt" id="rt" value="${rt}"/>
				<input type="hidden" name="b" id="b" value="${b}"/>
				<input type="hidden" name="appId" id="appId" value="${appId}"/>
				<input type="hidden" name="inviterId" id="inviterId" value="${inviterId}"/>
				<input type="hidden" name="action_id" id="action_id" value="208202" />
				<input type="hidden" name="g" id="g" value="${requestScope.g}" />
				<%@ include file="inc/jsanti.inc" %>
					<fieldset class="account-field">
						<legend>账户信息</legend>
						<table>
							<tr>
								<th>你的Email：</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="text" name="regEmail" id="regEmail" tabindex="1" />
									<%@ include file="inc/reg_email_recomm.inc" %></td>
								<td>
									<div class="zc_cwts hide" id="regEmail_ok" style="display:none;">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" /></div>
									<div id="regEmail_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr>
								<th>你的密码：</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="password" name="pwd" id="pwd" tabindex="2" onblur="this.form.repwd.value=this.value;"/>
									<input type="hidden" id="repwd" name="repwd" />
								</td>
								<td>
									<div class="zc_cwts hide" id="pwd_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" /></div>
									<div id="pwd_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<!-- tr>
								<th>再输入一遍密码:</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="password" name="repwd" id="repwd" tabindex="7" />
								</td>
								<td>
									<div class="zc_cwts hide" id="repwd_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"  /></div>
									<div id="repwd_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr-->
						</table>
						
					</fieldset>
					<fieldset>
						<legend>详细信息</legend>
						<table>
							<tr>
								<th>你的姓名：</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="text" name="name" id="name" tabindex="3"  style="width:200px;height:22px;padding:5px 5px 0;" />
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
									<label for="male" class="reg_radio"><input type="radio" name="gender" value="男生" id="male" tabindex="4" />男</label>
									<label for="woman" class="reg_radio"><input type="radio" name="gender" value="女生" id="woman" tabindex="5"/>女</label>
								</td>
								<td>
									<div class="zc_cwts hide" id="gender_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" /></div>
									<div id="gender_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr  style="display:none">
								<th></th>
								<td class="inputholder">
									<input type="hidden" name="birth_year" id="birth_year" value="1700"/>
									<input type="hidden" name="birth_month" id="birth_month" value="1"/>
									<input type="hidden" name="birth_day" id="birth_day" value="1"/>
								</td>
								<td>							
												<div id="birth_year_err" class="regerror-tip">&nbsp;</div>
												<div id="birth_month_err" class="regerror-tip">&nbsp;</div>
												<div class="regok-tip" id="birth_day_ok" style="display:none;">&nbsp;</div>
												<div id="birth_day_err" class="regerror-tip">&nbsp;</div>
								</td>
							</tr>
							<tr  style="display:none">
								<th></th>
								<td class="inputholder">
										<input type="hidden" name="homeprovince" id="homeprovince" value=""/>
										<input type="hidden" name="homecitycode" id="homecitycode" value=""/>
										<input type="hidden" name="workplacename" value=""/>
										<input type="hidden" name="stage" value="30"/>
										<input type="hidden" name="basicControl" value="99"/>
								</td>
								<td>
												
												<div id="homeprovince_err" class="regerror-tip">&nbsp;</div>
												<div id="homecitycode_err" class="regerror-tip">&nbsp;</div>
												<div class="regok-tip" id="homecitycode_ok" style="display:none;">&nbsp;</div>
								</td>
							</tr>							
						</table>
					</fieldset>
					<fieldset style="border-bottom:none">
						<legend>验证程序</legend>
						<table style="width:80%">
						
							<tr>
								<th></th>
								<td class="inputholder coder">
									<p class="float-left coder_img">
									<%
										java.util.Random random = new java.util.Random();
										int id = random.nextInt(1431655764);
									%>
										<input type="hidden" id="key_id" name="key_id" value="<%=id%>"/> 
										<img id="verifyPic" src="${applicationScope.urlIcode}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/>
										<a href="javascript:refreshCode();" onclick="javascript:void(0);" tabindex="7">看不清楚 换一张</a>
									</p>
								</td>
							</tr>
							<tr class="coder">
								<th>验证码：</th>
								<td class="inputholder">
									<p class="float-left coder_input">
										<span class="required">*</span>
										<input type="text" name="icode" id="icode" size="5" maxlength="5" tabindex="6" style="height:22px;padding:5px 5px 0;" />&nbsp;&nbsp;
									</p>									
								</td>
								<td>
									<div class="zc_cwts hide" id="icode_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" /></div>
									<div id="icode_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr style="display:none">
									<th></th>
									<td class="inputholder"><span class="required"></span>
										<input type="hidden" name="msnAccountOk" id="msnAccountOk" value="0" />
										<input type="hidden" name="msnValid2" id="msnValid2" value="0" />
										<input type="text" id="ismsn" name="ismsn" value="abc"/>
										</td>
									<td><div style="display: none;" id="ismsn_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="ismsn_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
							</tr>
							<tr>
								<td colspan="3" class="colspan" style="height:53px">
									<div id="waiting" name="waiting" style="display:none">
                    					<img src="${applicationScope.urlStatic}/kaixin/imgpro/icons/indicator.gif" /> 正在提交数据，请稍候...
                					</div>
									<button id="reg_sub" onclick="ismsnok();$('regform').submit();" style="cursor:pointer;" class="abnotinv_login" type="button	" tabindex="8"></button>
									<!--<a href="http://reg.kaixin.com/kx7118.do" target="_blank">先看一下&raquo;</a>-->
								</td>
							</tr>
							<tr style="display:none">
									<td class="colspan" colspan="2"><input type="checkbox" checked="checked" name="accept" id="accept"/></td>
									<td><div class="zc_cwts hide" id="accept_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
						</table>
					</fieldset>
					<input type="hidden" name="regFlag" id="regFlag" value="public_reg" />
					<div class="exinfo">
						<p><a id="reg_sub2" class="microReg" onclick="$('msnValid2').value='1';check2.submit();return false;" href="#" tabindex="9">使用微软认证</a>点击以上按钮，我们将认可您已阅读并同意遵守<a target="_blank" href="http://kaixin.com/info/agreement.jsp" tabindex="10">服务条款</a></p>
						<p class="sec-info"><input type="checkbox" name="sharemsg" id="sharemsg" value="ok" checked tabindex="11" />你可以将这个消息分享给更多的msn好友， 并可能出现您的 Windows Live Id 在此自动登陆的情况。</p>
					</div>
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
<%@ include file="inc/show_rate.inc" %>
<%@ include file="inc/set_form.inc.jsp" %>
</body>
</html>