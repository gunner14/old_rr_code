<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page import="com.xiaonei.reg.register.logic.additional.*" %>
<%@page import="org.apache.commons.lang.StringUtils"%>
<%
String actionId = request.getParameter("action_id");
if(StringUtils.equals("205214",actionId)){
%>
<!--Register成功页面
将以下代码 嵌入注册成功页面 HTML代码尾部
代码中的 ***** 部分 请动态输出用户注册后获得的唯一ID
-->

<script language="javascript" src="http://tmtest2.admaster.com.cn/sod/admBcnAct2.js"></script>
<script language="javascript">var cmpnId=115,actInfo="<%=java.util.UUID.randomUUID() %>",weight=1,tagStr="161"; admBcnActGen(cmpnId,actInfo,weight,tagStr);</script>
<%} %>