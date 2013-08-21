<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="avatar"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}" target="_blank"><img alt="${item.head_Url} (${item.name})" src="${item.head_Url}"/></a></p>

<%--<c:if test="${requestScope.curStage!=item.curNetworkStage}">	
<p class="avatar"><img alt="" src="${item.head_Url}"/></p>
</c:if>--%>
<div class="info">
<%--
	<c:choose>
		<c:when test="${requestScope.curStage==item.curNetworkStage}">--%>
	<dl>		
		<dt>姓名:</dt>
	    <dd><strong><a href="${applicationScope.urlMain}/profile.do?id=${item.id}" target="_blank">${item.name}</a></strong><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></dd>
	  <dt>网络:</dt>
	  	<c:if test="${!empty item.workList}">
	  	<c:forEach var="work" items="${item.workList}">
	  	<dd><a href="${applicationScope.urlBrowse}/Search.do?action=work&univ=${work.workplaceId}">${work.workplaceName}</a></dd>
	  	<dt>&nbsp;</dt>
	  	</c:forEach>	  	
	  	</c:if>
	  	<c:if test="${!empty item.uList}">
	  	<c:forEach var="univ" items="${item.uList}">
	  	<dd><a href="${applicationScope.urlBrowse}/Search.do?action=university&amp;univ=${univ.universityId}">${univ.universityName}</a>  <c:if test="${univ.enrollYear > 0}"><a href="${applicationScope.urlBrowse}/Search.do?action=year&amp;year=${univ.enrollYear}&amp;univ=${univ.universityId}">${univ.enrollYear}</a></c:if></dd>
	  	<dt>&nbsp;</dt>
	  	</c:forEach>	  	
	  	</c:if>
	  	<c:if test="${!empty item.hsList}">
	  	<c:forEach var="highsch" items="${item.hsList}">
	  	<dd><a href="${applicationScope.urlBrowse}/s.do?action=hs&sType=0&hs_highschool=<str:encodeUrl>${highsch.highSchoolName}</str:encodeUrl>">${highsch.highSchoolName}</a> <c:if test="${highsch.enrollYear > 0}"><a href="${applicationScope.urlBrowse}/Search.do?action=year&univ=${highsch.highSchoolId}&year=${highsch.enrollYear}">${highsch.enrollYear}</a></c:if></dd>
	  	<dt>&nbsp;</dt>
	  	</c:forEach>
	  	</c:if>	  	
	</dl>
	<%--
		</c:when>
		<c:otherwise>
			<dl>		
		<dt>姓名:</dt>
	    <dd><strong>${item.name}</strong><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></dd>
	  <dt>网络:</dt>
	  	<c:if test="${!empty item.workList}">
	  	<c:forEach var="work" items="${item.workList}">
	  	<dd>${work.workplaceName}</dd><dt>&nbsp;</dt>
	  	</c:forEach>	  	
	  	</c:if>
	  	<c:if test="${!empty item.uList}">
	  	<c:forEach var="univ" items="${item.uList}">
	  	<dd>${univ.universityName}  <c:if test="${univ.enrollYear > 0}">${univ.enrollYear}</c:if></dd><dt>&nbsp;</dt>
	  	</c:forEach>
	  	
	  	</c:if>
	  	<c:if test="${!empty item.hsList}">
	  	<c:forEach var="highsch" items="${item.hsList}">
	  	<dd>${highsch.highSchoolName} <c:if test="${highsch.enrollYear > 0}">${highsch.enrollYear}</c:if></dd>
	  	<dt>&nbsp;</dt>
	  	</c:forEach>
	  	</c:if>	  	
	</dl>
		</c:otherwise>
		</c:choose>
		--%>
</div>
