<%@ page pageEncoding="utf-8" %>
<c:if test="${requestScope.BarTreasure}">
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
</c:if>
