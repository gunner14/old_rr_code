<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript">
document.domain='${applicationScope.sysdomain}';
var isClicked = false;
function onReferClick()
{	
	isClicked = true;
	if(isAllowed)
		SetFocus("simplerte");
}
function setCite(citeownerid, citeownername)
{
	getEl("citeid").value = citeownerid;
	getEl("citename").value = citeownername;
}
var isAllowed = false;
function CiteClickNew(itemId,itemName,full)
{
		ss1=getEl(itemId).innerHTML;   
		index = ss1.indexOf("<br>"); 
		if(!full) 
		{
			ss2=ss1.substring(0,index);
			ss1= "-------------" + ">>［回复" + itemName +"］" + "\n" +  ">>" +  ss2;
		}
		else
		{
			if(navigator.appName=="Netscape")
				{
					ss1=replaceSubstring(ss1, "<br>", ">>");
					ss1=replaceSubstring(ss1, "<BR>", ">>");
					ss1=replaceSubstring(ss1,"&gt;&gt;",">>");
					ss1= "\n" + "\n" + "-------------\n" + ">>[" + itemName +"]" + "提到:" +"\n" +  ">>" +  ss1;
				}
			else
				{
					ss1=replaceSubstring(ss1, "<br>", "\n>>");
					ss1=replaceSubstring(ss1, "<BR>", "\n>>");
					ss1=replaceSubstring(ss1,"&gt;&gt;",">>");
					ss1= "\n" + "\n" + "-------------\n" + ">>[" + itemName +"]" + "提到:"+ "\n" + ">>" +  ss1;
				}  
		}
		getEl("cmtbody").value = ss1;
		$('cmtbody').focus();
}
</script>
	<div id="simpleEditPan" class="bxTool" style="height:140px;">
		<%@ include file="/inc/simpleEditorUBB.inc" %>	
     </div>
	<%@ include file="/inc/upFile.inc" %>	
<p>
	<input type="hidden" name="citeid" id="citeid" value="0">
	<input type="hidden" name="citename" id="citename" value="">	
	<input type="hidden" name="submit" value="" />
	<input type="hidden" name="ak" value="${requestScope.ak}" />
	<input type="hidden" name="tsc" id="tsc_a" value="${temp_sys_code}" />
	<input type="hidden" name="ac" id="ac" value="${requestScope.ac}" />
	<input type="hidden" name="at" id="at" value="" />	
	<input type="submit" id="editorFormBtn" name="submit" class="subbutton" value=" 发表 " />
</p>
