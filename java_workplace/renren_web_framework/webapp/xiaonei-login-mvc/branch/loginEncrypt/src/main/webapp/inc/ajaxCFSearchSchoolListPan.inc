<%@ page pageEncoding="UTF-8" %>
<div class="popupwrap" id="searchSchoolList" style="display:none">
	<div class="popup">
		<div id="schoolTabPan"></div>
		<ul id="searchlist_school"><center>请选择你的所在区</center></ul>
		<p class="operation"><a href="#" onclick="notMyHighSchool()"> 没有我的高中 :( </a><input type="button" value="关闭窗口" class="canbutton" onclick="todoSearch='hide'; return hideSchoolPan()" /></p>
	</div>
</div><input type="hidden" id="defaultViewContent" value="请选择你的所在区"/>
<SCRIPT LANGUAGE="JavaScript">
<!--
var HighSchoolComponent = new Object();
function HighSchoolComponent.tihuan(hiddenUl){
	$('searchlist_school').innerHTML = $(hiddenUl).innerHTML;
}
//-->
</SCRIPT>