<%@ page pageEncoding="UTF-8" %>

<div class="searchbar">
		<form name="searchbar" action="/ImOnline2.do" method="post">
			<label for="gender_all"><input type="radio" name="gender" id="gender_all" value="" <c:if test="${requestScope.gender==''||requestScope.gender==null}">checked</c:if>/>全部</label>
			<label for="gender_female"><input type="radio" name="gender" id="gender_female" value="女生" <c:if test="${requestScope.gender=='女生'}">checked</c:if>/>女生</label>
			<label for="gender_male"><input type="radio" name="gender" id="gender_male" value="男生" <c:if test="${requestScope.gender=='男生'}">checked</c:if>/>男生</label>
			<label for="province">省份:</label><select name="province" id="province" class="select" onchange="getUniversityByProvince()" tabindex="1"><option value="">--请选择--</option></select>
			<label for="univ">学校:</label><select name="id" id="id" class="select" tabindex="3"><option value="">--请选择--</option></select>
			<input type="submit" class="subbutton" value="搜索" />
		</form>
	</div>
<script type="text/javascript">
var frm = document.searchbar;
function getUniversityByProvince()
{
	if(frm.province.options.selectedIndex == 0) return;
	var index = frm.province.options.selectedIndex - 1;
	for(var i=frm.id.options.length; i>=0; i--) {
		frm.id.remove(i);
	}
	frm.id.options[frm.id.options.length] = new Option("--请选择--", "");
	for(var i=0; i<univListIM[index].univ.length; i++) {
		frm.id.options[frm.id.options.length] = new Option(univListIM[index].univ[i].name, univListIM[index].univ[i].id);
		<c:if test="${!empty requestScope.id}">
			if (${requestScope.id}==univListIM[index].univ[i].id)
				frm.id.options[frm.id.options.length-1].selected="true";
		</c:if>
	}
}
for(var i=0; i<univListIM.length; i++) {
	frm.province.options[frm.province.options.length] = new Option(univListIM[i].name, univListIM[i].id);
	<c:if test="${!empty requestScope.province}">
		if (${requestScope.province}==univListIM[i].id)
			frm.province.options[frm.province.options.length-1].selected="true";
	</c:if>
}
getUniversityByProvince();



</script>