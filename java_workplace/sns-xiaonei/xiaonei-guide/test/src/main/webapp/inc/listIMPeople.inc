<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
 
<c:choose>
	<c:when test="${empty item.univ}">
		<p class="image"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}" target="_blank"><img src="${item.head_Url}" alt="${item.head_Url} (${item.name})" /></a></p>
		<div class="info">
			<h4><a href="${applicationScope.urlMain}/profile.do?id=${item.id}" target="_blank">${item.name}</a><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><c:if test="${item.status == 1}" > - 在线 :)</c:if></h4>
			<ul class="subinfo">
				<li>入学:<c:if test="${item.univYear > 0}"><a href="${applicationScope.urlBrowse}/searchim.do?action=year&amp;year=${item.univYear}&amp;univ=${item.univId}">${item.univYear}</a></c:if></li>
				<li>宿舍:<a href="${applicationScope.urlBrowse}/searchim.do?action=dorm&amp;dorm=<str:encodeUrl>${item.dorm}</str:encodeUrl>&amp;univ=${item.univId}">${item.dorm}</a></li>
				<li>院系:<a href="${applicationScope.urlBrowse}/searchim.do?action=university&amp;univ=${item.univId}">${item.univName}</a><c:if test="${!empty item.department}"><span class="pipe">-</span><a href="${applicationScope.urlBrowse}/searchim.do?action=department&amp;department=<str:encodeUrl>${item.department}</str:encodeUrl>&amp;univ=${item.univId}">${item.department}</a></c:if></li>
				<li>浏览:<a href="${applicationScope.urlPhoto}/getalbumlist.do?id=${item.id}">相册</a><span class="pipe">|</span><a href="${applicationScope.urlBlog}/GetBlog.do?id=${item.id}">日志</a><span class="pipe">|</span><a href="${applicationScope.urlFriend}/GetFriendList.do?id=${item.id}">好友</a></li>
			</ul>
		</div>
	</c:when>
	<c:otherwise>
		<p class="image"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}" target="_blank"><img src="${item.head_Url}" alt="${item.head_Url} (${item.name})" /></a></p>
		<div class="info">
			<h4><a href="${applicationScope.urlMain}/profile.do?id=${item.id}" target="_blank">${item.name}</a><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><c:if test="${item.status == 1}" > - 在线 :)</c:if></h4>
			<ul class="subinfo">
				<li>入学:<c:if test="${requestScope.univYear > 0}"><a href="${applicationScope.urlBrowse}/searchim.do?action=year&amp;year=${item.univYear}&amp;univ=${item.univ.id}">${item.univYear}</a></c:if></li>
				<li>宿舍:<a href="${applicationScope.urlBrowse}/searchim.do?action=dorm&amp;dorm=<str:encodeUrl>${item.dorm}</str:encodeUrl>&amp;univ=${item.univ.id}">${item.dorm}</a></li>
				<li>院系:<a href="${applicationScope.urlBrowse}/searchim.do?action=university&amp;univ=${item.univ.id}">${item.univ.name}</a><c:if test="${!empty item.department}"><span class="pipe">-</span><a href="${applicationScope.urlBrowse}/searchim.do?action=department&amp;department=<str:encodeUrl>${item.department}</str:encodeUrl>&amp;univ=${item.univ.id}">${item.department}</a></c:if></li>
				<li>浏览:<a href="${applicationScope.urlPhoto}/getalbumlist.do?id=${item.id}">相册</a><span class="pipe">|</span><a href="${applicationScope.urlBlog}/GetBlog.do?id=${item.id}">日志</a><span class="pipe">|</span><a href="${applicationScope.urlFriend}/GetFriendList.do?id=${item.id}">好友</a></li>
			</ul>
		</div>
	</c:otherwise>
</c:choose>
