<%@ page contentType="text/html;charset=UTF-8" %>
<c:if test="${networkid_ != 0}">
	<c:choose>
		<c:when test="${networkid_ >= 1001 && networkid_ <= 19999999}">学校</c:when>
		<c:otherwise>
			<c:choose>
				<c:when test="${networkid_ >= 20000001 && networkid_ <= 29999999}">公司</c:when>
				<c:otherwise>
					<c:choose>
						<c:when test="${networkid_ >= 30000001 && networkid_ <= 49999999}">学校</c:when>
						<c:otherwise>
							<c:choose>
								<c:when test="${networkid_ >= 60000001 && networkid_ <= 69999999}">城市</c:when>
								<c:otherwise>
									<c:if test="${networkid_ >= 80000000 && networkid_ <= 90000000}">学校</c:if>
								</c:otherwise>
							</c:choose>
						</c:otherwise>
					</c:choose>
				</c:otherwise>
			</c:choose>
		</c:otherwise>
	</c:choose>
</c:if>