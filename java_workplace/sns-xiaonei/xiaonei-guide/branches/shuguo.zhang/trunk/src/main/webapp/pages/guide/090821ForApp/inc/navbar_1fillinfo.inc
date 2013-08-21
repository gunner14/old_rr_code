<%
Boolean is_less_17 = (Boolean)request.getAttribute("is_less_17");
if(is_less_17 == null) {is_less_17 =  false;}

if(is_less_17){
%>
	<ul class="steps first-step clearfix">
		<li class="current">
			<h3>第1步</h3>
			<span>寻找朋友</span>
		</li>
		<li>
			<h3>第2步</h3>
			<span>设置头像，进入首页</span>
		</li>
	</ul>
<%
}
else{
%>
	<ul class="steps first-step clearfix">
		<li class="current">
			<h3>第1步</h3>
			<span>完善个人信息</span>
		</li>
		<li>
			<h3>第2步</h3>
			<span>寻找朋友</span>
		</li>
		<li>
			<h3>第3步</h3>
			<span>设置头像，进入首页</span>
		</li>
	</ul>
<%
}
%>


