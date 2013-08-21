<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<%@ include file="/inc/kaixin/head.inc" %>
<title>开心网 - 重发激活链接</title>
<script>
	function toRegister()
	{
		window.location = "http://reg.kaixin.com/register.do?ss=10101";
	}
</script>
<style type="text/css">
.actbox{width:500px;border:1px solid #eee;background:#E0F0FE;margin:20px auto 0;padding:20px;text-align:center;
		-webkit-border-radius: 5px;
		-moz-border-radius: 5px;
	}
h3,
form fieldset p{margin:0 0 10px 0;}
.inputtext {width:180px;}
</style>
</head>
<body id="registerpage">
<div id="container">
	<%@ include file="/inc/kaixin/header-not-login.inc" %>
	<div id="content">
		<div class="actbox">
			<%@ include file="/inc/errors.inc" %>
			<form method="post" action="/reactivate.do" class="form-login">
				  <h3>重发激活链接</h3>
				 <fieldset>
				<p>
					<label for="email">帐号:</label>
					<input type="text" name="email" id="email" class="inputtext" />
				</p>
				<p>
					<label for="password">密码:</label>
					<input type="password" name="password" id="password" class="inputtext" />
				</p>
				<p id="reActivateBtns">
					<label class="empty"></label>
					<input type="submit" name="submit" id="submit" class="inputsubmit" value="重发激活链接" />
					<input type="button" name="register" id="register" value="重新注册帐号" onclick="toRegister()" class="inputbutton" />
				</p>
				<p id="help">如果你没有收到email，请用你的帐号email给 <a href="mailto:hi@kaixin.com">hi@kaixin.com</a> 发信，我们立刻帮你激活帐号。</p>
				</fieldset>
			</form>
		</div>
	</div>
	<%@ include file="/inc/kaixin/footer-noad.inc" %>
</div>
</body>
</html>
