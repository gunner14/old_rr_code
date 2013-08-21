<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<div class="main_right">
			<h2>领养一只狗狗做你的宠物</h2>
			<div class="dog_flash">
				<div id="imflash"></div>
				<script type="text/javascript">
						var flash_var = new SWFObject("${applicationScope.urlStatic}/swf/dogformailreg.swf", "dog", "180", "160", "9", ""); 
						flash_var.write("imflash");
				</script>
				<h3>黄色拉布拉多</h3>
				<p>年龄：两个月大</p>
			</div>
			<ul class="clearfix">
				<li id="dog_1"></li>
				<li id="dog_2"></li>
				<li id="dog_3"></li>
				<li id="dog_4"></li>
			</ul>
		</div>