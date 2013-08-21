<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<div class="generic_dialog explanation pop_dialog">
	<div class="generic_dialog_popup" style="top: 182px;">
		<table id="pop_dialog_table" class="pop_dialog_table" style="width:480px;">
			<tbody>
				<tr>
					<td class="pop_topleft"/>
					<td class="pop_border"/>
					<td class="pop_topright"/>
				</tr>
				<tr>
					<td class="pop_border"/>
					<td id="pop_content" class="pop_content">
						<h2><span>修改资料<a href="javascript:;">关闭</a></span></h2>
						<div class="dialog_content">
							<form action="/ajaxGuideSaveInfo.do" method="post" name="myEducationForm" id="educationInfo_form">
							<div class="dialog_body">
								<!--dialog_body begin-->
								<div class="fill-data-box">
									<div class="main-data">
										
											<table>
												<c:forEach varStatus="status" var="item" items="${requestScope.myUniversityList}">	
													<tr <c:if test="${empty item}">style="display:none;"</c:if>>
														<td class="td-name">大学 : </td>
														<td>
															<input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_high_junior" value="${item.universityName}"/>
															<input type="hidden" id="mid_high_junior_id"  name="juniorhighschoolcode"/>
															<select class="select" name="juniorhighschoolyear" id="mid_high_junior_year" ><option value="">入学年份</option></select>
														</td>
													</tr>
												</c:forEach>
												<c:forEach varStatus="status" var="item" items="${requestScope.myHighschoolList}" >    
													<tr <c:if test="${empty item}">style="display:none;"</c:if>>
														<td class="td-name" valign="top">高中 : </td>
														<td>
															<table>
																<tr>
																	<td>
																		<input type="text" class="input-text s-select s-high" name="highschoolname" id="mid_high_high" value="${item.highSchoolName}"/> 
																		<input type="hidden" name="highschoolcode" id="mid_high_high_id"/>
																		<select class="select" name="highschoolyear" id="mid_high_year">
																			<option value="">入学年份</option>
																		</select>
																	</td>
																</tr>
																<tr>
																	<td>高一 <input type="text" class="input-text digital" value="${item.hClass1}"/> 班</td>
																</tr>
																<tr>
																	<td>高二 <input type="text" class="input-text digital" value="${item.hClass2}"/> 班</td>
																</tr>
																<tr>
																	<td>高三 <input type="text" class="input-text digital" value="${item.hClass3}"/> 班</td>
																</tr>
															</table>
														</td>
													</tr>
												</c:forEach>
												<tr>
													<td class="td-name">中专技校 : </td>
													<td>
														<input type="text" class="input-text s-select s-tech" name="techschoolname" id="mid_tech_tech" value="${requestScope.collegeInfo.collegeName}"/> 
														<input type="hidden" id="mid_tech_tech_id" value="0" name="techschoolcode"/>
														<select class="select"  name="techschoolyear"><option value="">入学年份</option></select>
													</td>
												</tr>			
												<tr>
													<td class="td-name">初中 : </td>
													<td>
														<input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_high_junior" value="${requestScope.userJun.junHighentarySchoolName}"/>
														<input type="hidden" id="mid_high_junior_id"  name="juniorhighschoolcode"/>
														<select class="select" name="juniorhighschoolyear" id="mid_high_junior_year" ><option value="">入学年份</option></select>
													</td>
												</tr>
												<tr>
													<td class="td-name">小学 : </td>
													<td>
														<input type="text" class="input-text" id="mid_junior_entary" name="elementaryschoolname" value="${requestScope.userEle.elementarySchoolName}"/>
														<input  type="hidden" id="mid_junior_entary_id" name="elementaryschoolcode"/>
														<select class="select" ><option value="">入学年份</option></select>
													</td>
								
												</tr>
											</table>
										</form>
									</div><!--main-data end-->
									<div class="main-data online">
										<form>
											<table>
												<tbody>
													<c:forEach varStatus="status" var="item" items="${requestScope.myWorkList}">
														<tr>
															<td class="td-name">单位 : </td>
															<td><input type="text" class="input-text" value="${item.workplaceName}"></td>
														</tr>
													</c:forEach>
													<tr>
														<td class="td-name">所在地 : </td>
														<td>
															<select class="select">
																<option value="">选自省份</option>
															</select> 
															<select class="select">
																<option value="">选自地区</option>
															</select>
														</td>
													</tr>
													<tr>
														<td class="td-name">家乡 : </td>
														<td>
															<select class="select">
																<option value="">选自省份</option>
															</select> 
															<select class="select">
																<option value="">选自地区</option>
															</select>
														</td>
													</tr>
												</tbody>
											</table>
										
									</div><!--main-data end-->
								</div>
								<!--dialog_body end-->
							</div>
							<div class="dialog_buttons">
								<input type="submit" id="educationInfoSubmit" value="保存修改" tabindex="7" class="input-submit" name="submit" />
								<input type="button" value="取消" class="input-submit gray">
							</div>	
							</form>		
						</div>
					</td>
					<td class="pop_border"/>
				</tr>
				<tr>
					<td class="pop_bottomleft"/>
					<td class="pop_border"/>
					<td class="pop_bottomright"/>
				</tr>			
			</tbody>
		</table>
	</div>
</div><!--float end-->
								
