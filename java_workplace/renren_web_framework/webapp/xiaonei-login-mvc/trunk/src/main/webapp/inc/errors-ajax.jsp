<%--
ajax提交时返回的错误页面，可以通用，需要定义一个type，一个message,应该把用到的样式放到base.css中,并且可以自适应
type可扩展
--%>
<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:if test="${type=='error'}">
<div id="ajax_msgerror" class="errors_div ajax_msgerror">${message}</div>
</c:if>
<c:if test="${type=='sharecomment'}">
<div id="ajax_msgerror" class="errors_div ajax_msgerror" style="">${message}</div>
</c:if>