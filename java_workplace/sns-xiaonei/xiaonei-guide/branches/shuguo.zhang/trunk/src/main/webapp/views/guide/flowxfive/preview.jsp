<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!-- _pv_ -->
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.1.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<script src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$" type="text/javascript"></script>
	<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.temp-v1.1.js?ver=$revxxx$" type="text/javascript"></script>
	<title>${domainIdentified["siteName_fillinfo"]} - 选择你认识的人加为好友</title>
	
	<script type="text/javascript">		
		XN.DOM.readyDo(function(){
			<c:forEach var="item" items="${infolist}">
			ajaxGetFriends('${item.type}');
			</c:forEach>
		});

        <%--//guide 统计--%>
        var guideStats = {
            send : function( str ){
                var img = new Image();           
                img.src = 'http://guide.' + XN.env.domain + '/xtrs?nur=' + str + '&t=' + Math.random();
            }    
        }

        <%--//统计相关--%>
        XN.dom.ready(function(){
            <%--//新的统计--%>
            guideStats.send('rggp2v0a0');

            setTimeout(function(){
                var links = document.getElementsByTagName('a');
                XN.array.each(links , function(i,v){
                    <%--//加为好友统计--%>
                    if( v.innerHTML == '加为好友' ) { 
                         XN.event.addEvent(v,'click', function(){
                             guideStats.send('rggp2v0a1')
                         }); 
                     }
                     <%--//显示更多--%>
                    if( v.innerHTML == '显示更多' ) { 
                         XN.event.addEvent(v,'click', function(){
                             guideStats.send('rggp2v0a2')
                         }); 
                     }
                });
            },2000);

            var saveBtns = XN.dom.getElementsByClassName('input-submit'); 
            XN.array.each(saveBtns, function(i,v){
                <%--//保存并查找--%>
                if( v.value == '保存并查找' ) {
                     XN.event.addEvent(v,'click', function(){
                         guideStats.send('rggp2v0a3')
                     }); 
                }        

                <%--//下一步--%>
                if( v.value == '下一步' ) {
                     XN.event.addEvent(v,'click', function(){
                         guideStats.send('rggp2v0a4')
                     }); 
                }
            })
        })
	</script>
</head>
<body id="home" class="guide">
		<%@ include file="/inc/header-guide-flow.inc" %>
	<div id="content">
		<div class="user-data">
			<div class="title">
				<h1>有认识的人吗？加TA为好友吧</h1>
			</div>
			<form id="formResult" action="/previews" method="post" onsubmit="return false">
			<c:forEach var="item" items="${infolist}">
			<div id="chooseFriends_${item.type}" class="friend-choose">
				<h2><strong>${item.displayname}</strong><a href="javascript:;" class="more">显示更多</a></h2>
				<ul class="clearfix">
					<li class="loading">
						<img src="${applicationScope.urlSmallStatic}/imgpro/indicator/loading.gif" />
					</li>
				</ul>
				<!-- <input type="hidden" id="${item.type}_year" name="" value="${item.infoyear}" />  -->
				<input type="hidden" name="${item.type}" value="0" />
			</div>
			</c:forEach>
			</form>
			<c:if test="${formtype != 0}">
				<div class="fill-data" id="fillData" >
					<div class="main-data">
						<h2>更多信息，会找到更多好友</h2>					
						<c:choose>
							<%--大学--%>
							<c:when test="${formtype == 1}">
								<form method="post" action="preview-s-dx" autoComplete="off" onsubmit="return false;" id="fdaxue" >
									<table>
										<tbody>
										<tr>
											<td class="td-name">我的初中 : </td>
											<td>
											<input type="text" class="input-text s-select s-junior" id="juniorSchool" name="PFDaxue.juniorhighschoolname" /> 
											<select id="juniorScoolYear" class="select" name="PFDaxue.juniorhighschoolyear"><option value="">入学年份</option></select>
											<input id="juniorSchool_id" type="hidden" name="PFDaxue.juniorhighschoolcode"  value="-1" />
											</td>
											
										</tr>
										<tr>
											<td class="td-name">我的小学 : </td>
											<td><input type="text" class="input-text" id="elementarySchool"  name="PFDaxue.elementaryschoolname" /> 
											<select id="primaryScoolYear" class="select" name="PFDaxue.elementaryschoolyear"><option value="">入学年份</option>
											</select>
											<input id="elementaryschool_id" type="hidden" name="PFDaxue.elementaryschoolcode"  value="-1" />
											</td>
										</tr>
										</tbody>
									</table>
									<div class="save-box"><input type="submit" value="保存并查找" class="input-submit gray" /></div>
								</form>
							</c:when>
							<%--高中--%>
							<c:when test="${formtype == 2}">								
								<form method="post" action="preview-s-gz" autoComplete="off" onsubmit="return false;" id="fgaozhong" >
									<table>
										<tbody>
										<tr>
											<td class="td-name">我的小学 : </td>
											<td><input type="text" class="input-text" id="elementarySchool" name="PFGaozhong.elementaryschoolname" /> 
											<select id="primaryScoolYear" class="select" name="PFGaozhong.elementaryschoolyear" ><option value="">入学年份</option>
											</select>
											<input id="elementaryschool_id" type="hidden" name="PFGaozhong.elementaryschoolcode"  value="-1" />
											</td>
										</tr>
										</tbody>
									</table>
									<div class="save-box"><input type="submit" value="保存并查找" class="input-submit gray" /></div>
								</form>								
							</c:when>
							<%--已工作--%>
							<c:when test="${formtype == 3}">
								<form method="post" action="preview-s-bl" autoComplete="off" onsubmit="return false;"  id="fgongzuo" >
									<table>
										<tbody>
										<tr>
											<td class="td-name" valign="top">大学 : </td>
											<td>
												<table>
													<tr>
														<td>
															<input id="university" type="text" class="input-text s-select s-univ" name="PFBailing.universityname" />
															<select id="universityYear" class="select" name="PFBailing.universityyear">
																<option value="">入学年份</option>
															</select>
															<input id="university_id" type="hidden" name="PFBailing.universitycode" />
														</td>
													</tr>
													<tr>
														<td>
															<select id="university_dept" class="select" name="PFBailing.universitydepartment" >
																<option value="">选择院系</option>
															</select>
															<select class="select" name="PFBailing.coursetype">
																<option value="">请选择类型</option>
																<option value="大学生">大学生</option>
																<option value="硕士">硕士</option>
																<option value="博士">博士</option>
															</select>
														</td>
													</tr>				
												</table>
											</td>
										</tr>
										<tr>
											<td class="td-name">高中 : </td>
											<td>
											<input id="highSchool" type="text" class="input-text s-select s-hight" name="PFBailing.highschoolname"/>
											<select id="highScoolYear" class="select"  name="PFBailing.highschoolyear" ><option value="">入学年份</option></select>
											<input id="highSchool_id" type="hidden" name="PFBailing.highschoolcode" />
											</td>
										</tr>
										</tbody>
									</table>
									<div class="save-box"><input type="submit" value="保存并查找" class="input-submit gray" /></div>
								</form>								
							</c:when>
							<c:when test="${formtype == 4}">
								<form method="post" action="preview-s-qt" autoComplete="off" onsubmit="return false;" id="fqita" >
									<table>
										<tbody>
										<tr>
											<td class="td-name">高中 : </td>
											<td>
											<input id="highSchool" type="text" class="input-text s-select s-hight" name="PFQita.highschoolname" />
											<select id="highScoolYear" class="select" name="PFQita.highschoolyear" ><option value="">入学年份</option></select>
											<input id="highSchool_id" type="hidden" name="PFQita.highschoolcode" />
											</td>
										</tr>
										<tr>
											<td class="td-name">我的初中 : </td>
											<td>
											<input type="text" class="input-text s-select s-junior" id="juniorSchool" name="PFQita.juniorhighschoolname" /> 
											<select id="juniorScoolYear" class="select" name="PFQita.juniorhighschoolyear"><option value="">入学年份</option></select>
											<input id="juniorSchool_id" type="hidden" name="PFQita.juniorhighschoolcode" value="-1" />
											</td>
										</tr>

										</tbody>
									</table>
									<div class="save-box"><input type="submit" value="保存并查找" class="input-submit gray" /></div>
								</form>
							</c:when>
						</c:choose>						
						
					</div>
				</div>				
			</c:if>		
			<div class="btn-box"><input type="submit" value="下一步" class="input-submit" id="nextStep" onfocus="this.blur()" />
			</div>
		</div>
	</div>
<%@ include file="/inc/footer-guide-flow.inc" %>
</body>
</html>
