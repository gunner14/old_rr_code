<%@ page pageEncoding="UTF-8" %>
<c:choose>                                                                                                                                                                            
	<c:when test="${!empty requestScope.randomIMPopular}">                                                                                                                              
	<div class="box">                                                                                                                                                                   
		<h3>校内通人气之星</h3>                                                                                                                                                                
		<div class="boxcont">                                                                                                                                                             
			<table class="personIconList">                                                                                                                                                
				<tr>   
					<c:forEach varStatus="status" var="item" items="${requestScope.randomIMPopular}">       
					<c:set var="usr" value="${item}" />                                                                                                                                                            
					<td class="personInfo"><div class="personIcon">                                                                                                                             
							<table>                                                                                                                                                                 
								<tr>                                                                                                                                                                  
									<td><a target="_blank" href="${applicationScope.urlMain}/profile.do?portal=popularHome&id=${usr.id}"><img src="${usr.tiny_Url}" alt="${usr.name}" /></a></td>              
								</tr>                                                                                                                                                                 
							</table>                                                                                                                                                                
						</div>                                                                                                                                                                    
						<div class="personName"><a target="_blank" href="${applicationScope.urlMain}/profile.do?portal=popularHome&id=${usr.id}">${usr.name}</a></div></td>     
					</c:forEach>
				</tr>
			</table>                                                                                                                                                                        
		</div>                                                                                                                                                                            
	</div> 
	</c:when>                                                                                                                                                                             
</c:choose>
