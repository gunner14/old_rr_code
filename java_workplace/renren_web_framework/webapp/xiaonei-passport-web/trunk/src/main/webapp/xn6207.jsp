<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <c:set var="INC_COMMON_CSS" value="true" />
        <c:set var="INC_NO_JEBE" value="true" />
        <link href="${applicationScope.urlSmallStatic}/csspro/apps/login-unbuffered.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
        <%@ include file="/inc/headpro.inc" %>
        <title>人人网 人人-登录首页</title>
        <script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/register/register-app-invite.js?ver=$revxxx$"></script>
</head>
<body id="welcomepage" class="login"> 
    <div id='univs'></div>
    <%@ include file="/inc/header-noad.inc" %>
    <div id="opi" class="page-wrapper clearfix">
        <div class="full-page-holder">
            <div class="full-page">
                <div class="login-page clearfix">
                    <div class="side-column">
                        <div class="login-panel">
                            <div id="errorMessage" class="errors_div" style="display:none;"></div>
                            <form method="post" id="loginForm" class="login-form" action="${applicationScope.urlPassport}/PLogin.do" >
                                <p class="top clearfix">
                                <label for="email">帐号:</label>
                                <input type="text" name="email"  class="input-text" value="" id="email" tabindex="1" />
                                </p>
                                <p class="clearfix">
                                <label for="password">密码:</label>
                                <input type="password" id="password" name="password"  class="input-text" tabindex="2"  />
                                </p>
                                <div class="caps-lock-tips" id="capsLockMessage" style="display:none"></div>
                                <p class="savepassword">
                                <label title="为了确保您的信息安全，请不要在网吧或者公共机房勾选此项！" for="autoLogin" class="labelCheckbox"><input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="3" />下次自动登录</label>
                                </p>
                                <p class="bottom">
									<input type="hidden" name="origURL" value="http://www.renren.com/SysHome.do" />
									<input type="hidden" name="domain" value="renren.com" />
									<input type="submit" id="login" class="input-submit" value="登录" tabindex="4"  />
									<span class="getpassword"><a href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword">忘记密码？</a></span>
								</p>
                            </form>
                        </div>
                        <div class="extra-guide">
                        	<% String[] playTogetherUrls = {"http://apps.renren.com/rr_farm","http://apps.renren.com/rr_cook"};
								String playTogetherUrl;
								try{
									playTogetherUrl = playTogetherUrls[(int) (Math.random()*2)];
								}catch(Exception e){
									playTogetherUrl = "http://apps.renren.com/rr_farm?ex";
								}
							%>
							<%--
                        	<div onmouseout="$(this).delClass('hover')" onmouseover="$(this).addClass('hover')" class="video ">
									<a href="<%=playTogetherUrl %>" class="title">上人人网 玩在一起，心在一起</a>
									<div class="imgholder">
										<a href="http://page.renren.com/media/video/singleVideo?pid=600002246&vid=595">
											<img width="197" height="147" src="${applicationScope.urlStatic}/imgpro/login/playtogether-s-v3.jpg">
										</a>
									</div>
									<p class="play"><a href="<%=playTogetherUrl %>">我要玩</a></p>
							</div>
							--%>
							
							<div onmouseout="$(this).delClass('hover')" onmouseover="$(this).addClass('hover')" class="video ">
									<a href="http://wwv.renren.com/xn.do?ss=10147&rt=1&fs=reg_10xinsheng_renren" class="title">2010级新生注册专用通道</a>
									<div class="imgholder">
										<a href="http://wwv.renren.com/xn.do?ss=10147&rt=1&fs=reg_10xinsheng_renren">
											<img width="197" height="147" src="${applicationScope.urlStatic}/imgpro/login/xinsheng-s.jpg">
										</a>
									</div>
									<p class="play"><a href="http://wwv.renren.com/xn.do?ss=10147&rt=1&fs=reg_10xinsheng_renren">新生报到</a></p>
							</div>
							
							
							<div id="open-search" class="open-search clearfix">
                                	<div>
                                    	<form id="oos" name="oos" method="post" action="${applicationScope.urlBrowse}/searchEx.do">
                                        	<div class="search-input">
												<input id="friendname" name="q" type="text" class="input-text" />
												<script type="text/javascript">
												    XN.form.help('friendname').setDefaultValue('寻找你的朋友...');
												</script>
                                                <div class="search-submit">
													<a href="#nogo" onclick="if($('oos')['q'].value == '寻找你的朋友...') {return;}else $('oos').submit()"  stats="home_searchfriend">&nbsp;</a>
                                                </div>
                                            </div>
                                        </form>
                                    </div>
                                    <div class="description">
                                        <a class="find-friends" href="${applicationScope.urlInvite}/unrgsfidfrd.do" stats="home_findfriendreg">看看哪些朋友注册了${domainIdentified["siteName"]}网</a>
                                    </div>
                                </div>
                        </div>
						<div onmouseout="$(this).delClass('hover')" onmouseover="$(this).addClass('hover')" class="mobile ">
								<p>
									<img class="mobile-url" src="${applicationScope.urlStatic}/a.gif">
									<span>手机登录</span>
									<a stats="home_mobilelogin" target="_blank" href="http://mobile.renren.com/mobilelink.do?psf=8000201">http://3g.renren.com</a>
								</p>
								<p>
									<img class="mobile-app" src="${applicationScope.urlStatic}/a.gif">
									<a href="http://mobile.renren.com/showClient?psf=41004">下载手机客户端</a>
									<span class="app-description">省时间，省流量，多积分</span>
								</p>
						</div>
							<%
								int adsDesktopCount=2;
								switch(new java.util.Random().nextInt(adsDesktopCount)){
									case 0:
							%>	
							<div class="rrdesk clearfix " id="rrdesk">
								<a href="http://im.renren.com/?subver=8&amp;word01"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif"></img></a>
								<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-2.0-8.exe?word01" >人人桌面&nbsp;立即下载</a>
								<p>小窗查看新鲜事&nbsp;不留脚印</p>
								<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word01"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif"></img></a>
								<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word01" >下载游戏助手</a>&nbsp;<span>多帐号挂机</span>
							</div>
							<%				
								break;					
									case 1:
							%>
							<div class="rrdesk clearfix " id="rrdesk">
								<a href="http://im.renren.com/?subver=8&amp;word02"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif"></img></a>
								<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-2.0-8.exe?word02" >人人桌面&nbsp;立即下载</a>
								<p>不开网页&nbsp;快速回复</p>
								<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word02"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif"></img></a>
								<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word02" >下载游戏助手</a>&nbsp;<span>多帐号登录</span>
							</div>
							<%
									break;
								default:
									break;
								}	
							%>
                        <script type="text/javascript">
							function addhovereffect(){
								var addlist = new Array("open-search","rrdesk");
								for(var i=0;i<addlist.length;i++){
									$(addlist[i]).addEvent('mouseover',function(i){return function(){
											$(addlist[i]).addClass('hover');
										}}(i));
									$(addlist[i]).addEvent('mouseout',function(i){return function(){
											$(addlist[i]).delClass('hover');
										}}(i));
								}
							}
							addhovereffect()
						</script>
                    </div>
                    <div class="main-column">
                        <div class="shadow" id="content">
                            <div class="reg-holder">
                                <div id="reg_headlogo" class="clearfix">
                                    <p id="headtext">
                                    <span>人人网，中国<strong>最真实、最有效</strong>的社交平台，加入人人网，找回老朋友，结交新朋友。</span>
                                    </p>
                                </div>
                                <div id="reg_contentbox" class="form_wrap">
                                    <!-- regForm -->
                                    <form name="regShortCommonForm" id="regform" method="post" action="http://reg.renren.com/s-c-reg.do" class="main_regform">
                                        <input type="hidden" name="fromUrl" value="<%=request.getAttribute("f") %>" id="fromUrl">
                                        <input type="hidden" name="iu" value="" id="iu">
                                        <input type="hidden" name="ic" value="" id="ic">
                                        <input type="hidden" name="uuid" value="" id="uuid">
                                        <input type="hidden" name="ss" value="10112" id="ss">
                                        <input type="hidden" name="action_id" value="206207" id="action_id">

                                        <input type="hidden" name="accType" value="1" id="accType">
                                        <input type="hidden" name="g" value="" id="g">
                                        <input type="hidden" name="b" value="" id="b">
                                        <div class="form_row" id="d_email">
                                            <label id="regmail_label" for="regEmail">电子邮箱:</label>
                                            <div class="input_wrap">
                                                <input type="text" name="regEmail" value="" id="regEmail" class="inputtext">
                                                <span id="dmmg" style="display:none;"></span>
                                                <div class="box-error" id="regEmail_error_span"></div>
						<%--<span class="noqqalt">推荐使用<a target="_blank" href="http://wwv.renren.com/xn.do?ss=10622&rt=1">msn邮箱</a>，注册成功率高</span> --%>
                                            </div>
                                        </div>
                                        <div class="form_row" id="p_pwd">
                                            <label for="pwd">设置密码:</label>
                                            <div class="input_wrap">
                                                <input type="password" name="pwd" maxlength="20" value="" id="pwd" class="inputtext">
                                                <span id="mmg" style="display:none;"></span>
                                                <div class="box-error" id="pwd_error_span"></div>
                                            </div>
                                        </div>
                                        <div class="form_row" id="p_name">
                                            <label for="name">真实姓名:</label>
                                            <div class="input_wrap">
                                                <input type="text" name="name" value="" id="name" class="inputtext">
                                                <span id="xmg" style="display:none;"></span>
                                                <div class="box-error" id="name_error_span"></div>
                                            </div>
                                        </div>
                                        <div class="form_row" id="p_gender">
                                            <label>性别:</label>
                                            <div class="input_wrap">
                                                <label class="labelRadio"><input type="radio" name="gender" value="男生" id="gender">男</label>
                                                <label class="labelRadio"><input type="radio" name="gender" value="女生" id="gender">女</label>
                                                <div class="box-error" id="gender_error_span"></div>
                                            </div>
                                        </div>
                                        <div class="form_row" id="p_birthday">
                                            <label>生日:</label>
                                            <div class="input_wrap">
                                                <label for="birth_year">
                                                    <select name="birth_year"><option value="">请选择</option>
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
														<option value="1900">1900</option></select> 年
                                                </label>
                                                <label for="birth_month">
                                                    <select name="birth_month"><option value="">--</option>
                                                        <option value="12">12</option>
                                                        <option value="11">11</option>
                                                        <option value="10">10</option>
                                                        <option value="9">9</option>
                                                        <option value="8">8</option>
                                                        <option value="7">7</option>
                                                        <option value="6">6</option>
                                                        <option value="5">5</option>
                                                        <option value="4">4</option>
                                                        <option value="3">3</option>
                                                        <option value="2">2</option>
                                                        <option value="1">1</option></select> 月
                                                </label>
                                                <label for="birth_day">
                                                    <select name="birth_day"><option value="">--</option>
                                                        <option value="31">31</option>
                                                        <option value="30">30</option>
                                                        <option value="29">29</option>
                                                        <option value="28">28</option>
                                                        <option value="27">27</option>
                                                        <option value="26">26</option>
                                                        <option value="25">25</option>
                                                        <option value="24">24</option>
                                                        <option value="23">23</option>
                                                        <option value="22">22</option>
                                                        <option value="21">21</option>
                                                        <option value="19">19</option>
                                                        <option value="18">18</option>
                                                        <option value="17">17</option>
                                                        <option value="16">16</option>
                                                        <option value="15">15</option>
                                                        <option value="14">14</option>
                                                        <option value="13">13</option>
                                                        <option value="12">12</option>
                                                        <option value="11">11</option>
                                                        <option value="10">10</option>
                                                        <option value="9">9</option>
                                                        <option value="8">8</option>
                                                        <option value="7">7</option>
                                                        <option value="6">6</option>
                                                        <option value="5">5</option>
                                                        <option value="4">4</option>
                                                        <option value="3">3</option>
                                                        <option value="2">2</option>
                                                        <option value="1">1</option></select> 日
                                                </label>
                                                <div class="box-error" id="birthday_error_span"></div>
                                                <p class="b_tip"><a href="#birthday" class="birthday_link" onclick="XN.DO.confirm('出于安全考虑，同时为了保证人人网用户的真实性，凡注册人人网用户都需要填写真实的出生日期。<br /><span class=\'gray\'>你可以在个人主页上隐藏你的出生日期。</span>', '为什么需要提供我的生日日期？')">为什么要填写我的生日？</a></p>
                                            </div>
                                        </div>
                                        <%
                                        int key_id = new java.util.Random().nextInt();
                                        %>
                                        <div class="form_row icode_row">
                                            <label class="empty">验证码:</label>
                                            <div class="input_wrap">
                                                <img class="validate-num" id="verifyPic" src="http://icode.renren.com/getcode.do?t=register_<%=key_id%>"/>
                                                <a href="javascript:refreshCode();">看不清换一张?</a></span>
                                        </div>
                                    </div>


                                    <div class="form_row icode_input_row" id="p_icode">
                                        <label for="icode">验证码:</label>
                                        <div class="input_wrap">
                                            <input type="text" name="icode" id="icode" class="inputtext validate-code" size="5" maxlength="5" />									
                                            <input type="hidden" name="key_id" value="<%=key_id%>" /> 
                                            <div class="box-error" id="icode_error_span"></div>
                                        </div>
                                    </div>
 
                                    <div class="form_row">
                                        <input type="hidden" name="activity" value="" id="activity" />
                                        <input type="hidden" name="iv" value="" id="iv" />
                                        <input type="hidden" name="from" value="" id="from" />
                                        <div id="dialog_buttons" class="dialog_buttons">
                                            <input type="submit" id="button_submit" class="inputsubmit" value="" />
                                        </div>
                                    </div>

                                </form>
                                <script type="text/javascript">
                                    new RegCheck();
                                    </script>

                                    <!-- regForm -->	
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
<div class="hidden-area"></div>
<script>
    var url = 'http://reg.renren.com/xn6207.do';
    if (!this.x) this.x  = new X();
    this.x.setVars({
    'ss': '10112'
    }).send(url);
</script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.page.syshome.js?ver=$revxxx$"></script>
<script type="text/javascript">
    <!--
    XN.DOM.ready(function(){
            var errorBoxes = XN.DOM.getElementsByClassName('box-error');
            for (var i = 0; i < errorBoxes.length; i++) {
            errorBox = errorBoxes[i];
            errorBox.innerHTML = '<div class="box-error-outer">'+'<div class="box-error-inner">'+'<p id="'+errorBox.id+'_i"></p>'+'</div>'+'</div>';
            }
            });
-->
</script>
<%@ include file="/inc/footer-noad.inc" %>
    </body>
</html>
