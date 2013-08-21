<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<li>
	<p class="avatar"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}"><img src="${item.headurl}" alt="${item.name}" /></a></p>
	<div class="info">
		<dl>		
			<dt>群组:</dt>
			<dd><strong><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}" target="_blank">${item.name}</a></strong></dd>
			<dt>人数:</dt>
			<dd>
				<c:choose>
					<c:when test="${item.userCount>50000}">
						50000
					</c:when>
					<c:otherwise>
						${item.userCount}
					</c:otherwise>
				</c:choose>
			</dd>
			<dt>&nbsp;</dt>						
		</dl>
	</div>
	<ul class="actions">
		<li><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}" target="_blank">浏览群组</a></li>
	</ul>
</li>