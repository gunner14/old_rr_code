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
	<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlStatic}/kaixin/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlStatic}/kaixin/csspro/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<title>开心网</title>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/js/fillinfo.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/js/inCityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/renren.msn.reg.js?ver=$revxxx$"></script>
<script language="javascript">
	function refreshCode() {
		$('verifyPic').src = $('verifyPic').src + Math.random();
	}
</script>
<style type="text/css">
.inputholder{width:240px!important;}
.kaixin_friends{border:1px solid #e9e9e9;padding-top:10px;width:246px;}
.content{background:url(${applicationScope.urlStatic}/kaixin/imgpro/reg/test.jpg) no-repeat;height:980px;position:relative;}
.kaixin_reg_info{position:absolute;top:205px;right:170px;_right:126px;}
.form-tip{width:180px;}
.reg_layout{width:490px;margin:0;}
.login-top{position:absolute;top:92px;right:128px;}
.login-top p{margin:0 0 5px;_margin:0 0 2px;}
.login-top p .w150{width:150px;}
.login-top p label{font-weight:400;margin:0 7px 0 0;}
.login-top .enter{text-align:center;margin:5px 0 0;_margin:3px 0 0;}
</style>
</head>
<body>
<%
out.println(request.getAttribute("randuid"));
%>
	<div class="mainbox">
    <div id="widebar">
       	<div class="content"> 
			<div class="login-top">
				<form method="post" id="loginForm" action="http://www.renren.com/PLogin.do" >
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
				<input type="hidden" name="origURL" value="http://www.kaixin.com/Home.do" />
				<input type="hidden" name="g" id="g" value="${requestScope.g}" />
				<input type="hidden" name="b" id="b" value="${requestScope.b}" />		
				
				<p><label for="acc">账户:</label><input type="text" id="email" name="email" rule="blank#loginName" class="w150" tabindex="17"/></p>
				<p><label for="key">密码:</label><input type="password" id="password" name="password" class="w150"  tabindex="18"/></p>
				<p class="enter"><input type="image" src="${applicationScope.urlStatic}/kaixin/imgpro/login/loginbtn.png" onclick="$('loginForm').submit();" tabindex="19"/></p>
			</form>
			</div>

            <!---middiv start--->
            <div class="kaixin_reg_info">
                <div class="kaixin_reg_infoclass" style="border-bottom:none">
				<%@ include file="/inc/errors.inc" %>
                <form action="/registerdog.do" id="regform" name="regForm" method="post" class="reg_layout">
					<input type="hidden" name="i" value="700001678" />
					<input type="hidden" name="ayid" value="${requestScope.ayid}" />
					<input type="hidden" name="e" value="${requestScope.inviteeEmail}" />
					<input type="hidden" name="fromUrl" id="fromUrl" />
					<input type="hidden" name="appId" id="appId" />
					<input type="hidden" name="inviterId" id="inviterId" value="700001678" />
					<input type="hidden" name="inviteType" id="inviteType" value="2" />
					<input type="hidden" name="code" id="code" value="AFKTrbBj2H"/>
					<input type="hidden" name="from" id="from" />
					<input type="hidden" name="inviteeEmail" id="inviteeEmail" />
					<input type="hidden" name="appToken" id="appToken" />
					<input type="hidden" name="groupName" id="groupName" />
					<input type="hidden" name="ss" id="ss" />
					<input type="hidden" name="action_id" id="action_id" value="209999" />
					<input type="hidden" name="g" id="g" value="${requestScope.g}" />
					<input type="hidden" name="b" id="b" value="${requestScope.b}" />
					<%@ include file="inc/jsanti.inc" %>
						<fieldset class="account-field">
							<legend>账户信息</legend>
							<table>
								<tr>
									<th>你的帐号Email:</th>
									<td class="inputholder"><span class="required">*</span>
										<input type="text" name="regEmail" id="regEmail" tabindex="1" />
										<%@ include file="inc/reg_email_recomm.inc" %>
									</td>
									<td><div style="display: none;" id="regEmail_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="regEmail_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								<tr>
									<th>你的密码:</th>									
									<td class="inputholder"><span class="required">*</span>
										<input type="password" name="pwd" id="pwd" tabindex="2" /></td>

									<td><div id="pwd_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="pwd_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								
							</table>
						</fieldset>
						<fieldset>
							<legend>详细信息</legend>
							<table>
								<tr>
									<th>你的姓名:</th>
									<td class="inputholder"><span class="required">*</span>
										<input type="text" name="name" id="name" tabindex="3" /></td>
									<td><div id="name_ok" class="zc_cwts hide">  <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="name_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								<tr>
									<th>性别:</th>
									<td class="clearfix"><span class="required">*</span>
										<label for="male" class="reg_radio"><input type="radio" name="gender" value="男生" tabindex="4"/>男</label>										
										<label for="woman"  class="reg_radio"><input type="radio" name="gender" value="女生"  tabindex="5"/>女</label></td>

									<td><div id="gender_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="gender_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								<tr>
									<th>出生日期:</th>
									<td class="inputholder"><span class="required">*</span>
										<select name="birth_year" id="birth_year" style="width:55px;" tabindex="6">
									  <option value=""></option>
									  <option value="1790">90后</option>
									  <option value="1780">80后</option>
									  <option value="1770">70后</option>
									  <option value="1760">60后</option>
									</select>年
									<select name="birth_month" id="birth_month" style="width:55px;" tabindex="7">
										<option value=""></option>
										<option value="01">01</option>
										<option value="02">02</option>
										<option value="03">03</option>
										<option value="04">04</option>
										<option value="05">05</option>
										<option value="06">06</option>
										<option value="07">07</option>
										<option value="08">08</option>
										<option value="09">09</option>
										<option value="10">10</option>
										<option value="11">11</option>
										<option value="12">12</option>
									</select>月
									<select name="birth_day" id="birth_day" style="width:55px;" tabindex="8">
									    <option value=""></option>
										<option value="01">01</option>
										<option value="02">02</option>
										<option value="03">03</option>
										<option value="04">04</option>
										<option value="05">05</option>
										<option value="06">06</option>
										<option value="07">07</option>
										<option value="08">08</option>
										<option value="09">09</option>
										<option value="10">10</option>
										<option value="11">11</option>
										<option value="12">12</option>
										<option value="13">13</option>
										<option value="14">14</option>
										<option value="15">15</option>
										<option value="16">16</option>
										<option value="17">17</option>
										<option value="18">18</option>
										<option value="19">19</option>
										<option value="20">20</option>
										<option value="21">21</option>
										<option value="22">22</option>
										<option value="23">23</option>
										<option value="24">24</option>
										<option value="25">25</option>
										<option value="26">26</option>
										<option value="27">27</option>
										<option value="28">28</option>
										<option value="29">29</option>
										<option value="30">30</option>
										<option value="31">31</option>
									</select>
										日 </td>
									<td><div class="zc_cwts hide" id="birth_year_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
										<div class="zc_cwts hide" id="birth_month_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
										<div class="zc_cwts hide" id="birth_day_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
										<div id="birth_day_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div></td>
								</tr>
								<tr>
									<th>居住城市:</th>

									<td class="inputholder"><span class="required">*</span>
										<select name="homeprovince"
										id="homeprovince" class="select" tabindex="9"
										onchange="Profile.getInCityListOfProvince(this.selectedIndex, 'homecitycode')"
										>
										<%@ include file="inc/inProvince.jsp"%>
										</select>

										<input type="hidden" name="homecityname" id="homecityname" />
										
										<select
											name="homecitycode" id="homecitycode" class="select"
											tabindex="10"
											onblur="Profile.selectCity(this.selectedIndex, 'homecitycode', 'homecityname');"
											>
											<option value="">选择城市: </option>
										</select> 
										<script>
												Profile.getInCityListOfProvince($("homeprovince").selectedIndex, 'homecitycode');
										</script>
										<input type="hidden" name="workplacename" value=""/>
										<input type="hidden" name="stage" value="30"/>
										<input type="hidden" name="basicControl" value="99"/>
										</td>
									<td><div class="zc_cwts hide" id="homeprovince_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
										<div class="zc_cwts hide" id="homecitycode_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
										<div id="homecitycode_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div></td>
								</tr>								
							</table>
						</fieldset>
						<fieldset style="border-bottom:none">
							<legend>验证程序</legend>
							<table>
								<tr>
									<th>验证码:</th>
									<td class="inputholder coder"><p class="float-left coder_input"><span class="required">*</span>
											<input type="text" tabindex="11" maxlength="5" size="5" id="icode" name="icode"/>&nbsp;&nbsp;
										</p>

										<p class="float-left coder_img">
										<%
											java.util.Random random = new java.util.Random();
											int id = random.nextInt(1431655764);
										%>
										<input type="hidden" id="key_id" name="key_id" value="<%=id%>"/> 
										<img id="verifyPic" src="${applicationScope.urlIcode}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/>
										<a onclick="javascript:void(0);" href="javascript:refreshCode();" tabindex="12">看不清楚 换一张</a></p>
									</td>
									<td><div id="icode_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="icode_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
									</td>
								</tr>
								<tr style="display:none">
									<th></th>
									<td class="inputholder"><span class="required"></span>
										<input type="hidden" id="msnValid" name="msnValid" value="0"/>
										<input type="hidden" id="msnValid2" name="msnValid2" value="0"/>
										<input type="hidden" id="rt" name="rt" value="${requestScope.rt}"/>
										<input type="hidden" id="d" name="d" value="${requestScope.dogId}"/>
										<input type="text" id="ismsn" name="ismsn" value="abc"/></td>
									<td><div style="display: none;" id="ismsn_ok" class="zc_cwts hide"> <img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif"/></div>
										<div class="zc_cwts hide" id="ismsn_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								<tr>
									<td class="colspan" colspan="3">
									<div id="waiting" name="waiting" style="display:none">
                    					<img src="${applicationScope.urlStatic}/kaixin/imgpro/icons/indicator.gif" /> 正在提交数据，请稍候...
                					</div>
									<button style="cursor: pointer;" type="button" id="reg_sub" onclick="ismsnok();check1.submit();" tabindex="13">我已填写完毕</button><a href="#" onclick="$('msnValid2').value='1';check1.submit();return false;" style="color:#808080;" class="microReg" id="reg_sub2" tabindex="14">使用微软认证</a></td>
								</tr>
								<tr style="display:none">
									<td class="colspan" colspan="2"><input type="checkbox" checked="checked" name="accept" id="accept"/></td>
									<td><div class="zc_cwts hide" id="accept_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
								<tr>
									<td colspan="3" class="colspan">
										<input type="checkbox" checked="checked" name="sharemsg" id="sharemsg" value="ok" tabindex="15"/>
										你可以将这个消息分享给更多的msn好友<br/>点击以上按钮，我们将认可您已阅读并同意遵守 <a href="${applicationScope.urlMain}/info/agreement.jsp" target="_blank" tabindex="16">服务条款</a>
										<br>并可能出现您的 Windows Live Id 在此自动登陆的情况。
										</td>
								</tr>
							</table>
						</fieldset>
						<input type="hidden" name="regFlag" id="regFlag" value="invite_reg_dog2" />
					</form>
                </div>
            </div>            
       	</div>         
    </div>
</div><!--container END-->

<%@ include file="inc/set_form.inc.jsp" %>
</body>
</html>