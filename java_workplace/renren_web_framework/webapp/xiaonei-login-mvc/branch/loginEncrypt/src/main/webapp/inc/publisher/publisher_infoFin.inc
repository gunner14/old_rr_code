<c:choose>
	<c:when test="${(visiter.univ >= 20000001 && visiter.univ <= 29999999) || (visiter.univ >= 60000001 && visiter.univ <= 69999999)}">
		<%-- 白领资料完整度开始 --%>
		<c:if test="${buildSchedule.scheduleBasic==0 || buildSchedule.scheduleUniv==0 || buildSchedule.scheduleWork==0 || buildSchedule.schedulePersonalInfo==0}">
			<div class="info-integrity">
				<ul>
					<c:choose>
						<c:when test="${buildSchedule.scheduleBasic==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlWww}/BasicInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-basic-b.gif" alt="未完成"/> <font color="gray">基本资料</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<c:choose>
								<c:when test="${visiter.selected==0}">
									<li class="incomplete">
										<a href="http://head.upload.${applicationScope.domain}/Upload.do">
											<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-avatar-b.gif" alt="未完成" /> <font color="gray">上传头像</font>
										</a>
									</li>
								</c:when>
								<c:otherwise>
									<li class="complete">
										<a href="http://head.upload.${applicationScope.domain}/Upload.do">
											<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-avatar.gif" alt="已完成" /> <font color="black">上传头像</font>
										</a>
									</li>
								</c:otherwise>
							</c:choose>
						</c:otherwise>
					</c:choose>
					<c:choose>
						<c:when test="${buildSchedule.scheduleUniv==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlProfile}/MyEducationInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-school-b.gif" alt="未完成" /> <font color="gray">学校信息</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="${applicationScope.urlProfile}/MyEducationInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-school.gif" alt="已完成" /> <font color="black">学校信息</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>
					<c:choose>
						<c:when test="${buildSchedule.scheduleWork==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlProfile}/MyWorkInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-work-b.gif" alt="未完成" /> <font color="gray">工作信息</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="${applicationScope.urlProfile}/MyWorkInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-work.gif" alt="已完成" /> <font color="black">工作信息</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>
					<c:choose>
						<c:when test="${buildSchedule.schedulePersonalInfo==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlProfile}/PersonalInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-fav-b.gif" alt="未完成" /> <font color="gray">个人喜好</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="${applicationScope.urlProfile}/PersonalInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-fav.gif" alt="已完成" /> <font color="black">个人喜好</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>		
				</ul>
				<p>
					您还有图标没有点亮，填写资料即可点亮图标，<c:choose>
					<c:when test="${buildSchedule.scheduleBasic==0}"><a href="${applicationScope.urlWww}/BasicInfo.do"></c:when>
					<c:when test="${buildSchedule.scheduleUniv==0}"><a href="${applicationScope.urlProfile}/MyEducationInfo.do"></c:when>
					<c:when test="${buildSchedule.scheduleWork==0}"><a href="${applicationScope.urlProfile}/MyWorkInfo.do"></c:when>
					<c:otherwise><a href="${applicationScope.urlProfile}/PersonalInfo.do"></c:otherwise></c:choose>马上去完善。</a>
				</p>
			</div>	
		</c:if>
		<%-- 白领资料完整度结束 --%>
	</c:when>
	<c:otherwise>
		<%-- 高中和大学资料完整度开始 --%>
		<c:if test="${buildSchedule.scheduleBasic==0 || buildSchedule.scheduleUniv==0 || visiter.selected == 0 || buildSchedule.schedulePersonalInfo==0}">
			<div class="info-integrity">
				<ul>
					<c:choose>
						<c:when test="${buildSchedule.scheduleBasic==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlWww}/BasicInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-basic-b.gif" alt="未完成"/> <font color="gray">基本资料</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="${applicationScope.urlWww}/BasicInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-basic.gif" alt="已完成" /> <font color="black">基本资料</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>
					<c:choose>
						<c:when test="${visiter.selected==0}">
							<li class="incomplete">
								<a href="http://head.upload.${applicationScope.domain}/Upload.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-avatar-b.gif" alt="未完成" /> <font color="gray">上传头像</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="http://head.upload.${applicationScope.domain}/Upload.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-avatar.gif" alt="已完成" /> <font color="black">上传头像</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>
					<c:choose>
						<c:when test="${buildSchedule.scheduleUniv==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlProfile}/MyEducationInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-school-b.gif" alt="未完成" /> <font color="gray">学校信息</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="${applicationScope.urlProfile}/MyEducationInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-school.gif" alt="已完成" /> <font color="black">学校信息</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>			
					<c:choose>
						<c:when test="${buildSchedule.schedulePersonalInfo==0}">
							<li class="incomplete">
								<a href="${applicationScope.urlProfile}/PersonalInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-fav-b.gif" alt="未完成" /> <font color="gray">个人喜好</font>
								</a>
							</li>
						</c:when>
						<c:otherwise>
							<li class="complete">
								<a href="${applicationScope.urlProfile}/PersonalInfo.do">
									<img class="icon" src="${applicationScope.urlSmallStatic}/imgpro/icons/info-integrity-fav.gif" alt="已完成" /> <font color="black">个人喜好</font>
								</a>
							</li>
						</c:otherwise>
					</c:choose>		
				</ul>
				<p>
					您还有图标没有点亮，填写资料即可点亮图标，<c:choose>
					<c:when test="${buildSchedule.scheduleBasic==0}"><a href="${applicationScope.urlWww}/BasicInfo.do"></c:when>
					<c:when test="${visiter.selected==0}"><a href="http://head.upload.${applicationScope.domain}/Upload.do"></c:when>
					<c:when test="${buildSchedule.scheduleUniv==0}"><a href="${applicationScope.urlProfile}/MyEducationInfo.do"></c:when>			
					<c:otherwise><a href="${applicationScope.urlProfile}/PersonalInfo.do"></c:otherwise></c:choose>马上去完善。</a>
				</p>
			</div>	
		</c:if>
		<%-- 高中和大学资料完整度结束 --%>		
	</c:otherwise>
</c:choose>



