<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/univscript.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/schlist.js?ver=0.2"></script>

<div id="topmenu"  onmouseover="return tabs.mouseIn()" onmouseout="return tabs.mouseOut()">
	<div id="d1">
		<table id="dialog" cellspacing="0" cellpadding="0" style="width:100%">
		<tr> 
			<td class="dialog" style="padding:10px 0">
				<%@ include file="/inc/schlist.inc" %>
				<div id="closepanbtn"><a href="#" onclick="todo='hide'; return hidepan()">关闭窗口</a></div>
			</td>
		</tr>
		</table>
	</div> 
</div>

