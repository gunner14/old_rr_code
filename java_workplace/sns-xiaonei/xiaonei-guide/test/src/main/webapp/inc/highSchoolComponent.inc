<%@ page contentType="text/html;charset=UTF-8" %>
<%
//---------------------------------
//-- 高中组件-/inc/highSchoolComponent.inc
//-- 组件中的其他2个文件
//-- /js/highSchoolComponent.js
//-- /inc/highSchoolComponent_Pan.inc
//---------------------------------
%>
<style type="text/css">

/* univlist */
#highSchoolListWrapDiv, #searchSchoolList { position:absolute; width:606px; z-index:1000;}
#highSchoolCityQuList { border-bottom:1px solid #3B5888;}
#highSchoolCityQuList td { vertical-align:middle; color:#808080; text-align:center; font-size:13px;}
#highSchoolCityQuList a { font-size:12px;display:block; text-decoration:none;}
#highSchoolCityQuList td.activetab a { background:#3B5888; color:#fff;}
#highSchoolCityQuList:after { content:"."; display:block; height:0; clear:both; visibility:hidden;}
#highSchoolCityQuList { *zoom:1;}
#highSchoolListContentUl { list-style:none; padding:5px 10px; border:1px solid #d8dfea; background:#fff;}
#highSchoolListContentUl:after { content:"."; display:block; height:0; line-height:0; clear:both; visibility:hidden;}
#highSchoolListContentUl { *zoom:1;}
#highSchoolListContentUl li { float:left; width:140px; border-bottom:1px solid #f4f4f4; line-height:23px;}
#highSchoolListContentUl li.univ { width:112px;}
#highSchoolListContentUl li a:hover { background:#3B5888; color:#fff; text-decoration:none;}
#highSchoolListWrapDiv .operation a { margin-right:180px;}
/*  */
#highSchoolComponent_provinceCitySpan {display:none;}
</style>		
<span id="highSchoolComponent_provinceCitySpan">
	<select id="highSchoolProvince" name="highSchoolProvince" onclick="HighSchoolComponent.getHighSchoolCityList(this.selectedIndex,'highSchoolCityCode');" styleClass="select" tabindex="13">
		<%@ include file="/pages/profile/basicInfo_sub2.jsp" %>
	</select>
	<select id="highSchoolCityCode" name="highSchoolCityCode" class="select" tabindex="14" onclick="">
		<option value="">选择城市:</option>
	</select>
</span>
<input type="text" id="highSchoolName" name="highSchoolName" class="inputtext" readonly="true" value="${highSchoolComponent_highSchoolName}" onclick="HighSchoolComponent.whenClickReadonlyCityNameText('highSchoolProvince','highSchoolCityCode')" tabindex="15"/>
<input type="hidden" id="highSchoolCode" name="highSchoolCode" value="${highSchoolComponent_highSchoolCode}"/>
				
<script type="text/javascript">
	//初始化高中城市
	HighSchoolComponent.initProvinceCitySelect('${requestScope.userBasic.cityCode}','highSchoolProvince','highSchoolCityCode');
</script>