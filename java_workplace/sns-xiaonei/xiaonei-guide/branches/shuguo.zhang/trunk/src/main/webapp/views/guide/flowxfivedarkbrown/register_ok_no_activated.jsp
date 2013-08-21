<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@page import="org.apache.commons.lang.StringUtils"%><html xmlns="http://www.w3.org/1999/xhtml">
<head>
<%@ include file="/inc/head-reg-ok.inc" %>
<script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$"></script><!-- added by auscar 2009-8-21  -->   
<link href="http://s.xnimg.cn/css/register.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<title>${domainIdentified["siteName_fillinfo"]} - 注册成功</title>
<script type="text/javascript">
XN.namespace('Browser');
$extend( XN.Browser , {
	addHomePage: function(url) {
		if(!!(window.attachEvent && !window.opera)){
				document.body.style.behavior = 'url(#default#homepage)';
				document.body.setHomePage(url);
			}else{
			  if(window.clipboardData && clipboardData.setData){
		    clipboardData.setData('text', url);
		  }else{
		    alert( '您的浏览器不允许脚本访问剪切板，请手动设置~' );
            return;
		  }
		  alert('网址已经拷贝到剪切板,请您打开浏览器的选项,\n把地址粘到主页选项中即可~');
		}
		return true;		
	},
	addBookMark: function(url, title){
		var ctrl = (navigator.userAgent.toLowerCase()).indexOf('mac') != -1 ? 'Command/Cmd' : 'CTRL';	
		try{
				window.external.addFavorite(url, title || document.title || ' 人人网 - 因为真实, 所以精彩');
		}catch(e){
			try{
				window.sidebar.addPanel(url, title || document.title || ' 人人网 - 因为真实, 所以精彩');
			}catch(e){
				alert('您可以尝试通过快捷键' + ctrl + ' + D 添加书签~');
			}
		}		
	}
});
<%if("205224".equals(request.getParameter("action_id")) || "205110".equals(request.getParameter("action_id"))){%>
XN.dom.ready(function(){
XN.DO.alert({
msg:'<div class="popcollect">点击以下图标，可以更方便地找到人人网<p><a href="http://reg.renren.com/sc.jsp" title="点击右键，选择“另存为...”，将此快捷方式保存到桌面即可"><img src="http://s.xnimg.cn/imgpro/app/6227/icon_01.jpg" alt="" />发送到桌面</a><a href="javascript:;" onclick="XN.Browser.addBookMark(\'http://www.renren.com\')"><img src="http://s.xnimg.cn/imgpro/app/6227/icon_02.jpg" alt="" />加入收藏</a><a href="javascript:;" onclick="XN.Browser.addHomePage(\'http://www.renren.com\')"><img src="http://s.xnimg.cn/imgpro/app/6227/icon_03.jpg" alt="" />设为首页</a></p></div>',
params : {showCloseButton:true}
})
})
<%}%>
</script>

<c:if test="${!empty requestScope.emailPage}">
<script type="text/javascript">
function gotomail(){
	window.open("${requestScope.emailPage}");
}
</script>
</c:if>
<style type="text/css">
.compatible{ width:825px!important;}
.compatible #content{ width:810px;}
.regconfirmbox{ font-size:14px; line-height:1.5em; position:relative; background:url(${applicationScope.urlStatic}/img/reg/confirmbg2.png) top center no-repeat; height:400px; margin:0 auto;}
.t1{ position:absolute; left:240px; top:100px; margin-bottom:10px;}
.t2{ position:absolute; left:260px; top:265px; }
.t2 ul{ list-style:none;}
.color-red{ color:#ff0000;}
.regconfirmbtn{ position:absolute; left:240px; top:160px;}
.regconfirmbtn img {}
.regconfirmbtn b { margin-bottom:7px; margin-left:5px; display:inline-block;font-weight:normal;}
.popcollect { line-height:24px;}
.popcollect p { margin:10px 0;}
.popcollect a{ display:inline-block; margin:0 25px; text-align:center; width:68px;}
.popcollect a img { display:block; width:24px; height:24px; margin:0 auto;}
</style>
</head>
<body>
<div id="container" class="pageregister">
	<%@ include file="/inc/header-noad.inc" %>
	<%@ include file="/inc/sidebar-mini.inc" %>
	<div id="content">
		<%@ include file="/inc/errors.inc" %>
			<div class="regconfirmbox">
				<div class="t1"><strong>${requestScope.email}</strong> 将收到一封认证邮件，请查收。<br/>
				成功认证后，你就可以畅快使用站内所有功能，否则只能体验站内的部分功能哦~~</div>	
				<div class="regconfirmbtn"><c:if test="${!empty requestScope.emailPage}"><a href="#void" onclick="gotomail();">
				<img src="${applicationScope.urlStatic}/img/reg/confirmbtn2.png"/></a></c:if><b><a href="http://www.renren.com/Home.do">>>暂不认证，随便看看</a></b></div>				
				<div class="t2">
					<ul>
						<li>可以 到垃圾邮件目录里找找</li>
						<li>用${requestScope.email}向 <strong>hi@${applicationScope.sysdomain}</strong> 发送一封任意邮件，几分钟后既可完成认证并直接从首页登录</li>
					</ul>
				</div>
			<%@ include file="/pages/register/inc/reg_active_debug.inc" %>				
			</div>
	</div>
	<%@ include file="/pages/register/inc/reg_ok_stat.inc" %>
	<%@ include file="/inc/footer-noad.inc" %>
</div>
<%@ include  file="/pages/register/reg/inc/lianmeng/haoye_reg_complete.inc.jsp" %>
<%@ include  file="/pages/register/reg/inc/lianmeng/video_reg_complete.inc.jsp" %>
<%@ include  file="/pages/register/reg/inc/lianmeng/shipin_tuiguang_reg_complete.inc.jsp" %>
</body>
<%
//传IP
try {
        com.xiaonei.platform.core.model.User user = (com.xiaonei.platform.core.model.User)com.xiaonei.platform.core.opt.base.BaseThreadUtil.currentHost(request);
        String ip = com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil.getClientIP(request);
        com.xiaonei.xce.friendfinderbyip.FriendFinderByIPAdapter.getInstance().accessIPStr(user.getId(),ip);
        //response.sendRedirect(com.xiaonei.reg.common.constants.Globals.urlGuide + "/guide");
}
catch (Throwable e) {
}
%>
</html>