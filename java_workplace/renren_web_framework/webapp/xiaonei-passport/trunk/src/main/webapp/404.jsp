<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
<%
System.err.println("404:"+request.getRequestURI()+", Referer:"+request.getHeader("Referer")+", error:"+request.getAttribute("javax.servlet.error.request_uri"));
%>
<%@ include file="404-filter.jsp" %>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<link href="${applicationScope.urlSmallStatic}/csspro/apps/error.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<%@ include file="/inc/headpro.inc" %><%-- 头部信息 --%>
		<title>${domainIdentified["messages.domain.title"]} - 抱歉，没有找到该网页</title>
	</head>
	<body id="error404Page" class="error">
		<script>
			window.onload=function(){
				(new Image()).src="http://log.xiaonei.com/rl.do?f=404&m=" + document.getElementById('mach').value + "&t=" + Math.random();
			}
		</script>
		<div id="container">
			<%@ include file="/inc/header-noad.inc" %>
			<div id="opi">
				<div class="http404 error msg">
					<h2>404</h2>
					<p>抱歉，你输入的网址可能不正确，或者该网页不存在。<a href="${applicationScope.urlWww}">返回首页</a>或<a href="javascript:history.go(-1)">返回上一页</a></p>
				</div>
				
				<c:if test="${domainIdentified['sys.domain']=='renren.com'}">
				<div class="promotion clearfix">
					<h2>最近大家在关注这些内容:</h2>
					<div id="hotBlogsPanel" class="panel">
						<div class="header">
							<h3>论坛热帖 <strong class="top-n">TOP5</strong></h3>
						</div>
						<div class="section">
							<ol class="blogs list">
  								<li><a href="${applicationScope.urlClub}/index.php?uri=/show_214_216406_1.html">我的减肥经历：两个月瘦了16斤</a> </li>
  								<li><a href="${applicationScope.urlClub}/index.php?uri=/show_238_232971_1.html">青春在窗外的风中飘逝了</a> </li>
  								<li><a href="${applicationScope.urlClub}/index.php?uri=/show_114_704339_1.html">&quot;毕分族&quot;，感情就那么脆弱么？</a> </li>
  								<li><a href="${applicationScope.urlClub}/index.php?uri=/show_114_704643_1.html">一人一青春，一世一情事</a> </li>
  								<li><a href="${applicationScope.urlClub}/index.php?uri=/show_238_233006_1.html">一个真实的技校生活</a></li>
  							</ol>
						</div>
					</div>
					<div id="hotVotesPanel" class="panel">
						<div class="header">
							<h3>热门投票 <strong class="top-n">TOP5</strong></h3>
						</div>
						<div class="section">
							<ol class="votes list">
								<li><a href="${applicationScope.urlAbc}/knowabc/investigation/voted.action?invID=3801635">小时候期待过的事，参考个人经历</a> </li>
  								<li><a href="${applicationScope.urlAbc}/knowabc/investigation/voted.action?invID=3789347">你小时候肯定做过的18件事，超过5件你就太可爱了</a> </li>
  								<li><a href="${applicationScope.urlAbc}/knowabc/investigation/voted.action?invID=3796540">小时候你看过哪些动画片到现在还是记忆犹新？</a> </li>
  								<li><a href="${applicationScope.urlAbc}/knowabc/investigation/voted.action?invID=3792787">80后90后小时候最受欢迎的零食 ，你都吃过多少？</a> </li>
  								<li><a href="${applicationScope.urlAbc}/knowabc/investigation/voted.action?invID=3769479">你还记得这些儿时的游戏吗?玩过五个以上你就是孩子王~</a>  </li>
							</ol>
						</div>
					</div>
				</div>	
				</c:if>			
			</div>
			<c:set var="INC_FOOTER_MODE" value="0" />
			<%@ include file="/inc/footerdiv.inc" %>
		</div>
		<script src="${applicationScope.urlSmallStatic}/utm/urchin.js?ver=$revxxx$" type="text/javascript">
		</script> 
		<script type="text/javascript"> 
   			_userv=0; 
   			urchinTracker(); 
		</script> 
		<input type="hidden" id="mach" name="mach" value="${applicationScope.hostName}">
	</body>
</html>
<%
try{
org.apache.log4j.Logger logger = org.apache.log4j.Logger.getLogger("xnlog.404");
java.lang.Throwable t = (java.lang.Throwable)request.getAttribute("javax.servlet.error.exception");
String p = "host:"+application.getAttribute("hostName")+" - ref:" + request.getHeader("Referer")+" - errUri:http://"+request.getServerName()+(request.getAttribute("javax.servlet.error.request_uri")==null?request.getRequestURI():request.getAttribute("javax.servlet.error.request_uri"))+(request.getQueryString()==null?"":("?"+request.getQueryString()));
logger.error(p,t);
}catch(java.lang.Throwable t1){

}
%>
