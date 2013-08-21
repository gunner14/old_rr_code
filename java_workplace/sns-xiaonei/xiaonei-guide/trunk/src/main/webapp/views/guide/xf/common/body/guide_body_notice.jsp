<%@ page pageEncoding="utf-8" %>
<script type="text/javascript">
	// <![CDATA[
	XN.DOM.ready(function(){
           announcement_show('notice_system','${requestScope.visiter.id}')
    });           
    function closeNotice(){               
           announcement_close('${requestScope.visiter.id}','notice_system'); 
    }             
    function clickNotice(url,openWin){
           announcement_click( '${requestScope.visiter.id}' , url , openWin , 'notice_system' );
    }
	// ]]>
</script>
<div id="notice_system"></div>

