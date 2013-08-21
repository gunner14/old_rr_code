<%@ page contentType="text/xml;charset=utf-8" %>
<%@ page import="com.xiaonei.services.AutoComplete.*" %>
<%@ page import="java.io.*" %>
<%@ page import="java.util.*" %>
<%!
	private static AutoCompleteType[] type = new AutoCompleteType[]{AutoCompleteType.WORKPLACENAME};
%>
<%
	com.xiaonei.reg.guide.util.GuideTimeCost tc = com.xiaonei.reg.guide.util.GuideTimeCost.begin();
	response.setHeader("Pragma","No-cache");
	response.setHeader("Cache-Control","no-cache");
	response.setDateHeader("Expires", 0);
	String q = request.getParameter("query");
	Entry[] r;
	tc.endStep("Before Search");
	try{
	    r = AutoCompleteAdapter.getInstance().getAutoCompleteManager().suggest(type, q, 10);
	}catch(Exception e){
	    r = new Entry[]{new Entry(0, "数据下载中...")};
	}
	tc.endStep("After Search");
%>
<?xml version="1.0" encoding="UTF-8"?>
<ResultSet>
<%
	tc.endStep("Before For");
	StringBuffer result = new StringBuffer();
	for(Entry entry:r){
	    result.append("<Result>");
	    result.append("<Id>").append(entry.id).append("</Id>");
	    result.append("<Name><![CDATA[").append(entry.name).append("]]></Name>");
	    result.append("</Result>");
	}
	out.println(result.toString());
	tc.endFinally();
%>
</ResultSet>

