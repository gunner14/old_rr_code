<script type="text/javascript">
	
document.domain = "renren.com";
function ifrmaeAdd(){
	var Chrome19 = navigator.userAgent.indexOf("Chrome/19"),
		Chrome18 = navigator.userAgent.indexOf("Chrome/18"),
		IE9 = navigator.userAgent.indexOf("MSIE 9.0");
	if( Chrome19 > 0 || Chrome18 > 0 || IE9 > 0 ){
		iframeUrl = "http://login.renren.com/Widget.do?loginRegistrationId=1&origURL=http%3A%2F%2Fwww.renren.com%2Fhome&callBackCssUrl=http://s.xnimg.cn/n/apps/login/compatible/test-all-min.css";
	}else{
		iframeUrl = "https://login.renren.com/Widget.do?loginRegistrationId=1&origURL=http%3A%2F%2Fwww.renren.com%2Fhome&callBackCssUrl=http://s.xnimg.cn/n/apps/login/compatible/test-all-min.css";
	}
	if($('iframelogin')) $("iframelogin").src = iframeUrl;
}
window.onload = ifrmaeAdd; 
</script>

<iframe name="iframelogin" id="iframelogin" width="100%" height="0" frameborder="0" allowtransparency="true" scrolling="no"></iframe>