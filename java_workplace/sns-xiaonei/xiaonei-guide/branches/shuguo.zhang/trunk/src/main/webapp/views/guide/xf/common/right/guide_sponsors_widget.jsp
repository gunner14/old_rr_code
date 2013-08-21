<%@ page pageEncoding="utf-8" %>
<c:choose>	
	<c:when test="${requestScope.GUIDE_RENREN_SCHOOL==1}">
		<div style="">
			<div class="side-item sponsors">
				<!-- header部分 -->	
				<div class="side-item-header clearfix">	
					<!--整个模块的标题 -->    
					<h4>	
						<!--将下面一段中的url替换成点击后需要打开的URL地址 -->       	
						<a target="_blank" href="http://school.${applicationScope.sysdomain}">高考胜利！！！你要报哪里？</a>    
					</h4>   	
				</div>  
				<div class="side-item-body clearfix">		
					<!-- 图像部分 -->    
					<div class="figure"> 	
						<!-- 将下面一段中的url替换成点击图片后需要打开的URL地址 -->    	
						<a target="_blank" href="http://school.${applicationScope.sysdomain}" class="link"> 
							<img width="100" height="80" src="${applicationScope.urlStatic}/imgpro/guide/school.png">
						</a> 
						<!--- 图片放到 ftp:// -->     
					</div>	
					<!-- END 图像部分 -->          
					<!-- 正文右侧文案部分，如果没有文字(比如全宽图片)可以把下面的div给删了 -->     
					<div class="info">            
						<p>	  
							<!-- 将下面一段中的url替换成点击 文案 后需要打开的URL地址 -->          	
							<a target="_blank" href="http://school.${applicationScope.sysdomain}" class="text">
								人人网校园频道，<br>探索全国大学，<br>向师兄提问，<br>打听专业内幕！
							</a>      
						</p>	  
						<!-- 将下面一段中的url替换成点击 详细 后需要打开的URL地址，“详细”可以改成其他文字 -->       
						<a target="_blank" href="http://school.${applicationScope.sysdomain}" class="link">校园频道>></a>          
					</div>  
				</div>
			</div>
		</div>
	</c:when>
	<c:when test="${requestScope.BarTreasure}">
		<div style=""> 
			<div class="side-item wide-sponsors"> 
				<!-- header部分 -->  
				<div class="side-item-header clearfix"> 
					<!--整个模块的标题 -->   
					<h4> 
						<a target="_blank" href="http://huodong.${applicationScope.sysdomain}/duobao">网吧夺宝大行动</a> 
					</h4> 
					<div class="option sw-buttons-holder" style="display:none;"> 
						<a title="下一个更精彩" onfocus="this.blur();" href="javascript:;" name="jebe_button" class="sw-refresh-button" id="widgetRefresh"></a>
						<a title="关闭" onfocus="this.blur();" href="javascript:;" name="jebe_button" class="sw-x-button" id="widgetClose"></a>
					</div>
					<!--END　整个模块的标题 --> 
				</div> 
				<!-- 主体内容 --> 
				<div class="side-item-body clearfix">    
					<!-- 图像部分 --> 
					<div class="figure">  
						<a target="_blank" href="http://huodong.${applicationScope.sysdomain}/duobao" class="link"> 
							<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/indiana.gif">
						</a>  
					</div>
					<!-- END 图像部分 --> 
				</div>
			</div>
		</div>
	</c:when>
</c:choose>