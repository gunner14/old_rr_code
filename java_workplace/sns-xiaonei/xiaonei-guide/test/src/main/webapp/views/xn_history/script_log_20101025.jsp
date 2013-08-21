<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>


<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%
if(!"inshion".equals(request.getParameter("debug"))){
    return;
}
%>

<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyEnDeCoder"%>
<%@page import="com.xiaonei.platform.core.model.UserState"%>
<%@page import="com.xiaonei.xce.userstate.UserStateAdapter"%>
<%@page import="com.xiaonei.platform.core.model.factory.WUserStateFactory"%>
<%@page import="com.xiaonei.platform.component.request.logger.RequestRemoteLogger"%>
<%@page import="com.xiaonei.xce.log.XceLogger"%>
<%@page import="com.xiaonei.xce.log.XceLoggerAdapter"%><style>
	input {
		border:1px solid blue;
	}

</style>
<form action="">

count:<input name="regaccount" value="" />
time:<input name="time" value="" />

<input type="submit" />
<input type="hidden" name="run" value="true">
<input type="hidden" name="debug" value="inshion">
</form>


<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
boolean isdebug = true;
if(!Globals.DEBUG_ENV){
	isdebug = false;
}
String strid = request.getParameter("regaccount");
String code = request.getParameter("time");
int id = 0;
try{
	id = Integer.parseInt(strid);
}
catch(Exception e){
	
}
if(id != 0){
	RequestRemoteLogger.getInstance().log(id, 1, "just_a_test");
	XceLogger myLogger = XceLoggerAdapter.getLogger("M@XceLogger0");
	String[] CATEGORIES = new String[] { "ActiveTrackXN", "action", "bucket" };
	String KEY = "GUIDE";
	String MSG_SEPARATOR = "\t";
	long TIME = new Date().getTime();
	//myLogger.addLog(CATEGORIES, new StringBuilder(KEY).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(1).append(MSG_SEPARATOR).append(0).append(MSG_SEPARATOR).append("just_a_test").toString());
	String msg = new StringBuilder().append(TIME).append(MSG_SEPARATOR).append(id).append(MSG_SEPARATOR).append(KEY).append(MSG_SEPARATOR).append("LEAD_just_a_test").toString();
	out.println(msg);
	myLogger.addLog(CATEGORIES, msg);
}
%>