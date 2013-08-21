<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta name="Description" content="开心是一个真实的社交网络，联络你和你周围的朋友。加入开心网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,开心,开心网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlStatic}/kaixin/favicon.ico" rel="shortcut icon" type="image/x-icon" />
	<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<title>开心网</title>
    <script type="text/javascript" src="http://s.xnimg.cn/kaixin/js/fillinfo.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/kaixin/js/inCityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/renren.msn.reg.js?ver=$revxxx$"></script>
<style type="text/css">
.inputholder{width:240px!important;}
.kaixin_friends{border:1px solid #e9e9e9;padding-top:10px;}
.content{background:url(${applicationScope.urlStatic}/kaixin/imgpro/reg/farm-dog.jpg) no-repeat;height:980px;position:relative;}
.kaixin_reg_info{float:right;margin-top:340px;margin-right:10px;width:510px;}
.form-tip{width:160px;}
.reg_layout{width:490px;margin:0;}
.login-top{position:absolute;top:50px;right:15px;}
.login-top p{margin:0 0 7px;}
.login-top p .w150{width:150px;}
.login-top p label{font-weight:400;margin:0 7px 0 0;}
.login-top .enter{text-align:center;margin:0;}
.reg_layout{margin:0 10px;padding:0}
</style>
</head>
<body>
	<div class="mainbox">
    <div id="widebar">
       	<div class="content"> 
			<div class="login-top">
				<form method="post" id="loginForm" action="${applicationScope.urlPassport}/PLogin.do" >
				<input type="hidden" id="loginregFrom" name="loginregFrom"  value="msntab_bomb" />
				<input type="hidden" id="inviterId" name="inviterId"  value="${requestScope.inviterId}" />
				<input type="hidden" id="appId" name="appId"  value="${requestScope.appId}" />
				<input type="hidden" id="inviteType" name="inviteType"  value="${requestScope.inviteType}" />
				<input type="hidden" id="e" name="e" value="${requestScope.inviteeEmail}" />
				<input type="hidden" id="fromUrl" name="fromUrl"  value="${requestScope.fromUrl}" />
				<input type="hidden" id="code" name="code"  value="${requestScope.code}" />
				<input type="hidden" id="from" name="from"  value="${requestScope.from}" />
				<input type="hidden" id="appToken" name="appToken"  value="${requestScope.appToken}" />
				<input type="hidden" id="groupName" name="groupName"  value="${requestScope.groupName}" />
				<input type="hidden" id="ss" name="ss"  value="${requestScope.ss}" />
				<input type="hidden" id="ayid" name="ayid" value="${requestScope.ayid}" />
				<input type="hidden" id="iid" name="iid"  value="${requestScope.iid}" /><input type="hidden" name="domain" value="kaixin.com" />
				<input type="hidden" name="origURL" value="http://home.kaixin.com/Home.do" />
				<input type="hidden" name="g" id="g" value="${requestScope.g}" />
				<input type="hidden" name="b" id="b" value="${requestScope.b}" />					
				<p><label for="acc">帐号:</label><input type="text" id="email" name="email" rule="blank#loginName" class="w150" tabindex="12"/></p>
				<p><label for="key">密码:</label><input type="password" id="password" name="password" class="w150" tabindex="13" /></p>
				<p class="enter"><input type="image" src="${applicationScope.urlStatic}/kaixin/imgpro/login/loginbtn.png" onclick="$('loginForm').submit();" tabindex="14"/></p>
			</form>
			</div>
            <!---middiv start--->
            <div class="kaixin_reg_info">
                <div class="kaixin_reg_infoclass" style="border-bottom:none">
				<form action="/rgac.do" id="regform" name="regForm" method="post" class="reg_layout">
					<input type="hidden" name="action_id" id="action_id" value="207107" />
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
									<td colspan="3" class="colspan colspan_1">
										<div class="sharemsg_wrap">
											<a id="reg_sub2" class="microReg" onclick="$('msnValid2').value='1';check1.submit();return false;" href="#microReg" tabindex="9">使用微软认证</a> 
											点击以上按钮，我们将认可您已阅读并同意遵守<a href="${applicationScope.urlMain}/info/agreement.jsp" target="_blank" tabindex="10">服务条款</a>。<br />
											<input type="checkbox" checked="checked" name="sharemsg" id="sharemsg" value="ok" tabindex="11"/>
												你可以将这个消息分享给更多的msn好友，
												并可能出现您的 Windows Live Id 在此自动登陆的情况。
										</div>
									</td>
								</tr>
							</table>
						</fieldset>
						<input type="hidden" name="regFlag" id="regFlag" value="invite_reg_dog_anticheat" />
					</form>
                </div>
            </div>            
       	</div>         
    </div>
</div><!--container END-->
<c:if test="${requestScope.ss == '17001' && requestScope.crand < 90}">
<!--3610/注册开始页面-->
<!--AdForward I-allyes Begin-->
<script language='JavaScript'>
document.write('<scr'+'ipt language=javascript src=http://smarttrade.allyes.com/main/adftrack?db=smarttrade&point=3610&js=on&cache='+(new Date()).getTime()+'&pre='+escape(document.referrer)+'><\/s'+'cript>');
</script>
<script language="javascript" src="http://mscdn.allyes.cn/sm/rt/js/k1.js"></script>
<!--AdForward I-allyes End-->
</c:if>
<%@ include file="inc/set_form.inc.jsp" %>
</body>
</html>