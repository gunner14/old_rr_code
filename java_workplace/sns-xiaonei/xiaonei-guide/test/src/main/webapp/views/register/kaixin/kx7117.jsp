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
	<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/apps/invite-reg-play.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />	
</head>
<body>
<div class="main_wrap">
	<div class="main">
		<div class="login_box">
			<%@ include file="inc/reg_form_login.inc" %>
		</div>
		<a id="kaixin_logo" href="http://www.kaixin.com/" target="_blank" title="开心网"></a>
		<div class="main_middle clear">		
			<div class="main_left">
				<div class="l_top"></div>
				<div class="l_body"><img src="${applicationScope.urlStatic}/kaixin/imgpro/msntab/play_left.png" alt="请完善信息，进入游戏" title="请完善信息，进入游戏" /></div>
				<div class="l_bottom"></div>
			</div>
			<div class="form_wrap">
				<div class="r_top"></div>
					
					<form action="/rgac.do" id="regform" name="regForm" method="post" class="reg_layout">
					
						<div class="tip_div">请完善个人信息</div>
					
						<input type="hidden" name="action_id" id="action_id" value="207117" />
                		<%@ include file="inc/reg_form_short.inc" %>
                				<tr>
									<td colspan="3" class="colspan">
									<div id="waiting" name="waiting" style="display:none">
                						<img src="${applicationScope.urlStatic}/kaixin/imgpro/icons/indicator.gif" /> 正在提交数据，请稍候...
            						</div>
									<button id="reg_sub" type="button" onclick="ismsnok();check1.submit();" style="cursor:pointer;float:left" class="notinv" tabindex="8">进入游戏</button>
									</td>
								</tr>
                	<tr style="display:none">
					<td class="colspan" colspan="2"><input type="checkbox" checked="checked" name="accept" id="accept"/></td>
					<td><div class="zc_cwts hide" id="accept_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
				</tr>
				<td colspan="3" class="colspan colspan_1">
					<div class="sharemsg_wrap">
						<a id="reg_sub2" class="microReg" onclick="$('msnValid2').value='1';check1.submit();return false;" href="#microReg" tabindex="9">使用微软认证</a> 
						点击以上按钮，我们将认可您已阅读并同意遵守 <a href="${applicationScope.urlWww}/info/agreement.jsp" target="_blank" tabindex="10">服务条款</a>。<br />
						<input type="checkbox" checked="checked" name="sharemsg" id="sharemsg" value="ok" tabindex="11"/>
						你可以将这个消息分享给更多的msn好友，
						并可能出现您的 Windows Live Id 在此自动登陆的情况。
					</div>
							</td>								
						</tr>
					</table>
				</fieldset>
				<input type="hidden" name="regFlag" id="regFlag" value="channel_reg_kxfarm" />
				</form>
				<div class="r_bottom"></div>
			</div>			
			<img src="${applicationScope.urlStatic}/kaixin/imgpro/msntab/play_right_bottom.png" alt="" class="right_bottom" />		
		</div>
	</div>
</div>
<%@ include file="inc/show_rate.inc" %>
<%@ include file="inc/set_form.inc.jsp" %>
</body>
</html>