<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<div id="gift_pop" style="display:none">
	<script>
		function sendGift(args){
			if(typeof args == "undefined"){
				params={giftId:0,fid:0,tagId:0};
			} else
			if(isNumber(args)){
				params={giftId:args,fid:0,tagId:0};
			} else {
				params=args;
			}
			var data="giftId="+params.giftId+"&fid="+params.fid+"&tagId="+params.tagId;
			if(typeof params.flashData !="undefined"){
				data+="&flashData="+params.flashData;
			}
			new Ajax.Request("${applicationScope.urlGift}/op/send/pop", 
				{
					method: "post",
					parameters: data,
					onSuccess: function(o){
						$('gift_pop').innerHTML=o.responseText;
						$('gift_pop').eval_inner_JS();
					}
				}
			); 
		}
		function reqGift(args){
			if(typeof args == "undefined"){
				params={giftId:0,fid:0};
			} else
			if(isNumber(args)){
				params={giftId:args,fid:0};
			} else {
				params=args;
			}
			new Ajax.Request("${applicationScope.urlGift}/op/send/reqPop", 
				{
					method: "post",
					parameters: "giftId="+params.giftId+"&fid="+params.fid,
					onSuccess: function(o){
						$('gift_pop').innerHTML=o.responseText;
						$('gift_pop').eval_inner_JS();
					}
				}
			); 
		}
	</script> 
</div>