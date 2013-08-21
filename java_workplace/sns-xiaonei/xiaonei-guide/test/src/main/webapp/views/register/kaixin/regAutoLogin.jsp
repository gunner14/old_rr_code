<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<script type="text/javascript">

</script>
<title>开心网 - 激活</title>
</head>
<body>
<%@ include file="inc/active_stat.jsp" %>
<%
response.sendRedirect("http://home.kaixin.com/Home.do");
%>
</body>
</html>