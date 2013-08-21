<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<%@ include file="/inc/kaixin/head.inc" %>
<link href="${applicationScope.urlStatic}/kaixin/css/register.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<title>开心网 - 注册成功</title>
<c:if test="${!empty requestScope.emailPage}">
<script language="javascript">
function gotomail()
{
	window.open("${requestScope.emailPage}");
}
</script>
</c:if>

<style type="text/css">
 .compatible{ width:825px!important;}
 .compatible #content{ width:810px;}
 .regconfirmbox{ font-size:14px; line-height:1.5em; position:relative; background:url(${applicationScope.urlStatic}/kaixin/img/reg/confirmbg.png) top center no-repeat; height:400px; margin:0 auto;}
 .t1{ position:absolute; left:327px; top:77px; margin-bottom:10px;}
 .t2{ position:absolute; left:350px; top:265px; }
 .t2 ul{ list-style:none;}
 .color-red{ color:#ff0000;}
 .regconfirmbtn{ position:absolute; left:325px; top:160px;}
 #navigation .nav-right li a {display:none;}
</style>
</head>
<body>
<div id="container" class="pageregister">
	<%@ include file="/inc/kaixin/header-not-login.inc" %>
	<div id="content">
		<%@ include file="/inc/errors.inc" %>
			<div class="regconfirmbox">

				<div class="t1">你的邮箱 <strong>${requestScope.email}</strong> <br />将收到一封注册确认邮件，请到<span class="color-red"><strong>收件箱</strong></span>或<span class="color-red"><strong>垃圾邮件</strong></span>里查找，<br/>
				找到后点击邮件中的 <span class="color-red"><strong>确认链接</strong></span> 确认你的帐号。</div>
		
				<div class="regconfirmbtn"><c:if test="${!empty requestScope.emailPage}"><a href="#void" onclick="gotomail();"><img src="${applicationScope.urlStatic}/kaixin/imgpro/reg/email.gif"/></a></c:if></div>
				
				<div class="t2">
					<ul>
						<li>用该邮箱向 <strong>hi@${applicationScope.domain}</strong> 发送一封任意邮件，几分钟后即可直接从首页登录。</li>
					</ul>
						<br/>
						
						<%
						//<span style="margin-left:345px;">&nbsp;</span>
						//<a href="http://yd.kaixin.com/yd.do" title="跳过" >暂不确认，跳过此步>></a>
						 %>
						 <%
                         if(com.xiaonei.reg.common.constants.Globals.DEBUG_ENV){ //(com.xiaonei.platform.core.model.User)request.getAttribute("visitor");//
                             com.xiaonei.platform.core.model.User user = com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory.getUserAdapter().getByEmail(request.getAttribute("email").toString());
                             String vc_key = com.kaixin.register.logic.RegAccountLogic.MEM_CACHE_NEW_INVITEE_KEY + user.getId();
                             com.xiaonei.platform.core.cache.MemCacheManager manager = com.xiaonei.platform.core.cache.MemCacheFactory.getInstance().getManagerByPoolName( com.xiaonei.platform.core.cache.MemCacheKeys.pool_guide);
                             String vc = (String)manager.get(vc_key);
                             if(vc == null || "".equals(vc))
                                vc = com.kaixin.register.users.UserUtils.getInstance().getVerifyCode(user);
                             
                             String activeUrl = com.xiaonei.reg.common.constants.Globals.urlReg.toString() + "/activate.do?id=" + user.getId() + "&vc=" + vc;
                             //out.println("<a href='"+activeUrl+"'>active</a>");
                             out.println(""+"<ul><a style=\"color:white\" href=\""+activeUrl+"\" >debug_active</a></ul>");
                         }
                         %>
				</div>
				
			</div>
	</div>
	</div>
<%@ include file="inc/reg_ok_stat.inc" %>
<%@ page pageEncoding="UTF-8" %>
				</div>
			</div>
		</div>
	</div>
	<div id="footer">
		<div class="copyright">
			<span class="float-right">
				<a href="${applicationScope.urlSupport}/GetGuestbookList.do">客服</a><span class="pipe">|</span>
				<a href="http://${applicationScope.domain}/info/Help.do">帮助</a>
			</span>开心网 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2010
		</div>
	</div>
	<div class="hidden-area"></div>
<script src="${applicationScope.urlStatic}/kaixin/utm/urchin.js" type="text/javascript">
</script> 
<script type="text/javascript"> 
   _userv=2; 
   urchinTracker(); 
</script>

<c:if test="${requestScope.ss == '17001' && requestScope.bpara != '' && requestScope.crand <80}">
<!--AdForward I-allyes Begin-->
<script language='JavaScript'>
document.write('<scr'+'ipt language=javascript src=http://smarttrade.allyes.com/main/adftrack?db=smarttrade&point=3611&js=on&cache='+(new Date()).getTime()+'&pre='+escape(document.referrer)+'><\/s'+'cript>');
</script>
<!--AdForward I-allyes End-->
</c:if>
</body>
</html>
