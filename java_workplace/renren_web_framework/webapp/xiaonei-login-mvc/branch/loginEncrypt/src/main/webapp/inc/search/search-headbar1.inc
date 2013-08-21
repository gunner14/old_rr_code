<c:if test="${empty requestScope.p}">
<script type="text/javascript">
<%--speed up login page by lu.hua--%>
if (document.body.id != 'welcomepage') {
    document.write('<script ' + 'type="text/javascript" src="${applicationScope.urlSmallStatic}/js/searchschlist.js?ver=$revxxx$" ></' + 'script>');
    document.write('<script ' + 'type="text/javascript" src="${applicationScope.urlSmallStatic}/allunivlist.js?ver=$revxxx$"></' + 'script>');
}
<%--end--%>
</script>
<div id="univlist" style="display:none;" class="popup-wrapper" onmouseover="return univtabs.mouseIn()" onmouseout="return univtabs.mouseOut()">
<!--[if lte IE 6.5]><iframe></iframe><![endif]-->
<div class="popup-content">
	<ul id="popup-country">
		<li id="c_0" class="active"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('0');">中国</a></li>
		<li id="c_7"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('7');">美国</a></li>
		<li id="c_6"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('6');">加拿大</a></li>
		<li id="c_5"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('5');">英国</a></li>
		<li id="c_1"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('1');">澳大利亚</a></li>
		<li id="c_2"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('2');">法国</a></li>
		<li id="c_8"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('8');">德国</a></li>
		<li id="c_4"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('4');">新西兰</a></li>
		<li id="c_3"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('3');">新加坡</a></li>
		<li id="c_9"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('9');">韩国</a></li>
		<li id="c_10"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('10');">俄罗斯</a></li>
		<li id="c_11"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('11');">日本</a></li>
		<li id="c_12"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('12');">意大利</a></li>
		<li id="c_13"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('13');">爱尔兰</a></li>
		<li id="c_14"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('14');">荷兰</a></li>
		<li id="c_15"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('15');">马来西亚</a></li>
		<li id="c_16"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('16');">瑞士</a></li>
		<li id="c_17"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('17');">泰国</a></li>
		<li id="c_18"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('18');">乌克兰</a></li>
		<li id="c_19"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('19');">南非</a></li>
		<li id="c_20"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('20');">芬兰</a></li>
		<li id="c_21"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('21');">瑞典</a></li>
		<li id="c_22"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('22');">西班牙</a></li>
		<li id="c_23"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('23');">比利时</a></li>
		<li id="c_24"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('24');">挪威</a></li>
		<li id="c_25"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('25');">丹麦</a></li>
		<li id="c_26"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('26');">菲律宾</a></li>
		<li id="c_27"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('27');">波兰</a></li>
		<li id="c_28"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('28');">印度</a></li>
		<li id="c_29"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:univtabs.changeCountry('29');">奥地利</a></li>
	</ul>
<ul id="popup-province">            
</ul>
<ul id="popup-unis">            
</ul>
<input id="closeButton" type="button" onclick="todo='hide'; return hidepan()" class="canbutton float-right" value="关闭窗口"/>
</div>
<!--[if lte IE 6.5]><iframe></iframe><![endif]-->
</div>
<script type="text/javascript">
<%--for speed up login page--%>

if (document.body.id != 'welcomepage') {
    univtabs.changeCountry('0');
}
</script>
<div class="select-filter SelectBasic"  <c:if test="${!(requestScope.s!=2 && requestScope.rst=='ok')}">style="display:none;"</c:if>　 id="refilter">
<form id="filterForm"><p>
	<label>筛选：</label>
	<select name="gend" id="gend" onchange="XN.app.search.filterGend(this);">
	<option value="" selected="selected">选择性别</option>
	<option value="男生">男</option>
	<option value="女生">女</option>
	</select>
	<select name="homeProvince" id="homeProvince"  onchange="XN.app.search.filterHome(this);XN.app.search.getCityByChangeProvinceid(this.selectedIndex,'codeCity')">
	<option selected="selected" value="">家乡省份</option>
	<option value="北京">北京市</option>
	<option value="上海">上海市</option>
	<option value="天津">天津市</option>
	<option value="重庆">重庆市</option>
	<option value="黑龙江">黑龙江省</option>
	<option value="吉林">吉林省</option>
	<option value="辽宁">辽宁省</option>
	<option value="山东">山东省</option>
	<option value="山西">山西省</option>
	<option value="陕西">陕西省</option>
	<option value="河北">河北省</option>
	<option value="河南">河南省</option>
	<option value="湖北">湖北省</option>
	<option value="湖南">湖南省</option>
	<option value="海南">海南省</option>
	<option value="江苏">江苏省</option>
	<option value="江西">江西省</option>
	<option value="广东">广东省</option>
	<option value="广西">广西</option>
	<option value="云南">云南省</option>
	<option value="贵州">贵州省</option>
	<option value="四川">四川省</option>
	<option value="内蒙古">内蒙古</option>
	<option value="宁夏">宁夏</option>
	<option value="甘肃">甘肃省</option>
	<option value="青海">青海省</option>
	<option value="西藏">西藏</option>
	<option value="新疆">新疆</option>
	<option value="安徽">安徽省</option>
	<option value="浙江">浙江省</option>
	<option value="福建">福建省</option>
	<option value="台湾">台湾省</option>
	<option value="香港">香港</option>
	<option value="澳门">澳门</option>
	</select>
	<select name="codeCity" id="codeCity" <%--onblur="XN.app.search.changeCodeCity('codeCity',this.selectedIndex)"--%> onchange="XN.app.search.filterHome(this);">
	<option value="">城市</option>
	</select>
	<input type="text" onclick="todo='show';XN.app.search.showpan('nuniv','nunivid'); bodyclick=false;return false" class="input-text" id="nuniv" value="请选择大学" name="nuniv"/>
	<a href="javascript:void(0);" id="resetSch" style="display:none;" onclick="$('nuniv').value=$('nunivBtm').value='请选择大学';$('nunivid').value=$('nunividBtm').value='';XN.app.search.filterUniv();">清空大学</a>
	<input type="hidden" name="nunivid" id="nunivid" />
	<input type="hidden" name="adv_univId" id="adv_univId" />
	</p>
</form>
</div>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/search2.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/cityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/searchclassfangle.js?ver=$revxxx$"></script>
<script type="text/javascript">
searchType = 'refilter';
</script></c:if><c:if test="${not empty requestScope.p}">
<div class="select-filter clearfix">
	<p>您的搜索条件包括:&nbsp;&nbsp;</p>
	<a href="http://browse.${applicationScope.domain}/search.do" class="return-btn">重新搜索</a>
	<ul id="searchCon">
	</ul>
</div></c:if>
