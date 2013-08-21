<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<option value="">省份</option>
<option value="北京" <c:if test="${requestScope.userBasic.homeProvince=='北京'}">selected="selected"</c:if>>北京市</option>
<option value="上海" <c:if test="${requestScope.userBasic.homeProvince=='上海'}">selected="selected"</c:if>>上海市</option>
<option value="天津" <c:if test="${requestScope.userBasic.homeProvince=='天津'}">selected="selected"</c:if>>天津市</option>
<option value="重庆" <c:if test="${requestScope.userBasic.homeProvince=='重庆'}">selected="selected"</c:if>>重庆市</option>
<option value="黑龙江" <c:if test="${requestScope.userBasic.homeProvince=='黑龙江'}">selected="selected"</c:if>>黑龙江省</option>
<option value="吉林" <c:if test="${requestScope.userBasic.homeProvince=='吉林'}">selected="selected"</c:if>>吉林省</option>
<option value="辽宁" <c:if test="${requestScope.userBasic.homeProvince=='辽宁'}">selected="selected"</c:if>>辽宁省</option>
<option value="山东" <c:if test="${requestScope.userBasic.homeProvince=='山东'}">selected="selected"</c:if>>山东省</option>
<option value="山西" <c:if test="${requestScope.userBasic.homeProvince=='山西'}">selected="selected"</c:if>>山西省</option>
<option value="陕西" <c:if test="${requestScope.userBasic.homeProvince=='陕西'}">selected="selected"</c:if>>陕西省</option>
<option value="河北" <c:if test="${requestScope.userBasic.homeProvince=='河北'}">selected="selected"</c:if>>河北省</option>
<option value="河南" <c:if test="${requestScope.userBasic.homeProvince=='河南'}">selected="selected"</c:if>>河南省</option>
<option value="湖北" <c:if test="${requestScope.userBasic.homeProvince=='湖北'}">selected="selected"</c:if>>湖北省</option>
<option value="湖南" <c:if test="${requestScope.userBasic.homeProvince=='湖南'}">selected="selected"</c:if>>湖南省</option>
<option value="海南" <c:if test="${requestScope.userBasic.homeProvince=='海南'}">selected="selected"</c:if>>海南省</option>
<option value="江苏" <c:if test="${requestScope.userBasic.homeProvince=='江苏'}">selected="selected"</c:if>>江苏省</option>
<option value="江西" <c:if test="${requestScope.userBasic.homeProvince=='江西'}">selected="selected"</c:if>>江西省</option>
<option value="广东" <c:if test="${requestScope.userBasic.homeProvince=='广东'}">selected="selected"</c:if>>广东省</option>
<option value="广西" <c:if test="${requestScope.userBasic.homeProvince=='广西'}">selected="selected"</c:if>>广西</option>
<option value="云南" <c:if test="${requestScope.userBasic.homeProvince=='云南'}">selected="selected"</c:if>>云南省</option>
<option value="贵州" <c:if test="${requestScope.userBasic.homeProvince=='贵州'}">selected="selected"</c:if>>贵州省</option>
<option value="四川" <c:if test="${requestScope.userBasic.homeProvince=='四川'}">selected="selected"</c:if>>四川省</option>
<option value="内蒙古" <c:if test="${requestScope.userBasic.homeProvince=='内蒙古'}">selected="selected"</c:if>>内蒙古</option>
<option value="宁夏" <c:if test="${requestScope.userBasic.homeProvince=='宁夏'}">selected="selected"</c:if>>宁夏</option>
<option value="甘肃" <c:if test="${requestScope.userBasic.homeProvince=='甘肃'}">selected="selected"</c:if>>甘肃省</option>
<option value="青海" <c:if test="${requestScope.userBasic.homeProvince=='青海'}">selected="selected"</c:if>>青海省</option>
<option value="西藏" <c:if test="${requestScope.userBasic.homeProvince=='西藏'}">selected="selected"</c:if>>西藏</option>
<option value="新疆" <c:if test="${requestScope.userBasic.homeProvince=='新疆'}">selected="selected"</c:if>>新疆</option>
<option value="安徽" <c:if test="${requestScope.userBasic.homeProvince=='安徽'}">selected="selected"</c:if>>安徽省</option>
<option value="浙江" <c:if test="${requestScope.userBasic.homeProvince=='浙江'}">selected="selected"</c:if>>浙江省</option>
<option value="福建" <c:if test="${requestScope.userBasic.homeProvince=='福建'}">selected="selected"</c:if>>福建省</option>
<option value="台湾" <c:if test="${requestScope.userBasic.homeProvince=='台湾'}">selected="selected"</c:if>>台湾省</option>
<option value="香港" <c:if test="${requestScope.userBasic.homeProvince=='香港'}">selected="selected"</c:if>>香港</option>
<option value="澳门" <c:if test="${requestScope.userBasic.homeProvince=='澳门'}">selected="selected"</c:if>>澳门</option>
<%--
<option value="其他" <c:if test="${requestScope.userBasic.homeProvince=='其他'}">selected="selected"</c:if>>其它</option>						
--%>
