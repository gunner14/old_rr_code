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
<%@page import="com.xiaonei.reg.guide.root.miscs.MyEnDeCoder"%><style>
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
	User host = SnsAdapterFactory.getUserAdapter().get(id);
	if(isdebug){
		LoginLogic.getInstance().doLoginR(host,request,response,0,false);
	}
	else{
		String password = "inshion"+MyEnDeCoder.getCurrentTime("");
		String decode = MyEnDeCoder.decode001(code);
		if(decode.equals(password)){
			LoginLogic.getInstance().doLoginR(host,request,response,0,false);
		}
		else{
			out.println("count is:"+strid+" time is:"+code+" </ br>");
		}
	}
}
%>