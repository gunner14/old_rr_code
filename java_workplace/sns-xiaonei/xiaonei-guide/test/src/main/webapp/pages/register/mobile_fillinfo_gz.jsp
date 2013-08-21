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
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/autocomplete/autocomplete-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/connection/connection-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/event/event-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/yui/yahoo/yahoo-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/cityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/fillinfo.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/schoolComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/provinceCityComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/profileNet.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/autoComplete.js?ver=$revxxx$"></script>

<script type="text/javascript">
	ProvinceCityComponent.init('hometownProvince', 'hometownCityCode');
</script>

<c:set var="high_type" value="0" />
<c:set var="junior_type" value="1" />
<c:set var="tech_type" value="2" />
<script type="text/javascript">
function c_s(){
	if($("boys").checked == false && false == $("girls").checked){
		alert("请填写性别");
	}
	else if("" == $("hometownProvince").value){
		alert("请选择家乡");
	}
	else if("" == $("hometownCityCode").value){
		alert("请选择城市");
	}
	<c:choose>
		<c:when test="${schoolType == high_type}">
	else if("" == $("highSchoolCode0").value || "" == $("highSchoolName0").value){
		alert("请填写高中");
	}
	else if("" == $("highSchoolEnrollYear").value){
		alert("请选择高中入学年");
	}

	</c:when>
	<c:when test="${schoolType == tech_type}">
	else if("" == $("techSchoolCode").value || "" == $("techSchoolName").value){
		alert("请填写学校");
	}
	else if("" == $("techSchoolEnrollYear").value){
		alert("请选择学校入学年");
	}
	</c:when>
	</c:choose>
	else if("" == $("juniorSchoolName").value || "" == $("juniorSchoolCode").value){
		alert("请填写初中");
	}
	else if("" == $("juniorSchoolEnrollYear").value){
		alert("请选择初中入学年");
	}
	<c:choose>
	<c:when test="${schoolType == high_type}">
	else if($("juniorSchoolEnrollYear").value >= $("highSchoolEnrollYear").value){
		alert("初中入学年需要小于高中入学年");
	}
	</c:when>
	<c:when test="${schoolType == tech_type}">
	else if($("juniorSchoolEnrollYear").value >= $("techSchoolEnrollYear").value){
		alert("初中入学年过大");
	}
	</c:when>
	</c:choose>
	else if("" == $("elementarySchoolName").value ){
		alert("请填写小学");
	}
	else if("" == $("elementarySchoolEnrollYear").value){
		alert("请选择小学入学年");
	}
	else if($("elementarySchoolEnrollYear").value >= $("juniorSchoolEnrollYear").value){
		alert("小学入学年需要小于初中入学年");
	}
	else{
		$("infoForm").submit();
	}
}
</script>
<title>${domainIdentified["siteName_fillinfo"]}</title>
</head>
<body id="refinePage" class="newguide fill_inf">
<%@ include file="/inc/allschoollist.inc" %>
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
			<html:form action="/fillinfomg.do" styleId="infoForm" method="post">
				<html:hidden property="schoolType" styleId="schoolType"/>
					<input type="hidden" name="setGender" value="${setGender}" />
					<input type="hidden" name="setHometown" value="${setHometown}" />
					<input type="hidden" name="setHS" value="${setHS}" />
					<input type="hidden" name="setTS" value="${setTS}" />
					<input type="hidden" name="setJS" value="${setJS}" />
					<input type="hidden" name="setES" value="${setES}" />
				<fieldset>
					<legend style="display: none;">教育信息</legend>
					<div id="getinf" class="formcontent">
						<p class="clearfix" <c:if test="${setGender}">style="display: block;"</c:if>> 
							<label>性别:</label>
							<label style="text-align:left;width:50px" for="boys">
								<html:radio property="gender" value="男生" styleId="boys">男</html:radio>
							</label>
							<label style="text-align:left;width:50px" for="girls">
								<html:radio property="gender" value="女生" styleId="girls">女</html:radio>
							</label>
						</p>
						<p <c:if test="${setHometown}">style="display: block;"</c:if>>
							<label>家乡:</label>
							<c:choose>
								<c:when test="${setHometown}">
									<html:hidden property="hometownProvince" styleId="hometownProvince" /> 
									<html:hidden property="hometownCity" styleId="hometownCity" /> 
									<html:hidden property="hometownCityCode" styleId="hometownCityCode" /> 
								</c:when>
								<c:otherwise>
									<html:select property="hometownProvince" styleId="hometownProvince" styleClass="select">
										<%@ include file="/pages/profile/basicInfo_sub1.jsp"%>
									</html:select>
									<html:hidden property="hometownCity" styleId="hometownCity" /> 
									<html:select property="hometownCityCode" styleId="hometownCityCode" styleClass="select" onblur="Profile.selectCity(this.selectedIndex, 'hometownCityCode', 'hometownCity')">
										<html:option value="">选择城市:</html:option>
									</html:select>
								</c:otherwise>
							</c:choose>
						</p>
						<c:choose>
							<c:when test="true">
								<p <c:if test="${setHS}">style="display: block;"</c:if>>
									<label>高中:</label>
									<html:hidden property="highSchoolCode" styleId="highSchoolCode0" />
									<html:text property="highSchoolName" tabindex="6" styleId="highSchoolName0" styleClass="inputtext" onclick="SchoolComponent.show(SchoolComponent.OPEN_HIGH_SCHOOL,'highSchoolName0','highSchoolCode0');" readonly="true" />
									<span id="highSchoolName_error_span" style="display:inline;"></span>
									<c:choose>
										<c:when test="false">
											<html:hidden property="highSchoolEnrollYear" styleId="highSchoolEnrollYear"/>
										</c:when>
										<c:otherwise>
											<html:select property="highSchoolEnrollYear" styleId="highSchoolEnrollYear" styleClass="select">
												<html:option value="">入学年份:</html:option>
												<c:forEach var="i" begin="1989" end="2012">
                                                    <option value="${4001-i}">${4001-i}</option>
                                                </c:forEach>
											</html:select>
										</c:otherwise>
									</c:choose>
								</p>
							</c:when>
							<c:when test="${schoolType == tech_type}">
								<p <c:if test="${setTS}">style="display: block;"</c:if>>
									<label>学校:</label>
									<html:hidden property="techSchoolCode" styleId="techSchoolCode" />
									<html:text property="techSchoolName" tabindex="6" styleId="techSchoolName" styleClass="inputtext" onclick="SchoolComponent.show(SchoolComponent.COLLEGE_SCHOOL,'techSchoolName','techSchoolCode');" readonly="true" />
									<span id="techSchoolName_error_span" style="display:inline;"></span>
									<c:choose>
										<c:when test="false">
											<html:hidden property="techSchoolEnrollYear" styleId="techSchoolEnrollYear"/>
										</c:when>
										<c:otherwise>
											<html:select property="techSchoolEnrollYear" styleId="techSchoolEnrollYear" styleClass="select">
												<html:option value="">入学年份:</html:option>
												<c:forEach var="i" begin="1989" end="2012">
													<option value="${4001-i}">${4001-i}</option>
												</c:forEach>
											</html:select>
										</c:otherwise>
									</c:choose>
								</p>
							</c:when>
						</c:choose>
						<p <c:if test="${setJS}">style="display: block;"</c:if>>
							<label>初中:</label>
							<html:text property="juniorSchoolName" styleId="juniorSchoolName" styleClass="inputtext" onclick="SchoolComponent.show(SchoolComponent.JUNIOR_SCHOOL,'juniorSchoolName','juniorSchoolCode');" readonly="true"/>
							<html:hidden property="juniorSchoolCode" styleId="juniorSchoolCode" />
							<c:choose>
								<c:when test="false">
									<html:hidden property="juniorSchoolEnrollYear" styleId="juniorSchoolEnrollYear"/>
								</c:when>
								<c:otherwise>
									<html:select property="juniorSchoolEnrollYear" styleId="juniorSchoolEnrollYear" styleClass="select">
										<html:option value="">入学年份:</html:option>
										<c:forEach var="i" begin="1989" end="2012">
                                            <option value="${4001-i}">${4001-i}</option>
                                        </c:forEach>
									</html:select>
								</c:otherwise>
							</c:choose>
						</p>
						<p <c:if test="${setES}">style="display: block;"</c:if>>
							<label>小学:</label>
							<html:text property="elementarySchoolName" styleId="elementarySchoolName" styleClass="inputtext" />
							<c:choose>
								<c:when test="false">
									<html:hidden property="elementarySchoolEnrollYear" styleId="elementarySchoolEnrollYear"/>
								</c:when>
								<c:otherwise>
									<html:select property="elementarySchoolEnrollYear" styleId="elementarySchoolEnrollYear" styleClass="select">
										<html:option value="">入学年份:</html:option>
										<c:forEach var="i" begin="1989" end="2012">
                                              <option value="${4001-i}">${4001-i}</option>
                                        </c:forEach>
									</html:select>
								</c:otherwise>
							</c:choose>
						</p>
						<div id="elementaryschoolContainer" class="sug_infillinfo"></div>
						<script type="text/javascript">
							XIAONEI.addListener(XIAONEI.elementaryschool, 'elementarySchoolName', 'elementaryschoolContainer');
						</script>
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
