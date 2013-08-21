<%@ page pageEncoding="utf-8" %>
<script language="javascript">	
	//<![CDATA[
	 XN.DOM.readyDo(function(){    	
    	document.onclick=function(event){
			var e=event || window.event; 
			var srcElement = e.srcElement || e.target;
			gl = srcElement.getAttribute("g");
			if(gl){
				new Ajax.Request("guideClickStat.do?gl="+gl,{method:'get'});
			}
			else{
				if(srcElement.tagName == "A" || srcElement.tagName == "IMG" || srcElement.tagName == "a" || srcElement.tagName == "img"){
					var temp = srcElement;
					while(temp.parentNode){
						temp = temp.parentNode;
						if(temp.getAttribute("g")){
							gl = temp.getAttribute("g");
							var url="";
							if(gl){
								if(srcElement.href){
									if(srcElement.href.indexOf("profile") != -1){
										gl = gl.split("_")[0] + "_80";
									}
									url=escape(srcElement.href);
								}
							}							
							new Ajax.Request("guideClickStat.do?gl="+gl+"&url=" + url,{method:'get'});
							return;
						}	
					}
				}
			}
		}
	});
	// ]]>
</script>
<%
try{        	
    long beginPopGame= LogTimeUtils.logBegin();                     
    MemCacheManager mem = com.xiaonei.platform.core.cache.MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);  
    if(user.getLoginCount()<=2 && user.getStage()!=20 && !"popped".equals(mem.get("GuidePopGames"+user.getId()))){
    	mem.set("GuidePopGames"+user.getId(), "popped", (int)com.xiaonei.platform.core.cache.MemCacheKeys.hour*24);
%>
		<c:set var="poped" value="no" />
<%
    	request.setAttribute("GuidePopGames", "nopoped");
    }            
    LogTimeUtils.logEnd(beginPopGame, "EndFeedInfo");              		
 }
 catch(Exception e){
    e.printStackTrace();
 } 
%>
<script>
	XN.dom.ready(function(){
		if(${poped=='no' && domainIdentified['siteName']=='人人'}){
			setTimeout(function(){
				var w = window.open('${applicationScope.urlGuide}/popgame.do');
				if ( !w ){
					document.body.onclick = function(){
						document.body.onclick = null;
						window.open('${applicationScope.urlGuide}/popgame.do');
					}
				}
			},2000);
		}		
	});			
</script>