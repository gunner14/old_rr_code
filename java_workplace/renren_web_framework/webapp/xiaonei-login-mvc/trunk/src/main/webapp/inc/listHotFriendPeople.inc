<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="avatar"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}"><img src="${item.head_Url}" alt="${item.head_Url} (${item.name})" /></a></p>
<div class="info">
	<dl>
		<dt>姓名:</dt>
		<dd><a href="${applicationScope.urlMain}/profile.do?id=${item.id}">${item.name}</a><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></dd>
		
		<c:set var="networkid_" value="${item.univId}" />
		<dt><%@ include file="/inc/network.inc" %>:</dt>
		<dd>${item.univName}</dd>
		<dt>描述:</dt>
		<dd>
			<c:choose>
			<c:when test="${item.hotfriend_desc == ''}">
				<a href="${applicationScope.urlFriend}/showhotfrienddesc.do?hf_id=${item.id}&hf_name=<str:encodeUrl>${item.name}</str:encodeUrl>">描述我的特别好友</a>
			</c:when>
			<c:otherwise>
				${item.hotfriend_desc}<a href="${applicationScope.urlFriend}/showhotfrienddesc.do?hf_id=${item.id}&hf_name=<str:encodeUrl>${item.name}</str:encodeUrl>&hfDesc=<str:encodeUrl>${item.hotfriend_desc}</str:encodeUrl>"> [编辑]</a>
			</c:otherwise>
			</c:choose>			
		</dd>
	</dl>
</div>
