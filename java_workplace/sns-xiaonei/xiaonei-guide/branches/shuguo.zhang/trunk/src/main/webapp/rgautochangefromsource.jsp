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
if(!Globals.DEBUG_ENV){
	out.println("");
	return;
}
%>


<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%><style>
	input {
		border:1px solid blue;
	}

</style>
注意：此页面可更改<font color="red"><b>线上</b></font>用户的来源属性，使用时请小心谨慎，不要填错号了！
<br />
<br />
<br />
<form action="">

id:<input name="id" value="" />改成:
<select name="from">
<option value=":NETBAR">纯网吧</option>
<option value="S_COOKREG:NETBAR">餐厅网吧</option>
<option value="S_FARMREG:NETBAR">农场网吧</option>
<option value="S_MZMG:NETBAR">面馆网吧</option>
<option value="S_COOKREG">纯餐厅</option>
<option value="S_FARMREG">纯农场</option>
<option value="S_MZMG">纯面馆</option>
</select>
<input type="submit" />
<input type="hidden" name="run" value="true">
</form>


<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
if(!Globals.DEBUG_ENV){
	out.println("are you a hacker? :)");
	return;
}
String id = request.getParameter("id");
String from = request.getParameter("from");

try{
	if(MyStringUtils.isNullOrEmpty(id)){
		throw new Exception();
	}
	int nid = Integer.parseInt(id);
	RegFromSourceLogic.update(nid,from);
	
	from = from.replace("NETBAR","网吧").replace("S_COOKREG","餐厅").replace("S_MZMG","面馆");
	out.println("id:"+nid+" 的用户已改成 ["+from+"]用户");
}
catch(Exception e){
	out.println("修改出错：是不是用户id填错了？应该填id而不是邮箱账号……");
}

%>