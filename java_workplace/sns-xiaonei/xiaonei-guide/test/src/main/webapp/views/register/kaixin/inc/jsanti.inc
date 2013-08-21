	<input type="hidden" name="randuid" value="${requestScope.randuid}" id="randuid"/>
	<input type="hidden" name="biz" value=0 id="xn_biz"/>	
	<% if(null != request.getAttribute("xn_biz_rnd_function")){
		out.println(request.getAttribute("xn_biz_rnd_function")); 
	}
	%>
	