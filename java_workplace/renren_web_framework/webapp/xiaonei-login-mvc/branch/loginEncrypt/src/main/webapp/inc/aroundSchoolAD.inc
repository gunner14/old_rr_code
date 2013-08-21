<%@ page pageEncoding="UTF-8" %>

<c:choose >
	<c:when test="${(ServletPath == '/getuser.do') || (ServletPath == '/GetTribe.do') || (ServletPath == '/GetThreadList.do')}">
	</c:when>
	<c:when test="${(ServletPath == '/SocialNet.do') || (ServletPath == '/university.do')}">
	  <c:choose>
		  <c:when test="${(AroundADSchoolID != null) && (AroundADSchoolID != 'null')}">
			  <script src="${applicationScope.urlClub}/data/xiaonei.js"></script> 	  
		  </c:when>
			<c:otherwise>
			  <c:if test="${visiter.univ != null}">	     
				  <script src="${applicationScope.urlClub}/data/xiaonei.js"></script> 
				</c:if>
			</c:otherwise>		  
	  </c:choose>
	</c:when>
	<c:otherwise>
	  <c:if test="${visiter.univ != null}">	     
		  <script src="${applicationScope.urlClub}/data/xiaonei.js"></script> 
		</c:if>
	</c:otherwise>
</c:choose>
