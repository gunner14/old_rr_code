<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<li>
	<p class="avatar"><a href="${applicationScope.urlMain}/profile.do?portal=fsearch&from=s&sid=${status.index}&id=${item.id}&" target="_blank"><img alt="${item.headurl}" src="${item.headurl}"/></a></p>
	<div class="info">
		<dl>		
			<dt>姓名:</dt>
			<dd><strong><a href="${applicationScope.urlMain}/profile.do?portal=fsearch&from=s&sid=${status.index}&id=${item.id}&" target="_blank">${item.name}</a></strong>
			<c:if test="${item.selected==1}"><span class="rank"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级"/></span></c:if>
			<c:if test="${item.status_config==1}"><span class="rank hide-png"><a href="${applicationScope.urlI}/click.action?from=120041&url=%2F" target="_blank"><img src="${applicationScope.urlSmallStatic}/img/newsfeed/purple_user.png" alt="紫豆" /></a></span><!--[if IE 6]><span style="display: inline-block; FILTER:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='${applicationScope.urlSmallStatic}/img/newsfeed/purple_user.png', sizingMethod='crop'; width:12px; height:12px"></span><![endif]--></c:if>
			</dd>
			<dt style="visibility:hidden">网络 :</dt>
			<c:forEach var="network" items="${item.networks}">
			<dd>${network.networkName} ${network.year}</dd>
			<dt>&nbsp;</dt>
			</c:forEach>							
		</dl>
	</div>
	<ul class="actions">
		<li><a href="${applicationScope.urlMain}/Poke.do?id=${item.id}" onclick="return doPoke(event,'${item.id}','${item.name}')">打个招呼</a></li>
		<li><a href="${applicationScope.urlMsg}/SendMessage.do?id=${item.id}&amp;from=<str:encodeUrl>搜索结果</str:encodeUrl>&amp;oriUrl=<str:encodeUrl>${requestScope.origURL}</str:encodeUrl>">发站内信</a></li>
		<c:if test="${(item.id != requestScope.hostid)}"><li><a href="#nogo" onclick="javascript:talkto(${item.id}, '${item.name}');return false;">即时聊天</a></li></c:if>
		<li><%--<a href="${applicationScope.urlMain}/RequestFriend.do?id=${item.id}&from=s" target="_blank">--%><a id="addFriend${item.id}" onclick="showRequestFriendDialog('addFriend${item.id}','${item.id}','${item.name}','${item.headurl}')" href="#nogo">加为好友</a></li>
		<li><a href="${applicationScope.urlFriend}/GetFriendList.do?id=${item.id}">查看好友</a></li>
	</ul>
</li>