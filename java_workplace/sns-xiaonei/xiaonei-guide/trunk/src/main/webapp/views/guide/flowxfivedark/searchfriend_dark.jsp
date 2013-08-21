<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head><!-- _sf_dark_ -->
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$" media="all" type="text/css" rel="stylesheet" /> 
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.4.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<title>${domainIdentified["siteName_fillinfo"]} - 先来找找你的介绍人吧</title>

</head>
<body id="home" class="guide Infor">
		<%@ include file="/inc/header-guide-flow.inc" %>
        <script type="text/javascript">
	    	try{
		var recommandFriends =  XN.json.parse('${requestScope.lead_search_friend}');
		}catch(e){}
        </script>
	<div id="content">
    <div class="user-data clearfix">

        <div class="title">
            <h1>先来找找你的介绍人吧</h1>
        </div><!--title end-->
        <div class="nav-tab">
            <div class="friend-choose">     
	    	<div id="noFriendCon" style="display:none;">没有符合筛选条件的搜索结果，<a href="javascript:void(0);" onclick="reSetForm(event);">换个条件试试</a>?</div>
	    	<div id="noSearch" style="display:none;">没有填写搜索条件</div>
                <ul class="clearfix" id="search_con">
                </ul>
            </div><!--friend-choose end-->
        </div><!--nav-tab end-->
        <div class="friend-box" style="display:none">
            <h1>根据个人信息找到你的好友</h1>
            <ul class="clearfix">
                <li>

                    <p><img src="${applicationScope.urlStatic}/imgpro/avatar/d_default_50.gif" width="50" height="50"/></p>
                    <span>老同学</span>
                    <span>加为好友</span>
                </li>
                <li>
                    <p><img src="${applicationScope.urlStatic}/imgpro/avatar/d_default_girl_50.gif" width="50" height="50"/></p>
                    <span>老同学</span>

                    <span>加为好友</span>
                </li>
                <li>
                    <p><img src="${applicationScope.urlStatic}/imgpro/avatar/d_default_50.gif" width="50" height="50"/></p>
                    <span>老同学</span>
                    <span>加为好友</span>
                </li>

            </ul>
        </div><!--friend-box end-->
        <div class="fill-data">
            <div class="fill-data-box">
                <div class="main-data secondary" id="mid_high">
                    <form id="searchForm" action="sfbrown-s-r" method="post">
                    <div class="search-reg">输入姓名 : <input id="targetName_id" type="text" class="input-text" name="targetname" value="${requestScope.lead_target_name}" /></div>
                    <div id="search-tips" style="display:none;" class="search-tips">搜索结果超过100个，建议你添加一些筛选条件。</div>

                        <table>
                            <tr>
                                <td class="td-name">大学 : </td>
                                <td>
                                    <input type="text" class="input-text s-select s-univ" name="universityname" id="nuniv"/> 
                                    <input type="hidden" id="nuniv_id" value="0" name="universitycode"/>
                                    <select class="select" id="nuniv_year" name="universityyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2012" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                                
                            </tr>
                            <tr>
                                <td class="td-name" valign="top">高中 : </td>
                                <td>
                                    <input type="text" class="input-text s-select s-hight" name="highschoolname" id="mid_high"/> 
                                    <input type="hidden" id="mid_high_id" name="highschoolcode"/>
                                    <select class="select"  id="mid_high_year" name="highschoolyear"><option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2012" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>           
                            <tr>
                                <td class="td-name">中专技校 : </td>
                                <td>
                                    <input type="text" class="input-text s-select s-tech" name="techschoolname" id="mid_tech_tech"/> 
                                    <input type="hidden" id="mid_tech_tech_id" value="0" name="techschoolcode"/>
                                    <select class="select" id="mid_tech_tech_year" name="techschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2012" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
				<tr>
                                <td class="td-name">初中 : </td>
                                <td>
                                    <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname"  id="mid_high_junior"/>
                                    <input type="hidden" id="mid_high_junior_id"" name="juniorhighschoolcode"/>
                                    <select class="select" id="mid_high_junior_year" name="juniorhighschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2012" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
			<tr>
			<td class="td-name">小学 : </td>
			 <td>
			 <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
			   <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
			   <select class="select" id="mid_entary_year" name="elementaryschoolyear">
			             <option value="">入学年份</option>
				     <c:set var="strutsOptionYearComponent_begin" value="2012" />
				     <c:set var="strutsOptionYearComponent_end" value="1900" />
				     <%@ include file="inc/years.inc" %>
			</select>
			 </td>
			</tr>
			    <tr>
			     <td class="td-name"> 单位 : </td>
			     <td>
		            <input type="text" class="input-text" id="work" name="companyname" />
		            </td>
			    </tr>

                        </table>
                    <div class="save-box">
                        <input type="button" id="search_submit_btn"  value="搜索好友" class="input-submit" />
				<a href="javascript:void(0);" onclick="reSetForm()">清空条件</a>
                    </div>
		    </form>
                </div><!--main-data secondary end-->
            </div><!--fill-data-box end-->
        </div><!--fill-data end-->
        <div class="btn-box" id="btn-box" style="display:block">
            <form action="sfbrown-s" id="next-step-form">
                <div style="display:none" id="friends-count-filedset"> </div>
                <input type="submit" value="下一步" class="input-next" onclick="guideStats.send('rggp0v0a5');" onfocus="this.blur()" id="nextStep"/>
            </form>
	    <div id="showScroll"></div>
        </div>
    </div><!--user-data end-->
</div><!--content end-->
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script> 
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.searchFriend.js?ver=$revxxx$"></script> 
<%@ include file="/inc/footer-guide-flow.inc" %>
<img width="0" height="0" src="http://guide.${domainIdentified["sys.domain"]}/xtrs?nur=rggp0v0a0<%= (new java.util.Random()).nextFloat()%>"/>
</body>
</html>
