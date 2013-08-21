<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<%@ include file="/inc/head-for-mobile-lead.inc" %>
<link href="http://s.xnimg.cn/css/register.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
<link href="http://s.xnimg.cn/css/xsuggest.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
<link href="http://s.xnimg.cn/csspro/apps/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all"/>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/yahoo-dom-event/yahoo-dom-event.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/connection/connection-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/yahoo/yahoo-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/event/event-min.js?ver=$revxxx$"></script>
<title>${domainIdentified["siteName_fillinfo"]}</title>
<script type="text/javascript">
	function send(){
		//send
		var paras = "act=sv&email="+$('email').value;
		var sending= new Ajax.Request(
				"/verifymail.do",{
					method:'post',
					parameters :paras,
					onComplete :showResponse,
					onFailure :showError
					}
				);
	};
	function verify(){
		$("act").value="verify";
		$("verifyForm").submit();
	};
	function showResponse(r){
		var ok = "0";
		var fail = "10";
		var inemail = "20";
		var notwebreg = "30";
		var userexist = "40";
		var return_result = r.responseText;
		switch(return_result){
		case ok:
			$('err_msg_email').style.display="none";
			$('emailDiv').style.display="none";
			$('veriyfyDiv').style.display="block";
			$('rewrite').style.display="block";
			break;
		case inemail:
			alert("邮件格式有问题，请重新填写");
			break;
		case notwebreg:
			alert("您不是在手机注册的用户，不能使用此功能");
			break;
		case fail:
			alert("验证邮件发送失败，请稍后重试");
			break;
		case userexist:
			alert("这个邮箱已经注册过${domainIdentified["siteName"]}网，不能使用");
			break;
		}
		
	};
	function showError(){
		alert("系统错误，请稍后重试");
	};
	function prev(){
		$('err_msg_email').style.display="none";
		$('emailDiv').style.display="block";
		$('veriyfyDiv').style.display="none";
		$('rewrite').style.display="none";
	}
</script>
</head>
<body id="refinePage" class="newguide fill_inf">
<div id="container">
	<c:set var="hideLink" value="yes" />
	<%@ include file="/inc/header-guide-flow.inc" %>
	<%@ include file="/inc/sidebar_nocompatible.inc" %>
	<div id="content">
		<div id="oak">
		<div id="err_msg_email">
		<%@ include file="/inc/errors.inc" %>
		</div>
							<div class="fillbox emailinvite">
								<div class="innerborder">
									<div class="stepbox">
										<div class="float-right"><span class="steptxt">第3步，共3步</span>  <img src="${applicationScope.urlStatic}/img/guide/step3.gif"></div>
										<h2>填写你的邮箱</h2>
                                        <p>如果你有邮箱，可以在下面填写你的邮箱，用来接收${domainIdentified["siteName"]}网的通知邮件，也可以用来登录${domainIdentified["siteName"]}网。</p>
									</div>
									<div class="form-invite">
										<form id="verifyForm" method="post" action="/verifymail.do">
											<input type="hidden" name="act" id="act" />
											<fieldset>
												<%-- step 1 --%>
                                                <div id="emailDiv" class="formcontent" style="display: block">
                                                    <p>
														<label for="email">你的邮箱:</label>
    	                                                <input type="text" class="input-text" size="25" name="email" id="email" value="${email}" />
                                                    </p>
													<p>
														<label class="empty">提交:</label>
														<input onclick="send()" value="提交" class="inputsubmit" type="button">
														&nbsp;&nbsp;&nbsp;&nbsp;<a href="skipve">跳过</a>
														
													</p>
												</div>
												<%-- step 2 --%>
												<div id="veriyfyDiv" class="formcontent" style="display: none">
													一串验证码已经发送到您的邮箱，请查收后将验证码填写进下面的输入框。
                                                    <p>
														<label for="email">验证码:</label>
    	                                                <input type="text" class="input-text" size="25" name="vc" id="vc" />
                                                    </p>
													<p>
														<label class="empty">提交:</label>
														<input onclick="verify()" value="提交" class="inputsubmit" type="button">
														&nbsp;&nbsp;&nbsp;&nbsp;<a href="skipve">跳过</a>
													</p>
												</div>
											</fieldset>
										</form>
										
									</div>
									<div class="stepbox footer">
										<a id="rewrite" style="display:none" href="javascript:prev();" linkindex="238" set="yes">重新填写邮箱</a>
									</div>
								</div>
							</div>

		</div>
	</div>
	<%@ include file="/inc/footer-guide-flow.inc" %>
</div>
<c:if test="${step2 == 1}">
<script type="text/javascript">
$('emailDiv').style.display="none";
$('veriyfyDiv').style.display="block";
$('rewrite').style.display="block";
</script>
</c:if>
</body>
</html>
