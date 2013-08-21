<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="Description" content="开心网是一个与朋友，同学，同事，家人一起玩的地方:)和朋友们抢车位，养宠物狗狗，在开心农场种自己喜欢的蔬菜水果，一起玩游戏、交友。">
<meta name="Keywords" content="开心网,kaixinwang,真开心网,同事,白领,在职人士,博客,相册,社区,交友,游戏,休闲,聊天,音乐,视频,开心美女, 开心宝贝,开心达人,分享照片,网页游戏,flash游戏,休闲游戏,办公室宠物,开心农场,狗狗,抢车位,好友买卖,超级大亨,教父,踢屁屁,咬我吧 ">
<title>开心网登录首页 - 开心农场,狗狗,抢车位,超级大亨,游戏,交友</title>
<script type="text/javascript" src="${applicationScope.urlStatic}/kaixin/jspro/base.js?ver=$revxxx$"></script>
<link href="${applicationScope.urlStatic}/kaixin/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlStatic}/kaixin/csspro/module/navigationpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlStatic}/kaixin/favicon.ico" rel="shortcut icon" type="image/x-icon" />
<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlStatic}/kaixin/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
<!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(${applicationScope.urlStatic}/kaixin/csspro/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->

<script type="text/javascript">
  function addHomePage(url){
    if(!!(window.attachEvent && !window.opera)){
			document.body.style.behavior = 'url(#default#homepage)';
			document.body.setHomePage(url);
		}else{
		  if(window.clipboardData && clipboardData.setData){
        clipboardData.setData("text", url);
      }else{
        div = document.createElement('div');
        document.body.appendChild(div);
        div.innerHTML = '<embed src="${applicationScope.urlStatic}/kaixin/swf/clipboard.swf" FlashVars="clipboard='+encodeURIComponent(url)+'" width="0" height="0" type="application/x-shockwave-flash"></embed>';
      }
      alert('网址已经拷贝到剪切板,请您打开浏览器的选项,\n把地址粘到主页选项中即可~');
    }
    return true;
  }

  function refreshCode(){
	  	var id=Math.round(Math.random()*15926535897);
		var imgUrl = "${applicationScope.urlIcode}/getcode.do?t=feedback_"+id+"&rnd=<%=System.currentTimeMillis()%>";
		$('verifyPic').src = imgUrl;
		$('hcode').value = id;
  }

	function feedbackPop(){
		var content=
			"<div class=\"sugestform\">"
			+"<p style=\"margin:10px 0;\">&nbsp;&nbsp;非常感谢您对开心的支持和关注，请在这里留下您的意见，我们也许无法一一回"
			+"<br>&nbsp;&nbsp;复，但我们会认真阅读，您的支持是我们最大的动力。</p>"
			+"<p style=\"margin-bottom:10px;text-align:left;\">"
			+"&nbsp;&nbsp;<select name=\"\" id=\"ftype\" tabindex=\"5\" style=\"width:200px;\">"
			+"<option value=\"0\">注册相关</option>"
			+"<option value=\"1\">密码相关</option>"
			+"<option value=\"2\">登录相关</option>"
			+"<option value=\"3\">功能求助</option>"
			+"<option value=\"4\">小应用</option>"
			+"<option value=\"5\">建议</option>"
			+"<option value=\"6\">其它</option>"
			+"</select>"
			+"</p>"
			+"<p style=\"margin-bottom:10px;text-align:left\">"
			+"&nbsp;&nbsp;<textarea name=\"\" id=\"feedback\" style=\"width:420px;height:80px\" tabindex=\"6\"></textarea>"
			+"</p>"
			+"<p style=\"margin-bottom:10px;\">"
			+"<label for=\"uemail\" style=\"width:73px;text-align:right;float:left;margin-right:1em;line-height:20px;\">&nbsp;E-mail:</label><input type=\"text\" style=\"width:180px;\" class=\"inputtext\" id=\"femail\" tabindex=\"7\" />"
			+"<span style=\"color:#808080;padding-left:1em\">(我们将通过email与您联系)</span>"
			+"</p>"
			+"<p style=\"margin-bottom:10px;\">"
			+"<label for=\"uname\" style=\"width:73px;text-align:right;float:left;margin-right:1em;line-height:20px;\">你的名字:</label><input type=\"text\" style=\"width:180px;\" class=\"inputtext\" id=\"fname\" tabindex=\"8\" />"
			+"</p>"
			+"<p style=\"margin-bottom:10px;\">"
			+"<label for=\"code\" style=\"width:73px;text-align:right;float:left;margin-right:1em;line-height:20px;\">验证码:</label><input type=\"text\" style=\"width:70px;\" class=\"inputtext code\" id=\"ficode\" tabindex=\"9\" />"
			+"<span style=\"height:20px;\">"
			+"<input type=\"hidden\" id=\"hcode\" value=\"11274269479\" />"
			+"<img src=\"${applicationScope.urlIcode}/getcode.do?t=feedback_11274269479&amp;rnd=1231144600232\" id=\"verifyPic\" />"
			+"</span>"
			+"</p>"
			+"</div>";
			


		var feedbackSubmitResult = function (r){
			if(r.responseText == "code"){
				XN.DO.alert({
					title:"提示",
					message:"<center style='line-height:200%;font-size:14px;'>验证码错误</center>",Y:300});
				confirm.show();
				refreshCode();
			}else{
			XN.DO.alert({
				title:"给我们意见",
				message:"<center style='line-height:200%;font-size:14px;'>感谢您的支持！</center>"});
			}
		}
	    
	    var confirm = XN.DO.confirm({
	    	 message:content,
	    	 title:"给我们意见",
	    	 callBack:function(r){
	    	 	if(!r)return;
				if($("feedback").value != "" && $("ficode").value != ""){
					var email = encodeURIComponent($("femail").value);
					var name = encodeURIComponent($("fname").value);
					var icode = encodeURIComponent($("ficode").value);
					var content = encodeURIComponent($("feedback").value);
					var type = encodeURIComponent($("ftype").value);
					var hcode = encodeURIComponent($("hcode").value);
					var ajax2 = new XN.NET.xmlhttp();
	
					var par = "email="+email
							+"&type="+type
							+"&name="+name
							+"&icode="+icode
							+"&content="+content
							+"&hcode="+hcode;
					ajax2.post('${applicationScope.urlWww}/leaveFeedBack.do',par,feedbackSubmitResult);

				}else{
					if($("feedback").value == ""){
						XN.DO.alert({message:"<center style='line-height:200%;font-size:14px;'>请输入内容</center>",title:"提示",Y:300});
					}else if($("ficode").value == ""){
						XN.DO.alert({message:"<center style='line-height:200%;font-size:14px;'>请输入验证码</center>",title:"提示",Y:300});
					}
					this.show();
				}
    		},
	    	width:465});
	    refreshCode();
	    	
  }
</script>
<link href="${applicationScope.urlStatic}/kaixin/csspro/module/reg.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlStatic}/kaixin/csspro/apps/login.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <style type="text/css">
#syshome .page-wrapper .widebar .tvbox,
#syshome .page-wrapper .sidebar .portal,
.sign-up,
.updates .advice a,
.infobox dd,
.infobox dd {background-image:url(${applicationScope.urlStatic}/kaixin/imgpro/login/sys-bj2.jpg);}
#navigation {display:none;background:transparent url(${applicationScope.urlStatic}/kaixin/imgpro/bg/v2_head_bg.png) repeat-x scroll left top;width:831px;height:37px;}
#navigation .nav-right {background:transparent url(${applicationScope.urlStatic}/kaixin/imgpro/bg/v2_head_bg_r.png) no-repeat scroll right top;padding-right:20px;margin-right:0;padding-top:3px;}
#navigation .nav-right li a{padding-bottom:12px;*padding-bottom:13px;text-decoration:none;color:#FEF6E6;}
#header{margin-top:0;}
body {background:url(${applicationScope.urlStatic}/kaixin/imgpro/bg/v2_body_bj.png) repeat-x center top;}
#syshome .page-wrapper .widebar .tvbox{background-position:0 301px;}
#syshome .page-wrapper .widebar .sign-up{background:url(${applicationScope.urlStatic}/kaixin/imgpro/login/sys-bj2.jpg) no-repeat -429px 0;}
.infobox .ucando dd {background-position:0 15px;margin-bottom:4px;}
.advice #suggestion{background-position:-642px -469px;}
#syshome .page-wrapper .sidebar .portal{background:transparent;}
</style>
    
</head>
<body id="syshome">
	<div class="mainbox" style="background:none;">
    <!--head-->
    <div id="header">
		<div id="logo"><a href="${applicationScope.urlHome}/Home.do?id=${requestScope.host.id}" title="开心网"><img src="${applicationScope.urlStatic}/kaixin/imgpro/logo/v2_logo.png" alt="开心网(Logo)"></a></div>
		<div id="navigation" class="navigation-nologin">			
            <ul class="nav-right">
                <li></li>
                <li></li>
                <li></li>
                <li></li>
            </ul>
        </div>
	</div><!--head END-->

<div class="full-pages">
	<div id="opi" class="page-wrapper clearfix">
<!--begin -->
	<div class="sidebar">
		<div class="inner">
			<div class="innertop">&nbsp;</div>
			<form method="post" id="loginForm" action="${applicationScope.urlPassport}/PLogin.do" class="login-form">			
				<p class="top">
					<label for="email">帐号:</label>
					<input type="text" name="email" rule="blank#loginName" class="input-text" value="${_de}" id="email" tabindex="1" />
				</p>
				<p>
					<label for="password">密码:</label>					
					<input type="password" id="password" name="password" rule="blank" error="请输入密码" class="input-text" tabindex="2"  />
				</p>
				<p>
					<label for="autoLogin" class="labelCheckbox" id="autologlabel"><input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="3" />下次自动登录</label>
				</p>
				<p class="logbtn">
						<input type="hidden" name="origURL" value="http://home.kaixin.com/Home.do" />
						<input type="hidden" id="loginregFrom" name="loginregFrom"  value="index" />
						<input type="hidden" id="ss" name="ss"  value="17014" />
						<input type="hidden" name="domain" value="kaixin.com" />
						<input type="hidden" name="g" id="g" value="${requestScope.g}" />
						<input type="hidden" name="b" id="b" value="${requestScope.b}" />							
						<button type="submit" class="input-submit" id="login" tabindex="4">登录</button>
				</p>
				</form>			
			<div class="innerbottom">&nbsp;</div>
		</div>

		<div class="reg-guide">
			<div class="innertop">&nbsp;</div>
			<div class="goreg">
				<p class="portal">还没有开通你的开心网帐号？</p>
				<a class="gotoreg" href="http://reg.kaixin.com/register.do?ss=17014&rt=${requestScope.rt}&b=${requestScope.b}">立即注册</a>
			</div>
			<div class="innerbottom">&nbsp;</div>
		</div>
	
	</div>
	<div class="widebar">
		<div class="tvbox">
			<div class="fourpic">
				<ul>
					<li><a href="#" class="pic">&nbsp;</a></li>
					<li><a href="#" class="pic">&nbsp;</a></li>
					<li><a href="#" class="pic">&nbsp;</a></li>
					<li><a href="#" class="pic">&nbsp;</a></li>
				</ul>
			</div>
		</div>
		<div class="infobox clearfix">
			<div class="updata-advice">
				<div id="kaixin_update">
				</div>
			</div>
			<div class="sign-up">
				<a class="regbutton" href="http://reg.kaixin.com/register.do?ss=17014&rt=${requestScope.rt}&b=${requestScope.b}">&nbsp;</a>
				<div class="ucando">
					<dl>
						<dt>加入开心网你可以:</dt>
						<dd>与同事、朋友保持更紧密的联系</dd>
						<dd>了解朋友的动态</dd>
						<dd>和朋友分享相片、音乐和电影</dd>
						<dd>分享你的照片、心情和快乐</dd>
						<dd>养<a href="http://dog.kaixin.com/gougou.htm">开心狗狗</a>、玩<a href="http://www.kaixin.com/kaixinnongchang.htm">开心农场</a></dd>
					</dl>
				</div>
			</div>
		</div>
	</div>
	<!-- end -->
	</div>

</div>
<div id="footer">
	<div class="copyright" style="overflow:hidden; zoom:1;margin-bottom:0">
		<span class="float-left">开心网 <span title="revision$revxxx$;${applicationScope.hostName}">&copy;</span> 2010</span>
	</div>
	<div>
		<a style="float: left; margin-right: 10px;" target="_blank" href="http://www.hd315.gov.cn/beian/view.asp?bianhao=010202008123100016"><img src="${applicationScope.urlStatic}/kaixin/imgpro/bg/biaoshi.gif"/></a>	
	</div>
</div> 
</div>       	
</body>
</html>
<script type="text/javascript">
function $(s){
	return document.getElementById(s);
}
function isCapsLockOn(e){
	var c = e.keyCode || e.which;
	var s = e.shiftKey;
	if(((c >= 65 && c <= 90) && !s) || ((c >=97 && c <= 122) && s)){
		return true;
	}
	return false;
}

function showMsg(s){
	//var el = $('errorMessage');
	//el.style.display =  'block';
	//el.innerHTML = s;
}

function hideMsg(){
	//$('errorMessage').style.display = 'none';
}

$('loginForm').onsubmit = function(){
	function isEmail(str){
		var tmp = '';
		str = str.replace(/^\s+|\s+$/g,"");
		for(var i = 0,j = str.length;i < j;i++){
			var code = str.charCodeAt(i);
			if(code >= 65281 && code <= 65373){
				tmp += String.fromCharCode(code - 65248);
			}else{
				tmp += String.fromCharCode(code);
			}
		}
		tmp = tmp.replace(/·/,'@');
		$('email').value = tmp = tmp.replace(/[。|,|，|、]/g,'.');
		return /^[A-Z_a-z0-9-\.]+@([A-Z_a-z0-9-]+\.)+[a-z0-9A-Z]{2,4}$/.test(tmp);
	}
/*
	if(!isEmail($('email').value)){
		showMsg('E-mail格式错误');
		$('email').focus();
		return false;
	}
*/	
	if(/^\s*$/.test($('password').value)){
		showMsg('您还没有填写密码');
		$('password').focus();
		return false;
	}
	return true;
}

$('password').onkeypress = function(e){
	if(isCapsLockOn(e || window.event)){
		showMsg('大写锁定开启');
	}else{
		hideMsg();
	}
}

$('email').focus();
$('email').select();
</script>
