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
<script type="text/javascript" src="http://s.xnimg.cn/js/schoolComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/cityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/universityDepartmentComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/allunivlist.js?ver=$revxxx$"></script>
<script type="text/javascript">
var bodyclick = true;

function showpan() {
	var x = findPosX($('content'));
	var y = findPosY($('universityname'));
	$("univlist").style.left = x + "px";
	$("univlist").style.top = y + 25 + "px";
	$("univlist").style.display = "block";
	bodyclick = true;
}
function hidepan() {
	$("univlist").style.display = "none";
}
function cl(id) {
	$("universityid").value = id;
	$("universityname").value = $("sch_" + id).innerHTML;
	hidepan();
	changeUniv();
}
function university_onclick() {
	if (bodyclick)
		hidepan();
}

function changeUniv(){
	UnivDeptComponent.fillUniversityDept($('universityid').value,"");
}
function c_s(){
	if($("boys").checked == false && false == $("girls").checked){
		alert("请填写性别");
	}
	else if("" == $("typeOfCourse").value){
		alert("请选择类型");
	}
	else if("" == $("universityname").value || "" == $("universityid").value){
		alert("请填写大学");
	}
	else if("" == $("universityyear").value){
		alert("请选择大学入学年");
	}
	else if("" == $("department").value){
		alert("请选择大学院系");
	}
	else if("" == $("highSchoolCode0").value || "" == $("highSchoolName0").value){
		alert("请填写高中");
	}
	else if("" == $("highSchoolEnrollYear").value){
		alert("请选择高中入学年");
	}
	else if($("highSchoolEnrollYear").value >= $("universityyear").value){
		alert("高中入学年需要小于大学入学年");
	}
	else{
		$("fillForm").submit();
	}
}
</script>
<title>${domainIdentified["siteName_fillinfo"]}</title>
</head>
<body id="refinePage" class="newguide fill_inf" onclick="university_onclick()">
<%@ include file="/inc/allunivlist.inc" %>
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
					<img src="${applicationScope.urlStatic}/img/guide/step2.gif" alt="" />
				</div>
				<h2>欢迎通过电脑登录${domainIdentified["siteName"]}网</h2>
				<p>完成以下步骤，开始${domainIdentified["siteName"]}网生活。</p>
			</div>
			<div class="form-invite">
				<html:form action="/fillinfomd.do" styleId="fillForm" method="post">
					<input type="hidden" name="setGender" value="${setGender}" />
					<input type="hidden" name="setUniv" value="${setUniv}" />
					<input type="hidden" name="setHS" value="${setHS}" />
					
					<fieldset>
						<legend style="display: none;">教育信息</legend>
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
							<p id="typeOfCoursespan" <c:if test="${setUniv}">style="display: block"</c:if>>
								<label for="">类型:</label>
								<!-- change select 2 html:select -->
								<c:choose>
									<c:when test="false">
										<html:hidden property="typeOfCourse" styleId="typeOfCourse"/>
									</c:when>
									<c:otherwise>
										<html:select property="typeOfCourse" styleId="typeOfCourse" styleClass="select" tabindex="5">
											<html:option value="">请选择类型</html:option>
											<html:option value="大学生">大学生</html:option>
											<html:option value="硕士">硕士</html:option>
											<html:option value="博士">博士</html:option>
											<html:option value="校工">校工</html:option>
											<html:option value="教师">教师</html:option>
										</html:select>
									</c:otherwise>
								</c:choose>
							</p>
							<p <c:if test="${setUniv}">style="display: block"</c:if>>
								<label>大学:</label>
								<html:hidden property="univId" styleId="universityid" />
								<html:text property="univName" styleId="universityname" styleClass="inputtext" onclick="showpan();bodyclick=false;univtabs.changeCountry('0');" readonly="true" tabindex="6" />
								<span id="universityname_error_span" style="display: inline;">
								</span>
								<c:choose>
									<c:when test="false">
										<html:hidden property="univEnrollyear" styleId="universityyear"/>
									</c:when>
									<c:otherwise>
										<html:select property="univEnrollyear" styleId="universityyear" styleClass="select">
											<html:option value="">入学年份:</html:option>
											<c:forEach var="i" begin="1991" end="2011">
												<option value="${4001-i}" <c:if test="${!empty mDxFiFom.univEnrollyear && ((4001-i) == mDxFiFom.univEnrollyear)}"> selected="selected"</c:if>>${4001-i}</option>
											</c:forEach>
										</html:select>
									</c:otherwise>
								</c:choose>
							</p>
							<p <c:if test="${setUniv}">style="display: block"</c:if>>
								<label class="empty">院系:</label>
								<span id="univDeptComponent_Span">
								<c:choose>
									<c:when test="false">
										<html:hidden property="department" styleId="department"/>
									</c:when>
									<c:otherwise>
										<html:select property="department" styleId="department" styleClass="select" onchange="changeDept()">
											<html:option value="">院系</html:option>
										</html:select>
									</c:otherwise>
								</c:choose>
								</span>
							</p>
							<p <c:if test="${setHS}">style="display: block"</c:if>>
								<label>高中:</label>
								<html:hidden property="highSchoolCode" styleId="highSchoolCode0" />
								<html:text property="highSchoolName" tabindex="6" styleId="highSchoolName0" styleClass="inputtext" onclick="SchoolComponent.show(SchoolComponent.OPEN_HIGH_SCHOOL,'highSchoolName0','highSchoolCode0');" readonly="true" />
								<span id="highSchoolName0_error_span" style="display:inline;"></span>
								<c:choose>
									<c:when test="false">
										<html:hidden property="highSchoolEnrollYear" styleId="highSchoolEnrollYear"/>
									</c:when>
									<c:otherwise>
										<html:select property="highSchoolEnrollYear" styleId="highSchoolEnrollYear" styleClass="select">
											<html:option value="">入学年份:</html:option>
											<c:forEach var="i" begin="1991" end="2011">
                                                <option value="${4001-i}">${4001-i}</option>
                                            </c:forEach>
										</html:select>
									</c:otherwise>
								</c:choose>
							</p>
							<p>
								<label class="empty">提交:</label>
								<input onclick="c_s();" value="提交" class="inputsubmit" type="button"/>
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
<script type="text/javascript">
<!--
	if($("universityid")){
		if($("universityid").value != "" && parseInt($("universityid").value) > 0 ){
			//cl(${mDxFiFom.univId});
			changeUniv();
		}
	}
//-->
</script>
</body>
</html>
