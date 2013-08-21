<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="avatar"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}"><img src="${item.tiny_Url}" alt="${item.tiny_Url} (${item.name})" /></a></p>
<div class="info">
	<dl>
		<dt>姓名:</dt>
		<dd><a href="${applicationScope.urlMain}/profile.do?id=${item.id}">${item.name}</a><c:if test="${item.selected==true}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></dd>
		<!--先设定一个networkid_的变量为网络ID -->
		<c:set var="networkid_" value="${item.univId}" />
		<dt><%@ include file="/inc/network.inc" %></dt>
		<dd>${item.univName}</dd>
	</dl>
</div>
