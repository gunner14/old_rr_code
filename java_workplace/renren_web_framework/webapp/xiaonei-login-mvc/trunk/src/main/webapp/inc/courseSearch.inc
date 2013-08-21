<%@ page pageEncoding="UTF-8" %>
<div class="box" id="searchbar">
	<h3>搜索课程</h3>
	<div class="boxcont">
		<form method="get" action="${applicationScope.urlBrowse}/Search.do">
			<p>
				<input type="hidden" value="1" name="course" />
				<input type="text" name="either" maxlength="15" id="keyword" class="inputtext" />
			</p>
			<p class="operation"><input type="submit" name="submit" class="subbutton" id="submit" value="搜索" /></p>
			<input type="hidden" value="course" name="action">
			<c:choose>
				<c:when test="${empty requestScope.university.id}">
					<c:set var="univid" value="${requestScope.course.univId}"/>
				</c:when>
				<c:otherwise>
					<c:set var="univid" value="${requestScope.university.id}"/>
				</c:otherwise>
			</c:choose>
			<input type="hidden" name="univ" value="${univid}">
		</form>
	</div>
</div>
