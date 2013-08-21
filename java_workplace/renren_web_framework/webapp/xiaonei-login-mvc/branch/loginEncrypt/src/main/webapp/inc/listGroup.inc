<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="image"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}"><img src="${item.head_Url}" alt="${item.name}" /></a></p>
<table class="info">
	<caption><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}">${item.name}</a></caption>
	<tr><th>人数:</th><td>${item.userCount}</td></tr>
</table>
