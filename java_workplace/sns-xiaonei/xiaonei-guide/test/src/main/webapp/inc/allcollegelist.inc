<%@ page pageEncoding="UTF-8" %>
<div id="schoolList" style="display:none; width:600px; " class="popup-wrapper">
    <div class="popup-content">
        <ul id="popup-province">
        	<li id="p_1" class="active"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('1');">北京</a></li>
            <li id="p_2"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('2');">上海</a></li>
            <li id="p_3"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('3');">天津</a></li>
            <li id="p_4"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('4');">重庆</a></li>
            <li id="p_5" style="width:40px;"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('5');">黑龙江</a></li>
            <li id="p_6"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('6');">吉林</a></li>
            <li id="p_7"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('7');">辽宁</a></li>
            <li id="p_8"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('8');">山东</a></li>
            <li id="p_9"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('9');">山西</a></li>
            <li id="p_10"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('10');">陕西</a></li>
            <li id="p_11"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('11');">河北</a></li>
            <li id="p_12"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('12');">河南</a></li>
            <li id="p_13"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('13');">湖北</a></li>
            <li id="p_14"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('14');">湖南</a></li>
            <li id="p_15"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('15');">海南</a></li>
            <li id="p_16"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('16');">江苏</a></li>
            <li id="p_17"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('17');">江西</a></li>
            <li id="p_18"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('18');">广东</a></li>
            <li id="p_19"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('19');">广西</a></li>
            <li id="p_20"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('20');">云南</a></li>
            <li id="p_21"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('21');">贵州</a></li>
            <li id="p_22"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('22');">四川</a></li>
            <li id="p_23" style="width:40px;"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('23');">内蒙古</a></li>
            <li id="p_24"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('24');">宁夏</a></li>
            <li id="p_25"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('25');">甘肃</a></li>
            <li id="p_26"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('26');">青海</a></li>
			<li id="p_27"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('27');">西藏</a></li>
			<li id="p_28"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('28');">新疆</a></li>
			<li id="p_29"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('29');">安徽</a></li>
			<li id="p_30"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('30');">浙江</a></li>
			<li id="p_31"><a onfocus="this.blur()" href="javascript:void(0);" onclick="javascript:SchoolComponent.changeProv('31');">福建</a></li>           
        </ul>
        <ul id="popup-city" class="module-popupcity">            
        </ul>
        <div id="schoolTabPan" class="clear"></div>
		<ul id="schoolListContentUl" class="module-schoollist"><center style="padding-top:94px;">请选择你的所在区</center></ul>
        <input id="closeButton" type="button" onclick="return SchoolComponent.hide()" class="canbutton float-right" value="关闭窗口"/>
    </div>
<!--[if lte IE 6.5]><iframe></iframe><![endif]-->
</div>