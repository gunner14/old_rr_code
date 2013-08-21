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

<style>
	input {
		border:1px solid blue;
	}

</style>
<form action="http://browse.renren.com/searchgd">

s:<input name="s" value="0" />
ajax:<input name="ajax" value="1" />
q:<input name="q" value="李小明" />
<br />
p:<input size="200" name="p" value='[{"t":"high","name":"高中名","year":"2009"},{"t":"univ","name":"大学名","id":"1004","depa":"院系","year":"2009"},{"t":"juni","name":"&#229;&#140;&#151;&#228;&#186;&#172;&#228;&#186;&#148;&#229;&#141;&#129;&#229;&#155;&#155;&#228;&#184;&#173;"}][{"t":"high","name":"高中名","year":"2009"},{"t":"univ","name":"大学名","id":"1004","depa":"院系","year":"2009"}，{"t":"juni","name":"&#229;&#140;&#151;&#228;&#186;&#172;&#228;&#186;&#148;&#229;&#141;&#129;&#229;&#155;&#155;&#228;&#184;&#173;"}]' />

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
String from = request.getParameter("fromsource");
String regaccount = request.getParameter("regaccount");
String gender = request.getParameter("gender");
String name = request.getParameter("name");
if(gender.equals(""))gender=null;
%>