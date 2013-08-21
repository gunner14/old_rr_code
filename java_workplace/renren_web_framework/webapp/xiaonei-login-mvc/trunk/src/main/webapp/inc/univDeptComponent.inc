<%@ page contentType="text/html;charset=UTF-8" %>
<%
//---------------------------------------------------------------------------------------------------
//-- 大学院系组件-/inc/univDeptComponent.inc
//-- 支持同一个页面显示多个大学院系组件，
//-- 外部jsp调用此inc可以传入2个参数:
//-- univDeptComponent_index 可选的，但不要重复，指定是第几个组件，如 1、 2、 3、 4
//-- univDeptComponent_universityCode 大学id,必须的，如:value="1002";
//-- 组件中的其他文件
//-- /js/universityDepartmentComponent.js
//-- <script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/universityDepartmentComponent.js?ver=$revxxx$"></script>
//---------------------------------------------------------------------------------------------------
%>
<span id="univDeptComponent_Span${univDeptComponent_index}">
	<select value="${department}" tabindex="5" class="select" name="department${univDeptComponent_index}" id="department${univDeptComponent_index}" onchange="changeDept()">
        <option value="">院系</option>
    </select>
</span>
<script type="text/javascript">
	UnivDeptComponent.fillUniversityDept('${univDeptComponent_universityCode}','${univDeptComponent_index}');
</script>