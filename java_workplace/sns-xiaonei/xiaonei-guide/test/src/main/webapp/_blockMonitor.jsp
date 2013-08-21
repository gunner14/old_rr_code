<%@page import="com.xiaonei.platform.core.opt.ConnCounter"%><%@page import="java.util.Iterator"%><%@page import="java.util.Map"%><%@page import="com.xiaonei.platform.core.opt.ConnCounterMgr"%><%@ page contentType="text/html; charset=UTF-8" %><%
Map map =	ConnCounterMgr.getCounters();
for(Iterator it = map.keySet().iterator(); it.hasNext();)
{
	String key = (String) it.next();
	ConnCounter value = (ConnCounter)map.get(key);
	out.println(value.getName()  + "|" + value.getCounter().intValue() + "|" + value.getMaxBlocked());
}
%>