<%@ page pageEncoding="GBK" %>
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
		<a title="��һҳ" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; ��һҳ</a>
	</c:if>
	<c:if test="${currentPage>4}">
		<a title="������${currentPage-4}ҳ" href="/${navUrl}?curpage=${currentPage-5}${navUrlEnd}">${currentPage-4}</a>
	</c:if>
	<c:if test="${currentPage>3}">
		<a title="������${currentPage-3}ҳ" href="/${navUrl}?curpage=${currentPage-4}${navUrlEnd}">${currentPage-3}</a>
	</c:if>
	<c:if test="${currentPage>2}">
		<a title="������${currentPage-2}ҳ" href="/${navUrl}?curpage=${currentPage-3}${navUrlEnd}">${currentPage-2}</a>
	</c:if>
	<c:if test="${currentPage>1}">
		<a title="������${currentPage-1}ҳ" href="/${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a>
	</c:if>
	<c:if test="${currentPage>0}">
		<a title="������${currentPage}ҳ" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a>
	</c:if>
	<span class="this-page">${currentPage+1}</span>
	<c:if test="${currentPage<pageNum-1}">
		<a title="������${currentPage+2}ҳ" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-2}">
		<a title="������${currentPage+3}ҳ" href="/${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-3}">
		<a title="������${currentPage+4}ҳ" href="/${navUrl}?curpage=${currentPage+3}${navUrlEnd}">${currentPage+4}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-4}">
		<a title="������${currentPage+5}ҳ" href="/${navUrl}?curpage=${currentPage+4}${navUrlEnd}">${currentPage+5}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-5}">
		<a title="������${currentPage+6}ҳ" href="/${navUrl}?curpage=${currentPage+5}${navUrlEnd}">${currentPage+6}</a>
	</c:if>
	<c:if test="${currentPage<pageNum-1}">
		<a title="��һҳ" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">��һҳ &gt;</a>
	</c:if>
</c:if>
<c:if test="${pageNum>6}">
	<c:if test="${currentPage>-1 && currentPage<pageNum-3}">
		<c:if test="${currentPage>2}">
			<a title="��һҳ" href="/${navUrl}?curpage=0${navUrlEnd}">&laquo; ��һҳ</a>
			<span class="break">...</span>
		</c:if>
		<c:if test="${currentPage>0}">
			<a title="��һҳ" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; ��һҳ</a>
		</c:if>
		<c:if test="${currentPage>1}">
			<a title="������${currentPage-1}ҳ" href="/${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a>
		</c:if>
		<c:if test="${currentPage>0}">
			<a title="������${currentPage}ҳ" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a>
		</c:if>
		<span class="this-page">${currentPage+1}</span>
		<a title="������${currentPage+2}ҳ" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a>
		<a title="������${currentPage+3}ҳ" href="/${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a>
		<c:if test="${currentPage<2}">
			<a title="������${currentPage+4}ҳ" href="/${navUrl}?curpage=${currentPage+3}${navUrlEnd}">${currentPage+4}</a>
		</c:if>
		<c:if test="${currentPage<1}">
			<a title="������${currentPage+5}ҳ" href="/${navUrl}?curpage=${currentPage+4}${navUrlEnd}">${currentPage+5}</a>
		</c:if>
		<a title="��һҳ" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">��һҳ &gt;</a>
		<span class="break">...</span>
		<a title="���ҳ" href="/${navUrl}?curpage=${pageNum-1}${navUrlEnd}">���ҳ &raquo;</a>
	</c:if>
	<c:if test="${(currentPage < pageNum - 0)&&(currentPage>pageNum-4)}">
		<a title="��һҳ" href="/${navUrl}?curpage=0${navUrlEnd}">&laquo; ��һҳ</a>
		<span class="break">...</span>
		<a title="��һҳ" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; ��һҳ</a>
		<c:if test="${currentPage>pageNum-2}">
			<a title="������${currentPage-3}ҳ" href="/${navUrl}?curpage=${currentPage-4}${navUrlEnd}">${currentPage-3}</a>
		</c:if>
		<c:if test="${currentPage>pageNum-3}">
			<a title="������${currentPage-2}ҳ" href="/${navUrl}?curpage=${currentPage-3}${navUrlEnd}">${currentPage-2}</a>
		</c:if>
		<a title="������${currentPage-1}ҳ" href="/${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a>
		<a title="������${currentPage}ҳ" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a>
		<span class="this-page">${currentPage+1}</span>
		<c:if test="${currentPage<pageNum-1}">
			<a title="������${currentPage+2}ҳ" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a>
		</c:if>
		<c:if test="${currentPage<pageNum-2}">
			<a title="������${currentPage+3}ҳ" href="/${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a>
		</c:if>
		<c:if test="${currentPage<pageNum-1}">
			<a title="��һҳ" href="/${navUrl}?curpage=${currentPage+1}${navUrlEnd}">��һҳ &gt;</a>
		</c:if>
		<c:if test="${currentPage<pageNum-3}">
			<span class="break">...</span>
			<a title="���ҳ" href="/${navUrl}?curpage=${pageNum-1}${navUrlEnd}">���ҳ &raquo;</a>
		</c:if>
	</c:if>
</c:if>