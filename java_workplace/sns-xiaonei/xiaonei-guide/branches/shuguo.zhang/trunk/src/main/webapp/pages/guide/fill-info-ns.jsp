<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head.inc"%>
		<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/DD_roundies_0.0.2a-min.js?ver=$revxxx$"></script>
		<style type="text/css">
		p.label {
			margin-bottom: 10px;
		}
		
		.specail {
			color: #005EAC;
		}
		</style>

		<script type="text/javascript">
			XN.getFileVersion(
				[
					'http://s.xnimg.cn/csspro/apps/profile.css?ver=$revxxx$',
					'http://s.xnimg.cn/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
					'http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$',
					'http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
					'http://s.xnimg.cn/csspro/module/friendSelector.css?ver=$revxxx$'
			]);
			Array.prototype.contains = function(item, from) {
				return this.indexOf(item, from) != -1;
			};
			DD_roundies.addRule('.msn-account-info .inputtext, .msn-account-info .inputselect', '5px');
			DD_roundies.addRule('div.identities ul.identities li', '5px');
		</script>
		<script>
            function chooseHSchool(typeName, typeCode){
				//alert(typeCode);
				switch (typeCode){
					case 0:{
						$('schoolType').value='2';
						SchoolComponent.show(SchoolComponent.HIGH_SCHOOL,'highSchoolName','highSchoolCode');
						break;
					}
					case 1:{
						$('schoolType').value='3';
						SchoolComponent.show(SchoolComponent.JUNIOR_SCHOOL,'juniorHighSchoolName','juniorHighSchoolId');
						break;
					}
					case 2:{
						$('schoolType').value='4';
						SchoolComponent.show(SchoolComponent.COLLEGE_SCHOOL,'techHighSchoolName','collegeId');
						break;
					}
				}
            }
        </script>


		<script type="text/javascript" src="http://s.xnimg.cn/js/yui/yahoo/yahoo-min.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/yui/event/event-min.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/schoolComponent_guide.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/fillinfo.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/inCityArray.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/cityArray.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/register/guide_fillinfo_shortform_guide.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/universityDepartmentComponent.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/provinceCityComponent.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/js/basicInfo.js?ver=$revxxx$"></script>
		<title>${domainIdentified["siteName_fillinfo"]}</title>
	</head>
	<body id="pageFriends" class="msninvite-bg" onclick="university_onclick()">
		<%@ include file="/inc/allunivlist.inc" %>
		<%@ include file="/inc/allschoollist.inc" %>
		<div id="container" >
			<%@ include file="/inc/header-guide-flow.inc"%> 
			<div class="full-page-holder">
				<div class="full-page clearfix guide-msninvite">
					<div class="compatible">
						<div id="content">
							<div class="guide-invite-steps clearfix">
								<ul class="current1">
									<li class="current">
										<h4>第1步:</h4>
										<p>找到我的朋友</p>
									</li>
									<li>
										<h4>第2步:</h4>
										<p>上传头像</p>
									</li>
								</ul>
							</div>		
							<div class="invite_box">
								<div class="identities cube">
									<%@ include file="/inc/errors.inc" %>
									<span id="workingInfo" style="">
										<html:form styleId="fill_info" action="/fill-info-ns.do" styleClass="info" >
										<html:hidden property="porId" styleId="porId" />
										<html:hidden property="tinyUrl" styleId="tinyUrl" />
										<html:hidden property="headUrl" styleId="headUrl" />
										<html:hidden property="mainUrl" styleId="mainUrl" />
										<html:hidden property="largeUrl" styleId="largeUrl" />
										<html:hidden property="isskip" value="false" styleId="skip" />
										<html:hidden property="homecityName" styleId="homecityName" />
										
										<p id="p_highSchool">
											<label>高中:</label>
											
											<span id="hs" style="display: inline;">
												<html:text property="highSchoolName" readonly="true" styleId="highSchoolName" onclick="chooseHSchool('hs',0);" styleClass="inputtext" />
												<html:hidden property="highSchoolCode" styleId="highSchoolCode"/>
											</span>
											<html:select property="highschoolyear" styleId="highschoolyear" styleClass="inputselect"><html:option value="">选择入学年</html:option>
											<c:set var="strutsOptionYearComponent_begin" value="2009" />
											<c:set var="strutsOptionYearComponent_end" value="${user_age}" />
											<%@ include file="/inc/strutsOptionYearComponent.inc"%>
											</html:select>
											
											<span id="other-sch"><span><!--<a id="j-sch" class="other-sch">  初中</a></span><span>/</span><span><a id="t-sch" class="other-sch">中专技校</a> --></span></span>
                                                
												<script type="text/javascript">
												/*
                                                    var schs = [];
                                                    schs.push($('j-sch'));
                                                    schs.push($('t-sch'));
                                                    
                                                    for (var kk = 0; kk < schs.length; kk++) {
                                                    
                                                        schs[kk].onclick = function(){
															
                                                            switch(this.id){
																case 'j-sch':{
																	$('p_jourSchool').style.display = 'block';
																	break;
																}
																case 't-sch':{
																	$('p_techSchool').style.display = 'block';
																	break;
																}
															}
															
															
                                                        }
                                                        
                                                    }
                                                    */
                                                </script>
                      <html:hidden property="schoolType" styleId="schoolType"/>
											
										</p>
										<p id="p_univ">
											<label>大学:</label> 
											<html:text  property="universityname" onclick="showpan();bodyclick=false;" readonly="true" styleId="universityname" styleClass="inputtext" />
											<html:hidden property="universityId" styleId="universityid"/>
											<html:select property="universityyear" onchange="setHsYears()" styleId="universityyear" styleClass="inputselect"><html:option value="">选择入学年</html:option>
											<c:set var="strutsOptionYearComponent_begin" value="2009" />
											<c:set var="strutsOptionYearComponent_end" value="${user_age}" />
											<%@ include file="/inc/strutsOptionYearComponent.inc" %>
											</html:select>
											<span id="univDeptComponent_Span">
												<html:select property="department" styleId="department" styleClass="inputselect">
													<html:option value="">选择院系</html:option>
												</html:select>
											</span>
											</p>
											<p>
												<label>公司:</label>
												<html:text  property="company" styleId="company" styleClass="inputtext" />
											</p>
											<div id="company_sug" class="posrela"></div>
											<br />
											<c:if test="${!age_less_17}">
												<p>
													<label>当前所在地:</label>
													<html:select property="homeprovince" styleId="homeprovince" styleClass="inputselect" onchange="Profile.getInCityListOfProvince(this.selectedIndex, 'homecityCode');">
														<%@ include file="/pages/profile/inProvince.jsp"%>
													</html:select>
													<html:select property="homecityCode" styleId="homecityCode" styleClass="inputselect" style="display:none;" onchange="Profile.selectCity(this.selectedIndex, 'homecityCode', 'homecityName');" >
														<html:option value="">城市</html:option>
													</html:select>
													<select style="display: none;" onblur="Profile.selectFcity(this.selectedIndex, 'fcitycode', 'fcityname')" tabindex="4" class="select" name="fcitycode" id="fcitycode">
													<input type='hidden' Id="fcityname" value="" />
													
												</p>
											</c:if>
											<p class="addingfri">
												<a id="skip_next" class="float-right" href="?skip=1" >跳过</a>
												<input type="submit" id="add_friends" class="btn-5" value="提交信息"/>						
											</p>
										</html:form>
									</span>
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
		<c:set var="hideFooterLink" value="true" />
    <c:set var="noWebPager" value="true" />
		<%@ include file="/inc/footer-noad.inc" %>
		<!-- 将部分脚本移到页面底部,提高页面性能(auscar) -->
  <script type="text/javascript" src="http://s.xnimg.cn/js/cascade.js?ver=$revxxx$"></script>
  <script type="text/javascript" src="http://s.xnimg.cn/js/register/guide_fillinfo_shortform_guide.js?ver=$revxxx$"></script>
  <script>
	
	 /* auscar compnnent starts here */

	 /*
	var js = ('juniorHighSchoolName');
	var jsCas = new Casc(js,['juniorHighSchoolName','juniorschoolyear']);
	
	var ts = ('juniorHighSchoolName');
	var tsCas = new Casc(ts,['techHighSchoolName','techschoolyear']);
	*/
   
    var ms = $('highSchoolName');//'ms' is short for 'middle school'
    var midSchCas = new Casc(ms, ['highSchoolName', 'highschoolyear']);
    
    
    var un = $('universityname');//'un' is short for 'university name'
    var schoolCas = new Casc(un, ['universityname', 'universityyear', 'univDeptComponent_Span'], 'onchange');
    
    //   var curZone = $('homeprovince');
    //   var zoneCas = new Casc(curZone,['homeprovince','homecityCode']); 
    
    //in order to be fast,  i do it in an ugly way.
    var homeCode = $('homecityCode');
    if(homeCode){
	    homeCode.style.display = 'none';
    }
    
    //让我们来hack一下SchoolComponent.cl_school方法
	
	var old_cl_school = SchoolComponent.cl_school;
	SchoolComponent.cl_school = function(event,schNum){
		old_cl_school.call(SchoolComponent, event, schNum);
		var offsetYear = 3;
		
		function whoHasValue(){
			
			return $('highschoolyear').value? $('highschoolyear') : 
			 
			$('universityyear').value ? $('universityyear') : null;
		}
		
		//alert(whoHasValue().id);

		var who;
		switch(this.type){
			case 'JUNIORSCHOOL' : {
				var j_t = $('juniorschoolyear');
				j_t.style.display = 'inline';
				who = whoHasValue();
				if(who.id == 'universityyear'){
					offsetYear = 6;
				}
				SchoolComponent.selectByText(j_t, (who.value - 0 - offsetYear)+'');
				
				break;
			}
			case 'COLLEGESCHOOL' : {
				var t_t = $('techschoolyear');
				t_t.style.display = 'inline';
				
				who = whoHasValue();
				if(who.id == 'highschoolyear'){
					offsetYear = 0;
				}
				SchoolComponent.selectByText(t_t, (who.value - 0 - offsetYear)+'');
				break;
			}
			case 'HIGHSCHOOL' : {
				$('highschoolyear').style.display = 'inline';
				break;
			}
		}
		
		return false;
	}
	
	
	
	//cut from guide_fillinfo_shortform.js(auscar)
    function cl(id){
        $("universityid").value = id;
        
        var t = $("universityname");
        t.value = $("sch_" + id).innerHTML;
        
        hidepan();
        
        if (t.value != orginText) {
            if (schoolCas) 
                schoolCas.showAll();
        }
        
        UnivDeptComponent.fillUniversityDept(id, '');//maybe ajax request here(commented by auscar)
    }
	
	try
	{
		XN.FORM.userInfoAutoComplete( 'company','workplace');

		ProfileInfo.region.init('homeprovince','homecityCode','fcitycode');
		ProfileInfo.region.getCityList();

	}catch(e){}
</script>
  <script>
  function initData(){
      //init 
		$('hs').style.display='inline';
		/*
		$('js').style.display='inline';
		$('ts').style.display='inline';
		$('p_jourSchool').style.display='none';
		$('p_techSchool').style.display='none';
		*/
   }
	initData();	
  </script>
    <script type="text/javascript" src="http://s.xnimg.cn/js/formChecker.js?ver=$revxxx$"></script>
  
	</body>
</html>
