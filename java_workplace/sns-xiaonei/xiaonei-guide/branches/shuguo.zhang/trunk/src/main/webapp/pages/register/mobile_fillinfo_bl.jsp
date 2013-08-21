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
<script type="text/javascript">
function c_s(){
	if($("boys").checked == false && false == $("girls").checked){
		alert("请填写性别");
	}
	else if("" == $("locationCity").value || "" == $("locationCode").value || "" == $("locationProvince").value){
		alert("请填写所在地");
	}
	else{
		<c:if test="${stage==30}">
		if("选添" == $('company').value){
			$('company').value = "";
		}
		</c:if>
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
				<span class="steptxt">第2步，共3步</span>
				<img src="${applicationScope.urlStatic}/img/guide/step2.gif">
			</div>
			<h2>欢迎通过电脑登录${domainIdentified["siteName"]}网</h2>
			<p>完成以下步骤，开始${domainIdentified["siteName"]}网生活。</p>
		</div>
		<div class="form-invite">
			<html:form action="/fillinfomb.do" styleId="infoForm" method="post">
				<input type="hidden" name="setGender" value="${setGender}" />
				<input type="hidden" name="setRegion" value="${setRegion}" />
				<input type="hidden" name="setCompany" value="${setCompany}" />
				
				<input type="hidden" name="act" value="submit" />
				<html:hidden property="stage" value="${stage}"/>
				<fieldset>
					<div id="getinf" class="formcontent">
						<p class="clearfix" <c:if test="${setGender}">style="display: block"</c:if>>
							<label>性别:</label>
							<label style="text-align:left;width:50px" for="boys">
								<html:radio property="gender" value="男生" styleId="boys">男</html:radio>
							</label>
							<label style="text-align:left;width:50px" for="girls">
								<html:radio property="gender" value="女生" styleId="girls">女</html:radio>
							</label>
						</p>
						<p <c:if test="${setRegion}">style="display: block"</c:if>>
							<label>居住地:</label>
							<c:choose>
								<c:when test="${setRegion}">
									<html:hidden property="locationProvince" styleId="locationProvince"/>
									<html:hidden property="locationCode" styleId="locationCode"/>
									<html:hidden property="locationCity" styleId="locationCity"/>
								</c:when>
								<c:otherwise>
									<html:select property="locationProvince" styleId="locationProvince" styleClass="select" tabindex="5" onchange="Profile.getInCityListOfProvince(this.selectedIndex, 'locationCode')" >
										<%@ include file="/pages/profile/inProvinceHtml.jsp"%>
									</html:select>
									<html:hidden property="locationCity" styleId="locationCity" />
									<html:select property="locationCode" styleId="locationCode" styleClass="select" tabindex="6" onblur="Profile.selectCity(this.selectedIndex, 'locationCode', 'locationCity');" >
										<html:option value="">选择城市: </html:option>
									</html:select>
								</c:otherwise>
							</c:choose>
						</p>
						<c:if test="${stage==30}">
							<p <c:if test="${setCompany}">style="display: block"</c:if>>
								<label>公司:</label>
								<html:text property="company" styleId="company" styleClass="input-text" onfocus="javascript:if(this.value=='选填')this.value=''"/>
								<!-- <input type="hidden" id="stage" name="stage" value="30" /> -->
								<span id="showspancorp" class="exptext"></span>
							</p>
							<div id="company_sug" class="posrela"></div>
							<script type="text/javascript">
								$("company_sug").style.width = $("company").style.width;
								XIAONEI.addListener(XIAONEI.workplace, 'company', 'company_sug');
							</script>
						</c:if>
						<p>
							<label class="empty">提交:</label>
							<input onclick="c_s();" value="提交" class="inputsubmit" type="button">
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
