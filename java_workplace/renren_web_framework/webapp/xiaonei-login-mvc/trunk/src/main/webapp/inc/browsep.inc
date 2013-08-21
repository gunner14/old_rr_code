<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<c:if test="${empty itemPerPage}" >
	<c:set var="itemPerPage" value="20" />
</c:if>
<c:choose>
	<c:when test="${empty requestScope.curpage}">
		<c:set var="currentPage" value="0"/>
	</c:when>
	<c:otherwise>
		<c:set var="currentPage" value="${requestScope.curpage}"/> 
	</c:otherwise>
</c:choose>
<c:set var="numberMod" value="${navCount % itemPerPage}"/>
<c:set var="countInt" value="${navCount-numberMod}"/>
<c:choose>
	<c:when test="${numberMod==0}">
		<fmt:parseNumber value="${countInt/itemPerPage}" integerOnly="true" var="pageNum" />
	</c:when>
	<c:otherwise>
		<fmt:parseNumber value="${countInt/itemPerPage + 1}" integerOnly="true" var="pageNum" />
	</c:otherwise>
</c:choose>
<c:if test="${pageNum<7}">
	<c:if test="${currentPage>0}">
		<a title="上一页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; 上一页</a>
	</c:if>
	<c:if test="${currentPage>4}">
		<a title="跳到第${currentPage-4}页" href="/${navUrl}?curpage=${currentPage-5}${navUrlEnd}">${currentPage-4}</a>
	</c:if>
	<c:if test="${currentPage>3}">
		<a title="跳到第${currentPage-3}页" href="/${navUrl}?curpage=${currentPage-4}${navUrlEnd}">${currentPage-3}</a>
	</c:if>
	<c:if test="${currentPage>2}">
		<a title="跳到第${currentPage-2}页" href="/${navUrl}?curpage=${currentPage-3}${navUrlEnd}">${currentPage-2}</a>
	</c:if>
	<c:if test="${currentPage>1}">
		<a title="跳到第${currentPage-1}页" href="/${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a>
	</c:if>
	<c:if test="${currentPage>0}">
		<a title="跳到第${currentPage}页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a>
	</c:if>
	<span class="this-page">${currentPage+1}</span>
	<c:if test="${currentPage<pageNum-1}">
		<a title="跳到第${currentPage+2}页" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-2}">
		<a title="跳到第${currentPage+3}页" href="/${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-3}">
		<a title="跳到第${currentPage+4}页" href="/${navUrl}?curpage=${currentPage+3}${navUrlEnd}">${currentPage+4}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-4}">
		<a title="跳到第${currentPage+5}页" href="/${navUrl}?curpage=${currentPage+4}${navUrlEnd}">${currentPage+5}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-5}">
		<a title="跳到第${currentPage+6}页" href="/${navUrl}?curpage=${currentPage+5}${navUrlEnd}">${currentPage+6}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-1}">
		<a title="下一页" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">下一页 &gt;</a>
	</c:if>
</c:if>
<c:if test="${pageNum>6}">
	<c:if test="${currentPage>-1 && currentPage<pageNum-3}">
		<c:if test="${currentPage>2}">
			<a title="第一页" href="/${navUrl}?curpage=0${navUrlEnd}">&laquo; 第一页</a>
			<span class="break">...</span>
		</c:if>
		<c:if test="${currentPage>0}">
			<a title="上一页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; 上一页</a>
		</c:if>
		<c:if test="${currentPage>1}">
			<a title="跳到第${currentPage-1}页" href="/${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a>
		</c:if>
		<c:if test="${currentPage>0}">
			<a title="跳到第${currentPage}页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a>
		</c:if>
		<span class="this-page">${currentPage+1}</span>
		<a title="跳到第${currentPage+2}页" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a>
		<a title="跳到第${currentPage+3}页" href="/${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a>
		<c:if test="${currentPage<2}">
			<a title="跳到第${currentPage+4}页" href="/${navUrl}?curpage=${currentPage+3}${navUrlEnd}">${currentPage+4}</a>
		</c:if>
		<c:if test="${currentPage<1}">
			<a title="跳到第${currentPage+5}页" href="/${navUrl}?curpage=${currentPage+4}${navUrlEnd}">${currentPage+5}</a>
		</c:if>
		<a title="下一页" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">下一页 &gt;</a>
		<span class="break">...</span>
		<a title="最后页" href="/${navUrl}?curpage=${pageNum-1}${navUrlEnd}">最后页 &raquo;</a>
	</c:if>
	<c:if test="${(currentPage < pageNum - 0)&&(currentPage>pageNum-4)}">
		<a title="第一页" href="/${navUrl}?curpage=0${navUrlEnd}">&laquo; 第一页</a>
		<span class="break">...</span>
		<a title="上一页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; 上一页</a>
		<c:if test="${currentPage>pageNum-2}">
			<a title="跳到第${currentPage-3}页" href="/${navUrl}?curpage=${currentPage-4}${navUrlEnd}">${currentPage-3}</a>
		</c:if>
		<c:if test="${currentPage>pageNum-3}">
			<a title="跳到第${currentPage-2}页" href="/${navUrl}?curpage=${currentPage-3}${navUrlEnd}">${currentPage-2}</a>
		</c:if>
		<a title="跳到第${currentPage-1}页" href="/${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a>
		<a title="跳到第${currentPage}页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a>
		<span class="this-page">${currentPage+1}</span>
		<c:if test="${currentPage<pageNum-1}">
			<a title="跳到第${currentPage+2}页" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a>
		</c:if>
		<c:if test="${currentPage<pageNum-2}">
			<a title="跳到第${currentPage+3}页" href="/${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a>
		</c:if>
		<c:if test="${currentPage<pageNum-1}">
			<a title="下一页" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">下一页 &gt;</a>
		</c:if>
		<c:if test="${currentPage<pageNum-3}">
			<span class="break">...</span>
			<a title="最后页" href="/${navUrl}?curpage=${pageNum-1}${navUrlEnd}">最后页 &raquo;</a>
		</c:if>
	</c:if>
</c:if>
