<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript">
//speed up login page by lu.hua
	if(document.body.id != 'welcomepage'){
		document.write('<script ' + 'type="text/javascript" src="${applicationScope.urlSmallStatic}/js/searchschlist.js?ver=$revxxx$" ></' + 'script>');
		document.write('<script ' + 'type="text/javascript" src="${applicationScope.urlSmallStatic}/allunivlist.js?ver=$revxxx$"></' + 'script>');
	}
//end
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
<script>
//for speed up login page
if(document.body.id != 'welcomepage'){
	univtabs.changeCountry('0');
}
</script>
