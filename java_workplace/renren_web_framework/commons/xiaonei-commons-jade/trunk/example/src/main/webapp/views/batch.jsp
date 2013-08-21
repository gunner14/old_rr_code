<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="/WEB-INF/tlds/c.tld" prefix="c" %>

<html>

<head>

<meta http-equiv="Content-Type" content="UTF-8" />

</head>

<body>

<h2>Rose-Jade Example - 批量增加联系人</h2>

<form method="POST" action="batch_insert">

<p>用户 ID：<input type="text" name="userid" value="${userid}" /></p>

<hr/>

<p>联系人姓名 #1：<input type="text" name="name1" /></p>

<p>联系人号码 #1：<input type="text" name="phonecode1" /></p>

<hr/>

<p>联系人姓名 #2：<input type="text" name="name2" /></p>

<p>联系人号码 #2：<input type="text" name="phonecode2" /></p>

<hr/>

<p>联系人姓名 #3：<input type="text" name="name3" /></p>

<p>联系人号码 #3：<input type="text" name="phonecode3" /></p>

<hr/>

<p>联系人姓名 #4：<input type="text" name="name4" /></p>

<p>联系人号码 #4：<input type="text" name="phonecode4" /></p>

<hr/>

<p>联系人姓名 #5：<input type="text" name="name5" /></p>

<p>联系人号码 #5：<input type="text" name="phonecode5" /></p>

<input type="submit" name="Update" value="批量增加" />

</form>

<input type="button" name="Back" value="返回" onclick="history.back()" />

</body>

</html>
