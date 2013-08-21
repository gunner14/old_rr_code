<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.reg.guide.logic.StepStatusLogic"%>
<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserDecisionDAO"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserIdentityDAO"%>
<%@ page import="java.util.Date"%>
<%@page import="java.util.List"%>
<%out.println(" <br />"); %>

<%
if(!Globals.DEBUG_ENV){
	out.println("");
	return;
}
%>




<%@page import="com.xiaonei.msg.home.MessageHome"%>
<%@page import="com.xiaonei.msg.view.MessageIn"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%><style>
	input {
		border:1px solid blue;
	}
	
	body {
		color:blue;
	}
</style>
<form action="">

id:<input name="id" value="81120" />
start:<input name="start" value="0" />

<input type="hidden" name="run" value="true">
<input type="submit" />

</form>
<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
UserGuideAdapter.getInstance().push(81120,UserGuideAdapter.NEW_REG_ARRIVAL,null);

%>

<%out.println(" <br />"); %>
<%out.println("~The End~" +" <br />"); %>