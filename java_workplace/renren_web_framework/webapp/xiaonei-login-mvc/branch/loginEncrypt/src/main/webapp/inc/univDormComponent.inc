<%@ page contentType="text/html;charset=UTF-8" %>
<%
//---------------------------------
//-- 大学宿舍组件-/inc/univDormComponent.inc
//-- 外部jsp调用此inc可以传入2个参数:
//-- univDormComponent_universityCode 大学id,必须的，用户填充宿舍select控件
//-- 组件中的其他文件
//-- /js/universityDormComponent.js
//-- <script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/universityDormComponent.js?ver=$revxxx$"></script>
//---------------------------------
%>
<span id="univDormComponent_Span${univDormComponent_index}">
	<select tabindex="5" class="select" name="dorm${univDormComponent_index}" id="dorm${univDormComponent_index}" onchange="changeDorm()">
       	<option value="">宿舍</option>
   	</select>
</span>
<script type="text/javascript">
	UnivDormComponent.fillUniversityDorm('${univDormComponent_universityCode}','${univDormComponent_index}');
</script>  	