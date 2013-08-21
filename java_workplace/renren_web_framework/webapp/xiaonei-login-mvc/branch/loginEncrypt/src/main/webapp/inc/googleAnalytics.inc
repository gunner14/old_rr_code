<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<script type="text/javascript" src="http://www.google-analytics.com/urchin.js"></script>
<script type="text/javascript">
try{
_uacct = "UA-606891-1";
_usample = 0.5;
urchinTracker();
}catch(e){
}

<c:set var="theServerName" value="${pageContext.request.getServerName()}" />
<c:set var="searchString" value="class.xiaonei.com" />
<c:if test="${(!empty theServerName) && (fn:contains(theServerName, searchString))}">
try
{
  _uff = 0;
 	_uacct = "UA-877339-4"; 
 	urchinTracker();
}catch(e){}		
</c:if>

try {
	if(GetCookie("newbie") == "1") {
		_uff = 0;
		_uacct = "UA-877339-2";
		urchinTracker();
	}
}catch(e){
}

<c:if test="${!empty host && visiter.stage == 10}">
try {
		_uff = 0;
		_uacct = "UA-877339-5";	        
		urchinTracker();  
}catch(e){
}
</c:if>

<c:if test="${(!empty host && visiter.stage == 30) || (!empty majorReg && majorReg == 107)}">
try {	
		_uff = 0;
		_uacct = "UA-877339-6";	        
		urchinTracker();  
}catch(e){
}
</c:if>
<c:set var="groupString" value="group.xiaonei.com" />
<c:if test="${(!empty theServerName) && (fn:contains(theServerName, groupString))}">
try
{
	_uff = 0;
	_uacct = "UA-371132-18";
	urchinTracker();
}catch(e){}		
</c:if>
</script>
