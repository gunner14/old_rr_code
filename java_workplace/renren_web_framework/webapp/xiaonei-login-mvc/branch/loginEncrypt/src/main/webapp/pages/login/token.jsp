<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<script type="text/javascript">
document.location.href="${requestScope.callback_url}?${requestScope.auth_token}";
</script>