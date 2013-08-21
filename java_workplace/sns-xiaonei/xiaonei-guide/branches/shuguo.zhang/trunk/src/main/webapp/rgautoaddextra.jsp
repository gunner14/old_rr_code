<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%>
<%@page import="java.util.Date"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheFactory"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@page import="net.spy.memcached.MemcachedClient"%>
<%@page import="com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="java.util.HashMap"%><html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Take Care!</title>
</head>
<body>
    <form method="get" action="">
        <fieldset>
        <input type="text" id="s" name="id" value="0" /> 
        <input type="text" id="s" name="key" value="reg_page_lover" /> 
        <input type="text" id="s" name="value" value="600039359" />
        <input type="hidden" name="run" value="true">
        <input type="submit" id="x" value="Search" />
        <input type="button" id="x" value="Clear" onclick="resetForm()"/>
        <br />
        </fieldset>
    </form>
<%
if(!"true".equals(request.getParameter("run"))){
}
else{
    String sid = request.getParameter("id").trim();
    int id = (Integer)MyStringUtils.stringToObject(sid,Integer.class);
    User host = SnsAdapterFactory.getUserAdapter().get(id);
    String key = request.getParameter("key").trim();
    String value = request.getParameter("value").trim();
    if(!MyStringUtils.isNullOrEmpty(key) && !MyStringUtils.isNullOrEmpty(value)){
    	RegExtraMsg msg = new RegExtraMsg();
    	msg.setInput_key(key);
    	msg.setInput_value(value);
    	Map<String,RegExtraMsg> msgMap = new HashMap<String,RegExtraMsg> ();
    	msgMap.put(key,msg);
    	RegExtraMsgLogic.getInstance().saveMsg(host,msgMap);
    	out.println("save ok");
    }
    
}
%>

</body>
</html>