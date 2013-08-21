<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<div id="sidebar">
	<div id="loginpanel">
		<h3>用户登录:</h3>
		<form method="post" id="loginForm"  action="${applicationScope.urlLogin}/Login.do" onsubmit="getEl('email').value=Trim(getEl('email').value);" focus="email">
			<p class="top">
				<label for="email">登录邮箱:</label>
				<input type="text" name="email" class="inputtext" value="${_de}" id="email" tabindex="1" onblur="checkE()"/>
				<div id="errordiv" class="errorformatalt" style="display:none;">
				<div class="errorformatalttriangle"><img src="${applicationScope.urlSmallStatic}/img/dialog_triangle_bright_yellow.gif" /></div>
					<strong name='ts' id='ts'></strong>
				</div>		
			</p>
			<p>
				<label for="password">密码:</label>
				<input type="password" id="password" name="password" class="inputtext" tabindex="2" onkeypress="capLock(event)"  />
				<div id="mmerrordiv" class="mmerrorformatalt" style="display:none;">
				<div class="errorformatalttriangle"><img src="${applicationScope.urlSmallStatic}/img/dialog_triangle_bright_yellow.gif" /></div>
					<strong name='ms' id='ms'></strong>
				</div>	
			</p>
			<p>
				<label for="autoLogin" class="labelCheckbox"><input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="3" />下次自动登录</label>
			</p>
			<p>
			  <input type="hidden" name="origURL" value="${requestScope.origURL}" />
				<input type="submit" id="login" name="submit" class="subbutton" value="登录" tabindex="4" onclick="setLoginEmail();" />
				<a href="${applicationScope.urlMain}/GetPasswd.do">找回密码</a>
			</p>
		</form>
		<a href="${applicationScope.urlReg}/register.do" class="portal"><strong>注册</strong>${domainIdentified["siteName"]}期待你的加入</a>
		<!-- a href="${applicationScope.urlReg}/register.do?g3=08" class="portal"><strong>高三通道</strong>即刻畅游xiaonei.com</a -->
		<a href="${applicationScope.urlProfile}/mobile/web.do" class="portal" target="_blank"><strong>手机登录</strong>请访问m.xiaonei.com</a>
		<a href="${applicationScope.urlMain}/info/Demo.do" class="portal"><strong>页面介绍</strong>先看看你的${domainIdentified["siteName"]}主页</a>
	</div>
</div>
<script language="javascript">
  if (!e) var e = window.event;
  var regE = /^[A-Z_a-z0-9-\.]+@([A-Z_a-z0-9-]+\.)+[a-z0-9A-Z]{2,3}$/;
  var regstr = /^[\w@_.-]{3,50}$/;
  var regAT = /·/;
  var regCM = /[@|.]/;
  var CM = /[。|,|，|、]/g;
  function checkE() {
	var email = document.getElementById('email').value;
	email = email.replace(regAT,"@");
	email = email.replace(CM,".");
	var reEmail="";
	 for (i=0 ; i<email.length; i++) { 	
  		code = email.charCodeAt(i);//获取当前字符的unicode编码
	 	if (code >= 65281 && code <= 65373){ //在这个unicode编码范围中的是所有的英文字母已经各种字符
	 		reEmail += String.fromCharCode(code - 65248);//把全角字符的unicode编码转换为对应半角字符的unicode码
	 	} else {
	 		reEmail += String.fromCharCode(code);
	 	}
	 }
	document.getElementById('email').value = reEmail;
	if(reEmail.length > 0) {
		if(!regstr.test(reEmail)){
			document.getElementById('errordiv').style.display='block';
			document.getElementById('ts').innerHTML='<b>帐号格式错误</b>';
		}else if(regCM.test(reEmail)){
			if(!regE.test(reEmail)){
			document.getElementById('errordiv').style.display='block';		
				document.getElementById('ts').innerHTML='<b>邮箱格式错误</b>';		
			}else{
			document.getElementById('errordiv').style.display='none';
				document.getElementById('ts').innerHTML='';
			}
		}else{
			document.getElementById('errordiv').style.display='none';
			document.getElementById('ts').innerHTML='';
			
		}
	}else {
		document.getElementById('errordiv').style.display='none';
	}
 }
  function capLock(e){
 	valueCapsLock = e.keyCode? e.keyCode:e.which;  //Caps Lock　是否打开
    valueShift = e.shiftKey? e.shiftKey:((valueCapsLock == 16 ) ? true : false );//shift键是否按住
	if(valueCapsLock==13){
			setLoginEmail();
			return false;
	}
     if (((valueCapsLock >= 65 && valueCapsLock <= 90 )&&!valueShift) // Caps Lock 打开，并且 shift键没有按住
     || ((valueCapsLock >= 97 && valueCapsLock <= 122 )&& valueShift)) {// Caps Lock 打开，并且按住 shift键 
  		document.getElementById('mmerrordiv').style.display='block';		
		document.getElementById('ms').innerHTML='<b>大写状态开启</b>';
	}
 	else {
  		document.getElementById('mmerrordiv').style.display='none';
		document.getElementById('ms').innerHTML='';
	}
 }

</script>
<div id="widebar">
<!-- 广告放在这里 -->
<c:if test="${requestScope.visiter.loginCount > 6 && visiter.stage == 20}">
    <%@ include file="/inc/promo-wide.inc" %>
</c:if>

<script type="text/javascript">
	function addBookmark(title,url) {
		if( document.all ) {
			window.external.AddFavorite( url, title);
		} else if (window.sidebar) {
			window.sidebar.addPanel(title, url,"");
		} else if( window.opera && window.print ) {
			return true;
		}
	}
	function hotkey(eventobject){
		if(window.event.keyCode==13){
			setLoginEmail();
			return false;
		}
		else{
			return true;
		}
	}
</script>
<div id="utility"><a href="#" onclick="this.style.behavior='url(#default#homepage)';this.setHomePage('${applicationScope.urlProfile}');">设为首页</a><span class="pipe">|</span><a href="javascript:addBookmark('${domainIdentified["siteName"]} - 因为真实，所以精彩','${applicationScope.urlWww}/');">加入收藏</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a></div>
<div id="navigation"></div>