<%@ page pageEncoding="utf-8" %>
<script language="javascript">   
//<![CDATA[
	var ii = 0, jj = 0, kk = 0;		
	var mayknown=[];	
	<c:if test="${requestScope.hotStarJson!=null && requestScope.hotStarJson!=''}">
		var hotStarJson=${requestScope.hotStarJson};
	</c:if>
	<c:if test="${requestScope.sameIP!=null && requestScope.sameIP!=''}">
		var sameIP=${requestScope.sameIP};
	</c:if>
	<c:if test="${requestScope.friendTwoWay!=null && requestScope.friendTwoWay!=''}">
		var friendTwoWay=${requestScope.friendTwoWay};
	</c:if>
	<c:if test="${requestScope.emailIsConfirmed!=null && requestScope.emailIsConfirmed!=''}">
		var emailIsConfirmed=${requestScope.emailIsConfirmed};
	</c:if>		
	<c:if test="${requestScope.mostPopularApps!=null && requestScope.mostPopularApps!=''}">
		var mostPopularApps=${requestScope.mostPopularApps};
	</c:if>	
	<c:if test="${requestScope.friendListWithinTime!=null && requestScope.friendListWithinTime!=''}">
		var friendListWithinTime=${requestScope.friendListWithinTime};
	</c:if>	
	<c:if test="${requestScope.twoWayFriendsIds!=null && requestScope.twoWayFriendsIds!=''}">
		var twoWayFriendsIds=${requestScope.twoWayFriendsIds};
	</c:if>	
	<c:if test="${requestScope.sameAreaJson!=null && requestScope.sameAreaJson!=''}">
		var sameAreaJson=${requestScope.sameAreaJson};
	</c:if>
	<c:if test="${requestScope.sameSchoolJson!=null && requestScope.sameSchoolJson!=''}">
		var sameSchoolJson=${requestScope.sameSchoolJson};
	</c:if>
// ]]>	
</script>