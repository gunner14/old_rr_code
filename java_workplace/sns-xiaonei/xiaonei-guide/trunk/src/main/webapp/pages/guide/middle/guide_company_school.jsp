<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<c:if test="${!empty requestScope.others}">
	<c:forEach varStatus="status" var="othersItem" items="${requestScope.others}">
		<c:if test="${othersItem.count>0}">
			<script>var j${status.index}=0;</script>
			<div class="users">
				<div class="header">
					<h4>我们在  ${othersItem.typeName} 为你找到了${othersItem.count} 
						<c:choose>
							<c:when test="${othersItem.type=='work'}">
								位同事 &raquo;
							</c:when>
							<c:otherwise>
								位同学 &raquo;
							</c:otherwise>
						</c:choose>
					</h4>						
				</div>
				<ul class="figurepanels">
				</ul>
				<div class="footer">
					<span id="companyschoolbtn${status.index}" class="button" onclick="showMore(this, mayknown[${status.index+scIndex}], j${status.index}++);return false;">
						<button>更多 
							<strong>${othersItem.typeName}</strong> 
							<c:choose>
								<c:when test="${othersItem.type=='work'}">
									的同事 &raquo;
								</c:when>
								<c:otherwise>
									的同学 &raquo;
								</c:otherwise>
							</c:choose>
						</button>
					</span>						
					<p style="display:none">还没人认识？
							<c:choose>
								<c:when test="${visiter.selected!=1}">
									<a href="${applicationScope.urlHeadUpload}/Upload.do?f=gmk">完善资料</a>						
								</c:when>
								<c:when test="${buildSchedule.scheduleUniv==0}">
									<a href="${applicationScope.urlMain}/MyEducationInfo.do?f=gmk">完善资料</a>
								</c:when>
								<c:when test="${buildSchedule.schedulePersonalInfo==0}">
									<a href="${applicationScope.urlMain}/PersonalInfo.do?f=gmk">完善资料</a>
								</c:when>
								<c:when test="${buildSchedule.scheduleWork==0}">
									<a href="${applicationScope.urlMain}/MyWorkInfo.do?f=gmk">完善资料</a>
								</c:when>
								<c:otherwise>
									<a href="${applicationScope.urlWww}/profile.do?id=${requestScope.visiter.id}&amp;v=info&amp;act=edit&amp;f=gmk">完善资料</a>
								</c:otherwise>										
							</c:choose>
							或者
							<a href=${applicationScope.urlBrowse}/searchEx.do?from=guide&amp;${othersItem.moreLink}>再找找其他<c:choose><c:when test="${othersItem.type=='work'}">同事 </c:when><c:otherwise>同学</c:otherwise></c:choose></a>
							试试
						</p>
				</div>					
				<script>showMore($('companyschoolbtn${status.index}'), mayknown[${status.index+scIndex}], j${status.index}++);</script>		
			</div>		
		</c:if>
	</c:forEach>
</c:if>		