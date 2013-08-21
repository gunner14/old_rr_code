<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page import="com.xiaonei.reg.register.logic.additional.*" %>
<%
	String[] codeTypeDef={"guganwang","kuliu","tudou","youku"};
	String codeType = "";
	if("17015".equals(request.getParameter("ss"))){
		codeType = request.getParameter("g");
	}
%>
<!--LandingPage
将以下代码 嵌入LandingPage HTML代码尾部即可
-->
<script language="javascript" src="http://tmtest2.admaster.com.cn/sod/admBcnAct2.js"></script>
<script language="javascript">var cmpnId=115,actInfo=(new Date()).getTime(),weight=1,tagStr="160"; admBcnActGen(cmpnId,actInfo,weight,tagStr);</script>
<%if(codeTypeDef[0].equals(codeType)) {%>
	<script type="text/javascript">
		var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
		document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
		try {
			var pageTracker = _gat._getTracker("UA-1891599-15");
			pageTracker._trackPageview();
		} catch(err) {}
	</script>
	
	<div style='display:none'><script src="http://s45.cnzz.com/stat.php?id=1624916&web_id=1624916" language="JavaScript" charset="gb2312"></script></div>
<%}%>
<%if(codeTypeDef[1].equals(codeType)) {%>
	<script type="text/javascript">
		var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
		document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
		try {
			var pageTracker = _gat._getTracker("UA-1891599-17");
			pageTracker._trackPageview();
		} catch(err) {}
	</script>
	
	<div style='display:none'><script src="http://s108.cnzz.com/stat.php?id=1624908&web_id=1624908" language="JavaScript" charset="gb2312"></script></div>
<%}%>
<%if(codeTypeDef[2].equals(codeType)) { //tudou%>
	<script type="text/javascript">
		var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
		document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
	try {
		var pageTracker = _gat._getTracker("UA-1891599-18");
		pageTracker._trackPageview();
	} catch(err) {}</script>
	<div style='display:none'><script src="http://s100.cnzz.com/stat.php?id=1624914&web_id=1624914" language="JavaScript" charset="gb2312"></script></div>
<%}%>
<%if(codeTypeDef[3].equals(codeType)) {%>
	<script type="text/javascript">
		var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
		document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
		try {
			var pageTracker = _gat._getTracker("UA-1891599-19");
			pageTracker._trackPageview();
		} catch(err) {}
	</script>
	<div style='display:none'><script src="http://s108.cnzz.com/stat.php?id=1624912&web_id=1624912" language="JavaScript" charset="gb2312"></script></div>
<%}%>