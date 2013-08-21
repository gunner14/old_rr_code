<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei" %>
<input type="hidden" value='<xiaonei:sysMsg/>'/>
<html:errors />
<html:messages id="msg" message="true" header="messages.header" footer="messages.footer"><p>${msg}</p></html:messages>