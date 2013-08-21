<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<%@page import="org.apache.commons.lang.StringUtils"%>
<%@page import="com.xiaonei.reg.register.logic.additional.RegQQLogic"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheFactory"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<title>QQ邮箱注册激活</title>
<style>
html {overflow:-moz-scrollbars-vertical;overflow-y:scroll;}
body {background:#fff;font-family:Verdana,'宋体';font-size:13px;line-height:22px;}
*{ margin:0; padding:0;}
form { border:0; }
#top { background:url(http://s.xnimg.cn/imgpro/bg/menubg2.gif) repeat-x; height:52px; text-align:left;}
#wrap { width:800px; margin:0 auto; margin-top:60px; padding-top:30px; border:1px #eee solid; text-align:center;}
#login { padding-bottom:20px;}
#ta { padding-top:5px; text-align:left; width:600px; margin:0 auto; color:#999;}
#tips {border:1px #D1BEAA solid; width:600px; height:30px; padding:10px 5px; text-align:left;  margin:0 auto;margin-top:20px;}
input.i_text,input.i_textlong { width:180px; height:20px; padding:2px; padding-top:4px;border:1px #B8D4E8 solid;vertical-align:middle;}
input.i_textlong { width:340px; height:24px; padding-bottom:4px; font-size:18px; font-weight:bold; color:#005EAC; font-family:"微软雅黑"; font-weight:normal;}
input.i_submit { width:117px; height:35px; background:url(${applicationScope.urlStatic}/imgpro/reg/qqmanage_btn.gif) no-repeat; border:0; color:#fff; font-size:14px; cursor:pointer; font-weight:bold; vertical-align:middle;}
iframe#activateframe { border:1px #D1BEAA solid; width:600px; height:30px; padding:10px 5px; text-align:left; margin-top:20px;}
#main span { margin-left:46px;}
#main span a { display:inline-block; padding:0 5px; margin:0 5px; color:#666; text-decoration:none;}
#main span a:hover { text-decoration:underline;}
.list { color:#005EAC; text-align:left; width:610px; margin:0 auto; margin-top:10px;}
.list h3 { font-size:14px;}
</style>
<script type="text/javascript"> 
function copy_clip(copy){
if (window.clipboardData){
window.clipboardData.setData("Text", copy);}
else if (window.netscape){
netscape.security.PrivilegeManager.enablePrivilege('UniversalXPConnect');
var clip = Components.classes['@mozilla.org/widget/clipboard;1'].createInstance(Components.interfaces.nsIClipboard);
if (!clip) return;
var trans = Components.classes['@mozilla.org/widget/transferable;1'].createInstance(Components.interfaces.nsITransferable);
if (!trans) return;
trans.addDataFlavor('text/unicode');
var str = new Object();
var len = new Object();
var str = Components.classes["@mozilla.org/supports-string;1"].createInstance(Components.interfaces.nsISupportsString);
var copytext=copy;
str.data=copytext;
trans.setTransferData("text/unicode",str,copytext.length*2);
var clipid=Components.interfaces.nsIClipboard;
if (!clip) return false;
clip.setData(trans,null,clipid.kGlobalClipboard);}
//alert("已复制"+copy)
return false;
}
<!--  
//建立XMLHttpRequest对象  
var xmlhttp;  
try{  
    xmlhttp= new ActiveXObject('Msxml2.XMLHTTP');  
}catch(e){  
    try{  
        xmlhttp= new ActiveXObject('Microsoft.XMLHTTP');  
    }catch(e){  
        try{  
            xmlhttp= new XMLHttpRequest();  
        }catch(e){}  
    }  
} 
function getPart(url){  
	var number = document.getElementById("number").value;
    xmlhttp.open("get",url+'?number='+number + '&radom=' +Math.random(), true);  
    xmlhttp.onreadystatechange = function(){  
        if(xmlhttp.readyState == 4)  
        {  
            if(xmlhttp.status == 200)  
            {  
                if(xmlhttp.responseText!=""){  
				var tipsvalue = document.getElementById('tips');
                    tipsvalue.innerHTML = unescape(xmlhttp.responseText);
					//alert(tipsvalue.innerHTML);
					if (tipsvalue.innerHTML === "{\"code\":1,\"msg\":\"激活成功\"}") {
					copy_clip("您的帐号已激活，感谢您的注册！")
					};
					if (tipsvalue.innerHTML === "{\"code\":2,\"msg\":\"已经激活过\"}") {
					copy_clip("您的帐号已经激活过，请直接登录人人网！")
					};
					if (tipsvalue.innerHTML === "{\"code\":0,\"msg\":\"激活失败\"}" || tipsvalue.innerHTML === "{\"code\":3,\"msg\":\"用户不存在\"}"){
					copy_clip("请务必用您注册时的QQ号发送消息，不要使用与注册时QQ邮箱不一致的号码发送，谢谢！")
					};
		
                }  
            }  
            else{  
                document.getElementById("tips").innerHTML = "数据加载出错";  
            }  
        }  
    }  
    xmlhttp.setRequestHeader("If-Modified-Since","0");  
    xmlhttp.send(null);  
} 
getPart('http://reg.renren.com/WS/activate.do')

//-->  
</script> 
</head>
<body onLoad="document.forms.activeform.number.focus()">
<div id="top"><img src="${applicationScope.urlStatic}/imgpro/reg/qqmanage_logo.gif" alt="" /></div>
<div id="wrap">
<%
	if(StringUtils.equals("true",request.getParameter("logout"))){
		session.removeAttribute("isLogin");
	}
	if(StringUtils.equals("true",request.getParameter("rm"))){
		MemCacheFactory.getInstance()
		.getManagerByPoolName(MemCacheKeys.pool_user_space).delete("mem_reg_qq_activate");
	}
	if (!StringUtils.equals("true", (String)session.getAttribute("isLogin"))) {
		if (StringUtils.equals("test", request.getParameter("name"))
				&& StringUtils.equals("test", request
						.getParameter("pwd"))) {
			session.setAttribute("isLogin","true");	
		} else {
%>
			<form action="">
            <div id="login">
				<input type="text" name="name" class="i_text" /> 
				<input type="password" name="pwd" class="i_text" /> 
				<input type="submit" class="i_submit" value="登 录" />
              </div>
			</form>
<%
			return;
		}
	}
%>
<div id="main">
<form name="activeform" id="activeform">

<input type="hidden" name="key" value="vjsSDFaoGEesd93752wsadWERD" class="i_text" />
<input type="text" name="number" id="number" class="i_textlong" /> 
&nbsp;<input type="submit" class="i_submit" value="激 活"  id="i_submit" onclick="getPart()" /><span><a href="?logout=true">退出</a><a href="?rm=true">清空</a></span>
</form>
<div id="ta"></div>
<div id="tips"></div>
<div class="list">
<h3>激活列表（包括未激活）</h3>
<%
out.println("<br />");
List<String> l=RegQQLogic.get();
for(String s:l){
	out.println(s+"<br />");
}
%>
<script type="text/javascript">
var qqs = new Array();
<%
out.println("<br />");
int i =0 ;
for(String s:l){
	out.println("qqs["+i+"]='"+s+"'");
	++i;
}
%>
var orginvalue="";
function jc() {
	if(document.getElementById('number').value == orginvalue){
		return;
	}
	document.getElementById('ta').innerHTML="";
	for(var i in qqs){
		//alert(document.getElementById('number').value+" "+orginvalue)
		if(qqs[i].indexOf(document.getElementById('number').value)==0){
			document.getElementById('ta').innerHTML+="<ul>"+qqs[i]+"</ul>";
		}
	}	
	orginvalue=document.getElementById('number').value;
	
}
window.setInterval("jc()",1);
</script>
</div>
</div>
</div>
</body>
</html>