<c:if test="${item.ptype==10}">
                <li><a href="${applicationScope.urlPage}/${item.id}" target="_blank">查看公共主页</a></li>
				<c:if test="${!empty visiter.id}">
                <li><a href="#joinFas" onclick="XN.app.search.joinFans(${item.id})">加为好友</a></li>
                </c:if>
</c:if>
<c:if test="${item.ptype==20}">
                <li><a href="${applicationScope.urlOrg}/${item.id}" target="_blank">查看俱乐部</a></li>
				<c:if test="${!empty visiter.id}">
                <li><a href="#joinFas" onclick="XN.app.search.joinFans(${item.id},true)">加为好友</a></li>
                </c:if>
</c:if>
