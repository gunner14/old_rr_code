<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page import="com.xiaonei.reg.register.logic.additional.*" %>
<% 
	int rad = new java.util.Random().nextInt(9);
	boolean isShow = (rad >= 1) && LianMengLogic.isHaoyeLink(request);
%>
<%if(isShow) {%>
	<!--AdForward I-allyes Begin-->
		<script language='JavaScript'>
			document.write('<scr'+'ipt language=javascript src=http://smarttrade.allyes.com/main/adftrack?db=smarttrade&point=3610&js=on&cache='+(new Date()).getTime()+'&pre='+escape(document.referrer)+'><\/s'+'cript>');
		</script>
	<!--AdForward I-allyes End-->
<%}%>