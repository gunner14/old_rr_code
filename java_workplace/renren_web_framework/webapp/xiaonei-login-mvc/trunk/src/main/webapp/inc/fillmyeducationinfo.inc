<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/universityComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/universityDepartmentComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/newHighSchoolComponent.js?ver=$revxxx$"></script>
<script type="text/javascript">
	HighSchoolComponent.init('hs_name_','hs_id_');
</script>
<div class="note_fillinfo">
		<%@ include file="/inc/errors2.inc" %>
		<h3><span><a href="javascript:void(0)" onclick="hideInfo()">关闭</a></span>你的学校资料不完整</h3>
		<p class="alttext">我们会根据资料为你找到你可能认识的人，<strong>完善你的学校资料</strong>，我们将帮助你找到更多认识的朋友。</p>
		<form action="/FillMyEducationInfo.do" method="post">
			<c:if test="${!empty univInfo}">
			<p>
				<label>大学:</label>
				<input type="text" class="inputtext w150px" name="univname" id="univname" value="${univInfo.universityName}"/>
				<input type="hidden" name="univid" value="${univInfo.universityId}"/>
				<html:text property="univ_name_0" styleId="univ_name_0" size="20" value="${univInfo.universityName}" styleClass="inputtext"  readonly="true" onclick="showUniversityDiv('univ_name_0','univ_id_0',0)" tabindex="1"/>
				<html:hidden property="univ_id_0" styleId="univ_id_0" value="${univInfo.universityId}"/>
				<html:select property="univ_year_0" styleClass="select" styleId="univ_year_0" value="${univInfo.enrollYear}" tabindex="3">
				<html:option value="0">选择年份</html:option>
				<c:set var="strutsOptionYearComponent_begin" value="2009" />
				<c:set var="strutsOptionYearComponent_end" value="1970" />
				<%@ include file="/inc/strutsOptionYearComponent.inc" %>
				</html:select>
			</p>	
			<p>
				<label>院系:</label>
				<c:set var="univDeptComponent_universityCode" value="${univInfo.universityId}" />
				<c:set var="univDeptComponent_index" value="0" />
				<%@ include file="/inc/univDeptComponent.inc" %>
			</p>
			<html:hidden type="hidden" name="univinfoid_0" value="${univInfo.id}"/>
			<script type="text/javascript">
			try{
				window.setTimeout("UnivDeptComponent.initUniversityDept('${univInfo.department}','department0')",1000);
			}catch(e){alert(e);}
			</script>
			</c:if>
			
			<c:if test="${!empty highschoolInfo}">
			<p>
				<label>高中:</label>
				<html:text property="hs_name_0" styleId="hs_name_0" size="20" value="${highschoolInfo.highSchoolName}" styleClass="inputtext" readonly="true" onclick="HighSchoolComponent.show(0);" tabindex="11"/>
				<html:hidden property="hs_id_0" styleId="hs_id_0" value="${highschoolInfo.highSchoolId}"/>
				&nbsp;
				<html:select property="hs_year_0" styleClass="select" styleId="hs_year_0" tabindex="12" value="${highschoolInfo.enrollYear}">
				<html:option value="0">选择年份</html:option>
				<c:set var="strutsOptionYearComponent_begin" value="2009" />
				<c:set var="strutsOptionYearComponent_end" value="1970" />
				<%@ include file="/inc/strutsOptionYearComponent.inc" %>
			</html:select>
			</p>
			<html:hidden type="hidden" name="hsinfoid_0" value="${highschoolInfo.id}"/>
			</c:if>
			<p class="btngroup">
				<input type="submit" class="inputbutton" value="保存修改" />
			</p>
		</form>
</div>