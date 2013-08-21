<%@ page pageEncoding="UTF-8" %>
<%
//---------------------------------
//-- 高中组件-/inc/highSchoolComponent_Pan.inc
//-- 组件中的其他2个文件
//-- /js/highSchoolComponent.js
//-- /inc/highSchoolComponent.inc
//---------------------------------
%>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/highSchoolComponent.js?ver=$revxxx$"></script>
<div class="popupwrap" id="highSchoolListWrapDiv" style="display:none">
	<div class="popup">
		<div id="highSchoolTabPan"></div>
		<ul id="highSchoolListContentUl"><center>请选择你的所在区</center></ul>
		<p class="operation">
			<c:if test="${!empty highSchoolComponent_notFoundHighSchoolUrl}">
				<a href="#" onclick="HighSchoolComponent.notMyHighSchool('${highSchoolComponent_notFoundHighSchoolUrl}')"> 没有我的高中 :( </a>
			</c:if>
			<input type="button" value="关闭窗口" class="canbutton" onclick="HighSchoolComponent.highSchoolTodo='hide'; return HighSchoolComponent.hideSchoolPan()" />
		</p>
	</div>
<!--[if lte IE 6.5]><iframe></iframe><![endif]-->
</div><input type="hidden" id="defaultViewContent" value="请选择你的所在区"/>