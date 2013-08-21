<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<%@ include file="/inc/head-for-mobile-lead.inc" %>
<link href="http://s.xnimg.cn/css/register.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="http://s.xnimg.cn/css/xsuggest.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link media="all" type="text/css" rel="stylesheet" href="http://s.xnimg.cn/csspro/apps/guide.css?ver=$revxxx$"/>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/yahoo-dom-event/yahoo-dom-event.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/connection/connection-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/yahoo/yahoo-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/event/event-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/fillinfo.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/inCityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/autoComplete.js?ver=$revxxx$"></script>

<script type="text/javascript" src="http://s.xnimg.cn/js/yui/autocomplete/autocomplete-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/register/registerutil_main_page.js?ver=$revxxx$"></script>
<script type="text/javascript">
function chkStage(){
 	var stage = $("stage").value;
	if (stage == "") {
		alert("请选择你的身份");
		return false;
	}
	return true;
 }

function checkName() {
	var name = Trim($("name").value);
	if (name == "") {
		alert("请输入真实中文姓名，方便朋友查找。");
		return false;
	}
	if (strlen(name) > 12) {
		alert("姓名不能多于6个汉字或者12个字符");
		return false;
	}
	if (chinese(name) < 2) {
		alert("请输入真实中文姓名");
		return false;
	}

	return true;
}
function chkCode(){
	var code = Trim($("icode").value);
	if(code == ""){
		alert("请输入验证码。");
		return false;
	}
	

	return true;
}
function c_s(){
	if( chkStage() && checkName() && chkCode() ){
		$("infoForm").submit();
	}
}
</script>
<title>${domainIdentified["siteName_fillinfo"]}</title>
</head>
<body id="refinePage" class="newguide fill_inf">
<div id="container">
<c:set var="hideLink" value="yes" />
<%@ include file="/inc/header-guide-flow.inc" %>
<%@ include file="/inc/sidebar_nocompatible.inc" %>
<div id="content">
<div id="oak">
<%@ include file="/inc/errors.inc" %>
<div class="fillbox emailinvite">
	<div class="innerborder">
		<div class="stepbox">
			<div class="float-right">
				<span class="steptxt">第1步，共3步</span>
				<img src="${applicationScope.urlStatic}/img/guide/step1.gif">
			</div>
			<h2>欢迎通过电脑登录${domainIdentified["siteName"]}网</h2>
			<p>完成以下步骤，开始${domainIdentified["siteName"]}网生活。</p>
		</div>
		<div class="form-invite">
			<html:form action="/fillinfost.do" styleId="infoForm" method="post">
				<input type="hidden" name="act" value="submit" />
					<input type="hidden" name="setName" value="${setName}" />
					<input type="hidden" name="setStage" value="${setStage}" />
				<fieldset>
					<div id="getinf" class="formcontent">
						<p id="p_stage" class="clearfix" <c:if test="${setStage}">style="display: block"</c:if>>
							<label for="stage">身份:</label>
							<c:choose>
								<c:when test="${setStage}">
									<html:hidden property="stage" styleId="stage"/>
								</c:when>
								<c:otherwise>
									<html:select property="stage" styleId="stage" styleClass="select" size="4" style="height:auto" tabindex="4">
										<html:option value="30">已工作</html:option>
										<html:option value="20">大学生</html:option>
										<html:option value="10">中学生/中专技校</html:option>
										<html:option value="90">以上都不是</html:option>
									</html:select>
								</c:otherwise>
							</c:choose>
						</p>
						
						<p id="p_name" <c:if test="${setName}">style="display: none"</c:if>>
							<label for="name">姓名:</label>
							<html:text property="name" styleId="name" styleClass="inputtext"></html:text>
						</p>
						
						<p id="p_icode">
							<label for="code"><em>*</em>验证码:</label>
							<input type="text" name="icode" id="icode" class="inputtext validate-code" size="5" maxlength="5" tabindex="11" />									
							<%
							java.util.Random random = new java.util.Random();
							int id = random.nextInt(1431655764);
							%>
							<input type="hidden" name="key_id" value=<%=id%>> 
							<img class="validate-num" id="verifyPic" src="http://icode.${applicationScope.sysdomain}/getcode.do?t=mfillinfoSt&rnd=<%=System.currentTimeMillis()%>"/>
						</p>					
						<p>
							<label class="empty">提交:</label>
							<input onclick="c_s();" value="提交" class="inputsubmit" type="button" />
						</p>
					</div>
				</fieldset>
			</html:form>
		</div>
		<div class="stepbox footer"></div>
	</div>
</div>
</div>
</div>
<%@ include file="/inc/footer-guide-flow.inc" %>
</div>
</body>
</html>
