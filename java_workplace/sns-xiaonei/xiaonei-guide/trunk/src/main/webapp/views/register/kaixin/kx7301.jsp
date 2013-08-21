<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
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
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/renren.pub.reg.js?ver=$revxxx$"></script>

<script language="javascript">
	function refreshCode() {
		$('verifyPic').src = $('verifyPic').src + Math.random();
	}
</script>
<style>.inputholder{width:240px!important}
.kaixin_friends{ border:1px solid #e9e9e9; padding-top:11px; width:246px;}
.msntab .content .reg_layout{height:auto}
.msntab .content .reg_layout fieldset { width:520px;}
</style>
</head>
<body class="reg new_inv_mail">
<div id="container" class="msntab">
	<div class="mainbox">
    <%@ include file = "inc/reg_navigate_bar.inc" %>
    <div id="widebar">
       	<div class="content"> 
        <!---leftdiv_login start by henry--->
        <div class="kaixin_loginarea">&nbsp;
       		<div class="kaixin_login">
				<form method="post" id="loginForm" action="${applicationScope.urlPassport}/PLogin.do" >
					<input type="hidden" id="loginregFrom" name="loginregFrom"  value="defautl_reg" />
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
					<input type="hidden" id="iid" name="iid"  value="${requestScope.iid}" /><input type="hidden" name="domain" value="kaixin.com" />
					<input type="hidden" name="g" id="g" value="${requestScope.g}" />
					<input type="hidden" name="b" id="b" value="${requestScope.b}" />						
                <h2>登录开心网</h2>
                <div class="login_textinput">
                <label>帐号:</label>
						<input type="text" name="email" rule="blank#loginName" class="input-text" value="" id="email" tabindex="16" style="width:105px;"/>
					</div>
					<div class="login_textinput">
						<label>密码:</label>
						<input type="password" id="password" name="password" rule="blank" error="请输入密码" class="input-text" tabindex="17" style="width:105px;" />
						<input type="hidden" id="login_type" name="login_type"  value="1003" />
					</div>
					<p class="logbtn">
						<input type="hidden" name="origURL" value="http://home.kaixin.com/Home.do" />
						<button type="submit" class="input-button low-padding" id="login" tabindex="18" />登录</button>&nbsp;&nbsp;<span class="dis-findpwd findpwd"><a href='${applicationScope.urlMain}/GetPasswd.do' tabindex="19">找回密码</a></span>
					</p>
				</form>
				</div>	
            </div>
            <!---middiv start--->
            <div class="kaixin_reg_info">
            	<%@ include file="/inc/errors.inc" %>            
                <div class="top_info">
					<div class="noinv_message">
						<h2>完善你的个人信息，加入开心网</h2>
                    	<p>开心网是帮助你与朋友、同事、同学、家人保持更紧密联系的真实社交平台，在这里，你可以及时了解他们的最新动态；分享你的照片、心情、快乐；结识更多的新朋友</p>					
					</div>
                </div>                
                <div class="kaixin_reg_infoclass" style="border-bottom:none">
                <form action="/register.do" class="reg_layout" id="regform" name="regForm" method="post">
				<input type="hidden" name="fromUrl" id="fromUrl" />
				<input type="hidden" name="ss" id="ss" value="${ss}"/>
				<input type="hidden" name="rt" id="rt" value="${rt}"/>
				<input type="hidden" name="b" id="b" value="${b}"/>
				<input type="hidden" name="appId" id="appId" value="${appId}"/>
				<input type="hidden" name="inviterId" id="inviterId" value="${inviterId}"/>
				<input type="hidden" name="action_id" id="action_id" value="207301" />
				<input type="hidden" name="g" id="g" value="${requestScope.g}" />
				<input type="hidden" name="b" id="b" value="${requestScope.b}" />
				<%@ include file="inc/jsanti.inc" %>
					<fieldset class="account-field">
						<legend>账户信息</legend>
						<table>
							<tr>
								<th><%@ include file="inc/account.var" %></th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="text" name="regEmail" id="regEmail" tabindex="1" />
									<%@ include file="inc/reg_email_recomm.inc" %>
								</td>
								<td>
									<div class="zc_cwts hide" id="regEmail_ok" style="display:none;">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
									<div id="regEmail_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr>
								<th>你的密码:</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="password" name="pwd" id="pwd" tabindex="2" onblur="this.form.repwd.value=this.value;"/>
									<input type="hidden" id="repwd" name="repwd" />
								</td>
								<td>
									<div class="zc_cwts hide" id="pwd_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
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
									<div class="zc_cwts hide" id="repwd_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
									<div id="repwd_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr-->
						</table>
						
					</fieldset>
					<fieldset>
						<legend>详细信息</legend>
						<table>
							<tr>
								<th>你的姓名:</th>
								<td class="inputholder">
									<span class="required">*</span>
									<input type="text" name="name" id="name" tabindex="3"  style="width:200px;" />
								</td>
								<td>
									<div class="zc_cwts hide" id="name_ok">
									&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" />
									</div>
									<div id="name_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr>
								<th>性别:</th>
								<td class="clearfix">
									<span class="required">*</span>
									<label for="male" class="reg_radio"><input type="radio" name="gender" value="男生"  tabindex="4"/>男</label>
									<label for="woman" class="reg_radio"><input type="radio" name="gender" value="女生" tabindex="5"/>女</label>
								</td>
								<td>
									<div class="zc_cwts hide" id="gender_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
									<div id="gender_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
								</td>
							</tr>
							<tr>
								<th>出生日期:</th>
								<td class="inputholder">
									<span class="required">*</span>
									<select name="birth_year" id="birth_year" style="width:55px;" tabindex="6">
									  <option value=""></option>
									  <option value="2012">2012</option>
									  <option value="2011">2011</option>
									  <option value="2010">2010</option>
									  <option value="2009">2009</option>
									  <option value="2008">2008</option>
									  <option value="2007">2007</option>
									  <option value="2006">2006</option>
									  <option value="2005">2005</option>
									  <option value="2004">2004</option>
									  <option value="2003">2003</option>
									  <option value="2002">2002</option>
									  <option value="2001">2001</option>
									  <option value="2000">2000</option>
									  <option value="1999">1999</option>
									  <option value="1998">1998</option>
									  <option value="1997">1997</option>
									  <option value="1996">1996</option>
									  <option value="1995">1995</option>
									  <option value="1994">1994</option>
									  <option value="1993">1993</option>
									  <option value="1992">1992</option>
									  <option value="1991">1991</option>
									  <option value="1990">1990</option>
									  <option value="1790">90后</option>
									  <option value="1989">1989</option>
									  <option value="1988">1988</option>
									  <option value="1987">1987</option>
									  <option value="1986">1986</option>
									  <option value="1985">1985</option>
									  <option value="1984">1984</option>
									  <option value="1983">1983</option>
									  <option value="1982">1982</option>
									  <option value="1981">1981</option>
									  <option value="1980">1980</option>
									  <option value="1780">80后</option>
									  <option value="1979">1979</option>
									  <option value="1978">1978</option>
									  <option value="1977">1977</option>
									  <option value="1976">1976</option>
									  <option value="1975">1975</option>
									  <option value="1974">1974</option>
									  <option value="1973">1973</option>
									  <option value="1972">1972</option>
									  <option value="1971">1971</option>
									  <option value="1970">1970</option>
									  <option value="1770">70后</option>
									  <option value="1969">1969</option>
									  <option value="1968">1968</option>
									  <option value="1967">1967</option>
									  <option value="1966">1966</option>
									  <option value="1965">1965</option>
									  <option value="1964">1964</option>
									  <option value="1963">1963</option>
									  <option value="1962">1962</option>
									  <option value="1961">1961</option>
									  <option value="1960">1960</option>
									  <option value="1760">60后</option>
									  <option value="1959">1959</option>
									  <option value="1958">1958</option>
									  <option value="1957">1957</option>
									  <option value="1956">1956</option>
									  <option value="1955">1955</option>
									  <option value="1954">1954</option>
									  <option value="1953">1953</option>
									  <option value="1952">1952</option>
									  <option value="1951">1951</option>
									  <option value="1950">1950</option>
									  <option value="1949">1949</option>
									  <option value="1948">1948</option>
									  <option value="1947">1947</option>
									  <option value="1946">1946</option>
									  <option value="1945">1945</option>
									  <option value="1944">1944</option>
									  <option value="1943">1943</option>
									  <option value="1942">1942</option>
									  <option value="1941">1941</option>
									  <option value="1940">1940</option>
									  <option value="1939">1939</option>
									  <option value="1938">1938</option>
									  <option value="1937">1937</option>
									  <option value="1936">1936</option>
									  <option value="1935">1935</option>
									  <option value="1934">1934</option>
									  <option value="1933">1933</option>
									  <option value="1932">1932</option>
									  <option value="1931">1931</option>
									  <option value="1930">1930</option>
									  <option value="1929">1929</option>
									  <option value="1928">1928</option>
									  <option value="1927">1927</option>
									  <option value="1926">1926</option>
									  <option value="1925">1925</option>
									  <option value="1924">1924</option>
									  <option value="1923">1923</option>
									  <option value="1922">1922</option>
									  <option value="1921">1921</option>
									  <option value="1920">1920</option>
									  <option value="1919">1919</option>
									  <option value="1918">1918</option>
									  <option value="1917">1917</option>
									  <option value="1916">1916</option>
									  <option value="1915">1915</option>
									  <option value="1914">1914</option>
									  <option value="1913">1913</option>
									  <option value="1912">1912</option>
									  <option value="1911">1911</option>
									  <option value="1910">1910</option>
									  <option value="1909">1909</option>
									  <option value="1908">1908</option>
									  <option value="1907">1907</option>
									  <option value="1906">1906</option>
									  <option value="1905">1905</option>
									  <option value="1904">1904</option>
									  <option value="1903">1903</option>
									  <option value="1902">1902</option>
									  <option value="1901">1901</option>
									  <option value="1900">1900</option>
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
									</select>日
								</td>
								<td>
									<div id="birth_year_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
									<div id="birth_month_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
									<div id="birth_day_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
									<div class="zc_cwts hide" id="birth_day_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
								</td>
							</tr>
							<tr>
								<th>居住城市:</th>
								<td class="inputholder">
									<span class="required">*</span>
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
								<td>
									<div id="homeprovince_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
									<div id="homecitycode_err" class="zc_cwts hide"><img height="24" width="18" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div>
									<div class="zc_cwts hide" id="homecitycode_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
								</td>
							</tr>							
						</table>
					</fieldset>
					<fieldset style="border-bottom:none">
						<legend>验证程序</legend>
						<table>
							<tr>
								<th>验证码:</th>
								<td class="inputholder coder">
									<p class="float-left coder_input">
										<span class="required">*</span>
										<input type="text" name="icode" id="icode" size="5" maxlength="5" tabindex="11"/>&nbsp;&nbsp;
									</p>
									<p class="float-left coder_img">
									<%
										java.util.Random random = new java.util.Random();
										int id = random.nextInt(1431655764);
									%>
										<input type="hidden" id="key_id" name="key_id" value="<%=id%>"/> 
										<img id="verifyPic" src="${applicationScope.urlIcode}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/>
										<a href="javascript:refreshCode();" onclick="javascript:void(0);" tabindex="12">看不清楚 换一张</a>
									</p>
								</td>
								<td>
									<div class="zc_cwts hide" id="icode_ok">&nbsp;<img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/fzcg_dh.gif" width="17" height="13" /></div>
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
									<button id="reg_sub" type="button" onclick="check2.submit();" style="cursor:pointer;float:left" class="notinv" tabindex="13">注册帐号</button>
									<a id="reg_sub2" class="microReg" style="color: rgb(128, 128, 128);line-height:34px" onclick="$('msnValid2').value='1';check2.submit();return false;" href="#" tabindex="14">使用微软认证</a>
								</td>
							</tr>
							<tr style="display:none">
									<td class="colspan" colspan="2"><input type="checkbox" checked="checked" name="accept" id="accept"/></td>
									<td><div class="zc_cwts hide" id="accept_err"><img width="18" height="24" src="${applicationScope.urlStatic}/kaixin/imgpro/reg/th_ju.gif"/></div></td>
								</tr>
							<tr>
								<td colspan="3" class="colspan">
								<input type="checkbox" checked="checked" name="sharemsg" id="sharemsg" value="ok" tabindex="15"/>
									你可以将这个消息分享给更多的msn好友<br/>
									点击以上按钮，我们将认可您已阅读并同意遵守 <a href="${applicationScope.urlMain}/info/agreement.jsp" target="_blank" tabindex="16">服务条款</a>
									<!--<br>并可能出现您的 Windows Live Id 在此自动登陆的情况。-->
									</td>
							</tr>
						</table>
					</fieldset>
					<input type="hidden" name="regFlag" id="regFlag" value="public_reg7301" />
				</form>
                </div>
            </div><!--single-column END-->  <!---middiv end--->
            <!---rightdiv start--->

            
            <!---rightdiv end--->
            
       	</div><!--content END-->
         
    </div><!--widebar end-->

</div><!--container END-->
</div>
<div id="footer">
	<div class="blank-holder"></div>
		<div class="copyright">			
			<span>开心网 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2010</span>
		</div>
</div><!--footer END-->
<%@ include file="inc/set_form.inc.jsp" %>
</body>
</html>
