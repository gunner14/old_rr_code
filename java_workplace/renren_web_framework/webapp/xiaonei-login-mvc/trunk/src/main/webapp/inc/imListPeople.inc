<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<c:choose>
	<c:when test="${!empty requestScope.userList}">
		<c:choose>
			<c:when test="${!empty requestScope.hostid}">
				<c:set var="loginerId" value="${requestScope.hostid}"/>
			</c:when>
			<c:otherwise>
				<c:set var="loginerId" value="0"/>
			</c:otherwise>
		</c:choose>    
		<ol class="peoplelist">
				<li>
				<c:forEach varStatus="status" var="item" items="${requestScope.userList}"> 
					<div class="people">
						<p class="image"><a target="_blank" href="${applicationScope.urlMain}/profile.do?portal=userSocialNet&id=${item.id}"><img src="${item.fullHeadUrl}" alt="${item.fullHeadUrl} (${item.name})" /></a></p>
						<div class="info">
							<h4><a target="_blank" href="${applicationScope.urlMain}/profile.do?portal=userSocialNet&id=${item.id}">${item.name}</a></h4>
							<ul class="subinfo">
								<li>加入:<c:if test="${item.univYear > 0}"><a target="_blank" href="${applicationScope.urlBrowse}/Search.do?action=year&amp;year=${item.univYear}&amp;univ=${item.univId}">${item.univYear}</a></c:if></li>
								<li>网络:<a target="_blank" href="${applicationScope.urlBrowse}/Search.do?action=university&amp;univ=${item.univId}">${item.univName}</a></li>
								<li>浏览:<a target="_blank" href="${applicationScope.urlPhoto}/getalbumlist.do?id=${item.id}">相册</a><span class="pipe">|</span><a target="_blank" href="${applicationScope.urlBlog}/GetBlog.do?id=${item.id}">日志</a><span class="pipe">|</span><a target="_blank" href="${applicationScope.urlFriend}/GetFriendList.do?id=${item.id}">好友</a></li>
							</ul>
						</div>
						<div class="actions">
							<ul>
								<li><a class="chat" href="#" onclick="javascript:talkto(event, ${visiter.id}, ${item.id}, '${requestScope.visiter.email}', '${item.name}');return false">和<c:choose><c:when test="${item.gender=='男生'}">他</c:when><c:otherwise>她</c:otherwise></c:choose>聊天</a></li>
								<li><a class="add" href="javascript:showRequestFriendDialog('addFriend','${item.id }','${item.name }', '${item.fullHeadUrl}')">加为好友</a></li>
							</ul>
						</div>
					</div>
				</li>
				</c:forEach>
			</ol>
	</c:when>

</c:choose>
