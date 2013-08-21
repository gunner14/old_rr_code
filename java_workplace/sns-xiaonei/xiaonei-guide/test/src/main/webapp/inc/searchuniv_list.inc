<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/univscript.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/searchschlist.js?ver=$revxxx$"></script>

<div class="popupwrap" id="univlist" style="display:none;"  onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()">
	<div class="popup">
		<table id="univlist_provinces">
			<tr>
				<td id="anhui"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('anhui',tabs.changeTab_cb)">安徽</a></td>
				<td id="aomen"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('aomen',tabs.changeTab_cb)">澳门</a></td>
				<td id="beijing" class="active"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('beijing',tabs.changeTab_cb)">北京</a></td>
				<td id="chongqing"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('chongqing',tabs.changeTab_cb)">重庆</a></td>
				<td id="fujian"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('fujian',tabs.changeTab_cb)">福建</a></td>
				<td id="gansu"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('gansu',tabs.changeTab_cb)">甘肃</a></td>
				<td id="guangdong"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('guangdong',tabs.changeTab_cb)">广东</a></td>
				<td id="guangxi"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('guangxi',tabs.changeTab_cb)">广西</a></td>
				<td id="guizhou"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('guizhou',tabs.changeTab_cb)">贵州</a></td>
				<td id="hainan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('hainan',tabs.changeTab_cb)">海南</a></td>
				<td id="hebei"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('hebei',tabs.changeTab_cb)">河北</a></td>
				<td id="henan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('henan',tabs.changeTab_cb)">河南</a></td>
				<td id="hlj"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('hlj',tabs.changeTab_cb)">黑龙江</a></td>
				<td id="hubei"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('hubei',tabs.changeTab_cb)">湖北</a></td>
				<td id="hunan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('hunan',tabs.changeTab_cb)">湖南</a></td>
				<td id="jilin"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('jilin',tabs.changeTab_cb)">吉林</a></td>
				<td id="jiangsu"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('jiangsu',tabs.changeTab_cb)">江苏</a></td>
			</tr>
			<tr>
				<td id="jiangxi"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('jiangxi',tabs.changeTab_cb)">江西</a></td>
				<td id="liaoning"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('liaoning',tabs.changeTab_cb)">辽宁</a></td>
				<td id="neimeng"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('neimeng',tabs.changeTab_cb)">内蒙</a></td>
				<td id="ningxia"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('ningxia',tabs.changeTab_cb)">宁夏</a></td>
				<td id="qinghai"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('qinghai',tabs.changeTab_cb)">青海</a></td>
				<td id="shandong"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('shandong',tabs.changeTab_cb)">山东</a></td>
				<td id="taiyuan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('taiyuan',tabs.changeTab_cb)">山西</a></td>
				<td id="xian"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('xian',tabs.changeTab_cb)">陕西</a></td>
				<td id="shanghai"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('shanghai',tabs.changeTab_cb)">上海</a></td>
				<td id="sichuan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('sichuan',tabs.changeTab_cb)">四川</a></td>
				<td id="taiwan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('taiwan',tabs.changeTab_cb)">台湾</a></td>
				<td id="tianjin"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('tianjin',tabs.changeTab_cb)">天津</a></td>
				<td id="xizang"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('xizang',tabs.changeTab_cb)">西藏</a></td>
				<td id="xinjiang"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('xinjiang',tabs.changeTab_cb)">新疆</a></td>
				<td id="xianggang"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('xianggang',tabs.changeTab_cb)">香港</a></td>
				<td id="yunnan"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('yunnan',tabs.changeTab_cb)">云南</a></td>
				<td id="zhejiang"><a href="#" onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()" onclick="return tabs.changeTab('zhejiang',tabs.changeTab_cb)">浙江</a></td>
			</tr>
		</table>
		<ul id="univlist_univs">
		</ul>
		<p class="operation"><input type="button" value="关闭窗口" class="canbutton" onclick="todo='hide'; return hidepan()" /></p>
	</div>
</div>

<script type="text/javascript">
	tabs.changeTab('beijing',tabs.changeTab_cb)
</script>
