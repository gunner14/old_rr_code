<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="avatar"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}"><img src="${item.head_Url}" alt="${item.head_Url} (${item.name})" /></a></p>
<div class="info" id="friend_info_${item.id}">
	<dl>		
		<dt>姓名:</dt>
		<dd><a href="${applicationScope.urlMain}/profile.do?id=${item.id}">${item.name}</a><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></dd>
			

		<c:choose>
			<c:when test="${item.stage==20}">
				<dt>网络:</dt>
				<dd>${item.univName}<c:if test="${item.univYear > 0}"><a href="${applicationScope.urlBrowse}/Srch.do?action=uyear&amp;year=${item.univYear}&amp;univ=<str:encodeUrl>${item.univName}</str:encodeUrl>">${item.univYear}</a></c:if></dd>
			</c:when>
			<c:when test="${item.stage==10}">
				<dt>网络:</dt>
				<dd>${item.univName}<c:if test="${item.univYear > 0}"><a href="${applicationScope.urlBrowse}/ns.do?highschool_name=<str:encodeUrl>${item.univName}</str:encodeUrl>&yr=${item.univYear}&action=highschool">${item.univYear}</a></c:if></dd>			
			</c:when>	
			<c:otherwise>
				<dt>网络:</dt>
				<dd>${item.univName}</dd>
			</c:otherwise>
		</c:choose>	

		<dt>描述:</dt>
		<c:choose>
			<c:when test="${item.confirmed == 2}">
				<dd>等待你确认</dd>
			</c:when>
			<c:when test="${item.confirmed == 1}">
				<dd>等待对方确认</dd>
			</c:when>		
			<c:when test="${item.confirmed == 3}">				
			</c:when>			
			<c:otherwise>
				<dd>[<a href="#nogo" onclick="javascript:showFriendMemoDialog(${item.id});">你是如何认识${item.name}的？</a>]</dd>
			</c:otherwise>
		</c:choose>
		
		<c:if test="${item.confirmed != 3}">
			<dt>&nbsp;</dt>
		</c:if>
		
		<c:forTokens var="sub_item" delims="|" items="${item.friend_memo}">
				<c:choose>
					<c:when  test="${item.confirmed != 3}">
						<dd class="unconfirm">
					</c:when>
					<c:otherwise>
						<dd>
					</c:otherwise>
			  </c:choose>
					你们${sub_item}
				</dd>
			<dt>&nbsp;</dt>
		</c:forTokens>
		
		<dd> 
			<c:choose>
				<c:when test="${!empty item.friend_memo}">
					<c:if test="${item.confirmed == 2}">[<a href="#nogo" onclick="javascript:confirmMemo('${item.id}');">确认</a>] 或 </c:if>[<a href="#nogo" onclick="javascript:showFriendMemoDialog(${item.id});">编辑</a>]
				</c:when>
				<c:otherwise>					
				</c:otherwise>
			</c:choose>
		</dd>
	</dl>
</div>
