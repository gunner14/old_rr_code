<%@ page pageEncoding="UTF-8" %>
<div id="feedsFangle" class="tipwrap">
	<div class="tip">
	<div class="note">
		<h4>重要更新！！</h4>
		<p>我们将于3月中旬推出班级功能，请重新填写以下信息</p>
	</div>
	<form action="/CFSaveSchool.do" method="post" onsubmit="return saveSchool('highSchool','highSchoolCode');">
		<p class="description">${domainIdentified["siteName"]}希望让更多的同学选择统一、精确的个人信息，以方便你日后能在这里找到更多的老同学，谢谢合作。</p>
		<c:if test="${empty requestScope.userBasic.cityCode}">
		<p>
			<label for="homeProvince">家乡:</label>
			<html:select styleId="homeProvince" property="homeProvince" onclick="getCityByChangeProvinceid(this.selectedIndex,'codeCity')" styleClass="select" value="${requestScope.userBasic.homeProvince}">
				<%@ include file="/pages/profile/basicInfo_sub1.jsp" %>
			</html:select>
			<html:select styleId="codeCity" property="codeCity" styleClass="select" value="${requestScope.userBasic.cityCode}" onblur="changeCodeCity('codeCity',this.selectedIndex)">
				<html:option value="">选择城市:</html:option>
			</html:select>	
			<html:hidden styleId="hometown" property="hometown" value="${requestScope.userBasic.homeCity}" />
		</p>
		</c:if>
		<c:if test="${empty requestScope.userBasic.highSchoolCode}">
		<p>
			<label for="homeProvince2">高中所在:</label>
			<select id="homeProvince2" name="homeProvince2" onclick="getCityByChangeProvinceid2(this.selectedIndex,'codeCity2');" class="select" value="">
				<%@ include file="/pages/profile/basicInfo_sub2.jsp" %>
			</select>
			<select id="codeCity2" name="codeCity2" class="select" tabindex="14" value="${requestScope.userBasic.cityCode}" onclick="">
				<option value="">选择城市:</option>
			</select>
		</p>
		<p>
			<span id="systemHighSchoolSpan"><html:text styleId="highSchool" property="highSchool" styleClass="inputtext" readonly="true" size="30" value="点击这里选择高中" onclick="checkBeforeRequest('homeProvince2','codeCity2');"/></span>
			<html:hidden styleId="highSchoolCode" property="highSchoolCode" value="${requestScope.userBasic.highSchoolCode}"/>
			<span id="customHighSchoolSpan"><input type="text" id="customHighSchool" name="customHighSchool" class="inputtext" size="25" onclick="clearDefaultCustomSchoolInfo();"/></span>&nbsp;&nbsp;
		</p>
		</c:if>
		<p class="actions">
			<input type="submit" id="submit" name="submit" class="subbutton" value="保存修改"  tabindex="13" />
		</p>
	</form>
	</div>
</div>
<script type="text/javascript">
	initHighSchoolSearchBar('${requestScope.userBasic.cityCode}');
</script>