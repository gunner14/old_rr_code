<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta name="Description" content="开心是一个真实的社交网络，联络你和你周围的朋友。加入开心网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,开心,开心网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园" />	
	<title>开心网 - 注册</title>
	<link href="${applicationScope.urlStatic}/kaixin/favicon.ico" rel="shortcut icon" type="image/x-icon" />	
    <script type="text/javascript" src="http://s.xnimg.cn/kaixin/js/fillinfo.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/kaixin/js/inCityArray.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/renren.msn.reg.js?ver=$revxxx$"></script>
	<script language="javascript" type="text/javascript">
		function refreshCode() {
			$('verifyPic').src = $('verifyPic').src + Math.random();
		}
	</script>	
	<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/apps/invite-reg-new.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
</head>
<body>
<div class="mask_layer" id="mask_layer" style="display:none"></div> 
<div id="reg_layer" style="display:none;" class="generic_dialog explanation pop_dialog">
	<div class="generic_dialog_popup">
		<table id="pop_dialog_table" class="pop_dialog_table">
		<tbody>
		<tr>
		<td class="pop_topleft">
		</td><td class="pop_border">
		</td><td class="pop_topright">
		</td></tr>
		<tr>
		<td class="pop_border">
		</td><td id="pop_content" class="pop_content">
		<h2>
			<!--<a class="link_out" href="#" target="_blank">稍后再说&raquo;</a>-->
			<span>请完善您的个人资料：</span>
		</h2>
		<div class="regAdWrapper">
			<div class="">	
                <form action="/rgac.do" id="regform" name="regForm" method="post" class="reg_layout">
                	<input type="hidden" name="action_id" id="action_id" value="207113" />
					<%@ include file="inc/reg_form_short.inc" %>
								<tr>
									<td class="colspan" colspan="3">
									<div id="waiting" name="waiting" style="display:none">
                    					<img src="${applicationScope.urlStatic}/kaixin/imgpro/icons/indicator.gif" /> 正在提交数据，请稍候...
                					</div>
									<button style="cursor: pointer;" type="button" id="reg_sub" onclick="ismsnok();check1.submit();" tabindex="8">我已填写完毕</button></td>
								</tr>
								<tr style="display:none">
									<td class="colspan" colspan="2"><input type="checkbox" checked="checked" name="accept" id="accept"/></td>
									<td><div class="zc_cwts hide" id="accept_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								<tr>
									<td colspan="3">
									<div class="sharemsg_wrap">
									<a id="reg_sub2" class="microReg" onclick="$('msnValid2').value='1';check2.submit();return false;" href="#microReg" tabindex="9">使用微软认证</a> 
									点击以上按钮，我们将认可您已阅读并同意遵守<a href="${applicationScope.urlMain}/info/agreement.jsp" target="_blank" tabindex="10">服务条款</a>。<br />
									<input type="checkbox" checked="checked" name="sharemsg" id="sharemsg" value="ok" tabindex="11"/>
										你可以将这个消息分享给更多的msn好友，
										并可能出现您的 Windows Live Id 在此自动登陆的情况。										
									</div>
									</td>
								</tr>
							</table>
						</fieldset>
						<input type="hidden" name="regFlag" id="regFlag" value="invite_reg_dog2" />
					</form>
					</div>
				</div>  
		</td>
		<td class="pop_border">
		</td></tr>
		<tr>
		<td class="pop_bottomleft">
		</td><td class="pop_border">

		</td><td class="pop_bottomright">
		</td></tr>
		</tbody>
		</table>
	</div>
</div>



<div class="main_wrap">
	<div class="main">
		<ul class="pet_list clearfix">
			<li id="dog_1"></li>
			<li id="dog_2"></li>
			<li id="dog_3"></li>
			<li id="dog_4"></li>
			<li id="dog_5"></li>
			<li id="dog_6"></li>
		</ul>
		<script type="text/javascript">
			var Flip = {
				on: function() {
					this.className = this.id + '_hover';
				},
				out: function() {
					this.className = '';
				},
				click: function() {
					$('mask_layer').style.display='block';
					$('reg_layer').style.display='block';
				}
			};
			for (var i = 1; i <=6; i++) {
				$('dog_' + i).onmouseover = Flip.on;
				$('dog_' + i).onmouseout = Flip.out;
				$('dog_' + i).onclick = Flip.click;
			}
		</script>
		
		<div class="login_box">
			<%@ include file="inc/reg_form_login.inc" %>
		</div>
		
		<ol>
			<li >1. 他们是你从未见过的，最为逼真的虚拟宠物。</li>
			<li >2. 他们会跑、会跳，要吃东西、喝水、拉屎。</li>
			<li >3. 他们喜欢和人玩，被人抚摸。</li>
			<li >4. 他们是最完美的宠物：不会弄脏弄乱你的家。</li>
			<li >5. 已经有超过5,000,000的爱心人士领养了宠物。</li>
		</ol>
		
		<div class="button_wrap">
			<a href="#reg" id="button_go" onfocus="this.blur()" onclick="Flip.click()"></a>
		</div>
	</div>
</div>
<script type="text/javascript">
<%
if("1".equals(request.getAttribute("I_err"))){
%>
$('mask_layer').style.display='block';
$('reg_layer').style.display='block';
<%
}
else{
%>
<%
}
%>
</script>
<%@ include file="inc/show_rate.inc" %>
<%@ include file="inc/set_form.inc.jsp" %>
</body>
</html>


