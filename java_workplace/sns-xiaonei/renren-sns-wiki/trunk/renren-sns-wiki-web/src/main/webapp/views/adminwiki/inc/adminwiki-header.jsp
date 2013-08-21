<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring"%>
<div id="wiki-adminheader">
<form action="/adminwiki/list" method="post">
	<input type="hidden" name="type" vlaue="2">
	wikiId:<input name="wikiIdStr" value="${wikiIdStr }" />
	wiki名称:<input name="wikiName" value="${wikiName }" /><br>
	是否高亮:
	<select name="light">
		<option value="true" <c:if test="${light }">selected="selected"</c:if>>高亮</option>
		<option value="false" <c:if test="${!light }">selected="selected"</c:if>>全部</option>
	</select>
	类型:<select name="wikiType">
	<option value="0">全部</option>
	<option value="1" <c:if test="${wikiType==1 }">selected="selected"</c:if>>电影</option>
	<option value="2" <c:if test="${wikiType==2 }">selected="selected"</c:if>>书籍</option>
	<option value="3" <c:if test="${wikiType==3 }">selected="selected"</c:if>>游戏</option>
	<option value="4" <c:if test="${wikiType==4 }">selected="selected"</c:if>>景点</option>
</select> <input type="submit" value="确定"></form>
</div>