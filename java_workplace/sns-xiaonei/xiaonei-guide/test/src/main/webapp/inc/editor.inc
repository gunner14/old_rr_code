<script type="text/javascript">
document.domain='${applicationScope.sysdomain}';
</script>
<iframe src="http://${pageContext.request.getServerName()}/pages/editor/editor.htm?ver=545" frameborder="0" id="rte" class="rte" onload="if(typeof(fnEditorOnLoad) == 'function') {fnEditorOnLoad()}"></iframe>
<script type="text/javascript">
//added by pekkyl for the short cut submit
function quickSendEditorCon() {
	getEl("editorFormBtn").click();
}
</script>
