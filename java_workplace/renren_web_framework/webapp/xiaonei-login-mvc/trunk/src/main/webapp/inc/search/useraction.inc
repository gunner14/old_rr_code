<c:if test="${!empty visiter.id}">
<li><a href="#addFriend" onclick="showRequestFriendDialog('${item.id}','${item.name}','${item.tinyUrl}',true,'${requestScope.ref}${offset + status.index}')">加为好友</a></li>
</c:if>
<li><a href="${applicationScope.urlMsg}/SendMessage.do?id=${item.id}">发站内信</a></li>
