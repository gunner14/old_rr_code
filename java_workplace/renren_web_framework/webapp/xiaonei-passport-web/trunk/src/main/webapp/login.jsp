<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<c:choose>
	<c:when test="${not empty cookie.passport.value && cookie.passport.value != '' && not empty cookie.ap.value && cookie.ap.value != ''}">
		<script type="text/javascript">
		document.domain = "renren.com";
		function setHeight(){
			top.document.getElementById('iframelogin').style.display = 'none';
		}
		window.onload = setHeight; 
		</script>
	</c:when>
	<c:otherwise>
		<script type="text/javascript">
		document.domain = "renren.com";
		function setHeight(){
			hashHeight = self.location.href.split('&')[0].split('=')[1];
			top.document.getElementById('iframelogin').style.height = hashHeight +"px";
		}
		window.onload = setHeight; 
		</script>
	</c:otherwise>
</c:choose>
</head>
<body>
</body>
</html>
