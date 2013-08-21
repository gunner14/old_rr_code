<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<!doctype html>
<html>
<head>
	<title>wiki运营后台</title>
	<link rel="stylesheet" type="text/css" href="http://wiki.renren.com/views/adminwiki/css/wiki.css" />
</head>
<body>
	<div class="pagehead">
		<div class="headcenter">
			<h1><a href="http://www.renren.com/"><img width=160 height="31" src="http://s.xnimg.cn/imgpro/v6/logo.png" alt="人人网" /></a></h1>
		</div>
	</div>
	<div class="pagecontent clearfix">
		<%@include file="inc/adminwiki-nav.jsp" %>
		<div class="right-con">
<%@include file="inc/adminwiki-header.jsp"%>
<c:choose>
	<c:when test="${fn:length(wikiList) > 0}">
		<table id="adminwiki-list">
			<thead>
				<tr>
					<th>wikiId</th>
					<th>wikiName</th>
					<th>修改资料</th>
					<th>修改头像</th>
					<th>修改头图</th>
					<th>内容管理</th>
				</tr>
				</tr>
			</thead>
			<tbody>
				<c:forEach items="${wikiList}" var="wiki">
					<tr>
						<td>${wiki.id }</td>
						<td><a href="http://wiki.renren.com/${wiki.id}"
							target="_blank">${wiki.name }</a></td>
						<td><a
							href="http://wiki.renren.com/adminwiki/modifyWikiInfo/${wiki.id}?light=${light}&wikiType=${wikiType}">修改资料</a></td>
						<td><a
							href="http://wiki.renren.com/adminwiki/modifyWikiHead/${wiki.id}?light=${light}&wikiType=${wikiType}">修改头像</a></td>
						<td><a
							href="http://wiki.renren.com/adminwiki/modifyWikiCover/${wiki.id}?light=${light}&wikiType=${wikiType}">修改Cover</a></td>
						<td><a
							href="http://wiki.renren.com/adminwiki/${wiki.id}/fdeditor">内容管理</a></td>
					</tr>
				</c:forEach>
			</tbody>
		</table>
	</c:when>
	<c:otherwise>
		未查询到wiki
	</c:otherwise>
</c:choose>
<script type="text/javascript">
function turnPage(page){
	var form = document.getElementById('turnPage');
	form.page.value = page;
	form.submit();
}
</script>
<form id="turnPage" action="http://wiki.renren.com/adminwiki/list">
	<input type="hidden" name="light" value="${light }" /> 
	<input type="hidden" name="wikiType" value="${wikiType }" /> 
	<input type="hidden" name="curPage" value="${curPage }" /> 
	<input type="hidden" name="endId" value="${endId }" />
	<input type="hidden" name="pageSize" value="${pageSize}" /> 
	<c:choose>
	<c:when test="${curPage>0}">
		<a href="javascript:void(0);" onclick="turnPage(${curPage-1})">上一页</a>
	</c:when>
	<c:otherwise>
		上一页
	</c:otherwise>
	</c:choose>
	<c:choose>
	<c:when test="${fn:length(wikiList) >= pageSize}">
	<a href="javascript:void(0);" onclick="turnPage(${curPage+1})">下一页</a>
	</c:when>
	<c:otherwise>
		下一页
	</c:otherwise>
	</c:choose>
<input type="text" name="page" value="${curPage}" width="10" /><input
	type="submit" value="跳转" /></form>
		</div>
	</div>
</body>
</html>