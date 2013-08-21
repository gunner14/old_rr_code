<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-tiles" prefix="tiles"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<c:if test="${!yurenInfo.dataMap.isEmpty()}">
<c:if test="${!empty yurenInfo}">
<div style="" class="fools-day-remind" id="vipYurenNewsFeed">
<div class="fools-header">
<h2>
<img src="${applicationScope.urlSmallStatic }/imgpro/icons/tomato.png" width="16" height="16" alt="" /><a href="http://act.${domainIdentified['sys.domain']}/yuren/home">整蛊大战:</a>
<span class="fools-words">你刚被${yurenInfo.friendsNum}个好友捉弄了，整蛊指数+${yurenInfo.addPoints}</span><a href="#nogo" onclick="zhengguPop()" class="repeatedly">重新播放</a>
</h2>
<span><a href="javascript:;" onclick="vip_attend(false)">退出活动</a> | <a href="javascript:;" onclick="$('vipYurenNewsFeed').style.display='none'">关闭</a></span>
</div>
<div class="fools-body"><c:forEach var="item" items="${yurenInfo.dataMap.asMap()}">
<div>
<span class="fools-title">${item.key.name}：</span>
<p><c:forEach var="user" items="${item.value}" varStatus="status">
<a href="http://www.${domainIdentified['sys.domain']}/profile.do?id=${user.id}" target="_blank" <c:if test="${user.keepUse}">class="lively-user"</c:if>>${user.name}</a><c:if test="${!status.last}">、</c:if></c:forEach></p>
</div></c:forEach></div>
</div>
<script>
vip_attend=function(join){
var msg = join ? "你是否要加入" : "您确定要退出整蛊大战吗？如果退出整蛊大战，好友将无法整蛊您，您的首页将不出现整蛊动画与提示，但仍可参加抽奖活动。为此您将被扣除20%的整蛊指数。";
window.vip_attend_do = function(join){
var url = "http://act." + XN.env.domain + "/yuren/op/";
url += join ? "attend" : "quit";
new Ajax.Request(url, {
method: "post",
onSuccess: function(o){
$('vipYurenNewsFeed').hide();
}
});
}
XN.DO.confirm({
title: "退出活动",
message: msg,
params:{showCloseButton: true},
callBack: function(r){
if (r) {
vip_attend_do(join);
}
}
})
}
</script>
<div id="fflashBox" style="z-index:2010;position:absolute;top:0;left:0">
<div id="fflash">
</div>
<script type="text/javascript" src="http://s.xnimg.cn/xnapp/common/js/swfobjectv2.2.js"></script>
<script type="text/javascript">
/**
* flash播放完 调用函数
*/
window.closeTickFlash=function(){
setTimeout(function(){
var flashBox=$("fflashBox");
flashBox.innerHTML='<div id="fflash">&nbsp;</div>';
flashBox.hide();
XN.dom.enable();
},300);
}
window.checkFlashVersion=function(){
	try{
		var version=10;
		var userVersion=swfobject.getFlashPlayerVersion().major;
		if(userVersion<version){
			return false;
		}
	}catch(e){
	}
	return true;
}

var windowHeight = window.innerHeight ? window.innerHeight : document.documentElement.clientHeight;
var windowWidth=window.innerWidth ? window.innerWidth: document.documentElement.clientWidth;
var flashvars = {
data: encodeURIComponent('${yurenInfo.dataJson}'),
max: 10
};
var player = XN.env.staticRoot + "xnapp/vip/swfpro/activity/trick_3.swf";
function zhengguPop(){
if(!checkFlashVersion()){
	return false;
}
XN.dom.disable();
var flashBox=$("fflashBox");
var isIE6 = (!! window.ActiveXObject) &&( ! window.XMLHttpRequest);
if(!isIE6){
flashBox.style.position="fixed";
}
flashBox.show();
try{
	swfobject.embedSWF(player, 'fflash', windowWidth,windowHeight , "9.0.0", "", flashvars , {
	"allowScriptAccess": 'always',
	"wmode": "transparent",
	"menu": false
	});
}catch(e){
	XN.dom.enable();
}
}

XN.DOM.readyDo(function(){zhengguPop()});
</script>
</div></c:if>
</c:if>