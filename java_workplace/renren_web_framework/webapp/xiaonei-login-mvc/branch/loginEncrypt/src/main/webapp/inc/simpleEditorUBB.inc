<script type="text/javascript">
document.domain='${applicationScope.sysdomain}';
</script>
<link href="${applicationScope.urlSmallStatic}/css/editor.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/blog2.js?ver=$revxxx$"></script>
<div class="bxTool">
	<div id="left"></div>
	<div>
		<ul id="toolbar"></ul>
	</div>
</div>
<textarea style="width:460px;" name="body" id="cmtbody" cols="60" rows="6" tabindex="10"></textarea>
<div id="popup">
	<div id="faceImg"></div>
	<div id="shadow"></div>
	<div id="smallFace" onmouseover="this.selectDir()"></div>
</div>

<script type="text/javascript">
//edited by ray 4 the short cut submit
function quickSendEditorCon() {
	getEl("editorFormBtn").click();
}
</script>
