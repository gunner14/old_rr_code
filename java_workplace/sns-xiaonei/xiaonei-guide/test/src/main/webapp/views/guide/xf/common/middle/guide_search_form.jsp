<%@ page pageEncoding="utf-8" %>
<div class="search-form all" id="search-con">																				
	<form id="guide-search" action="${applicationScope.urlBrowse}/searchEx.do" method="post">
		<input type="hidden" id='guidesearchref' name="ref" value="sg_guide_search_other_search"/>
		<select onchange="liandong(this);" id="tab_stud_item" style="display:none;">		
			<option value="tab_nuniv" selected="selected">大学</option>
			<option value="tab_hs">高中</option>
			<option value="tab_tech">中专</option>
			<option value="tab_junior">初中</option>
			<option value="tab_min">小学</option>
		</select>
		<select onchange="liandong(this);" id="tab_middstu_item" style="display:none;">		
			<option value="tab_hs" selected="selected">高中</option>
			<option value="tab_tech">中专</option>
			<option value="tab_junior">初中</option>
		</select>																				
		<span id="tab_nuniv_item" style="display:none;">
			<input type="text" id="nuniv" name="nuniv" class="input-text s-select s-univ" value="选择学校"/> 
			<input type="hidden" id='nuniv_id' name="adv_univId"/>
			<select id="univYear" name="univYear"><option value="">入学年份</option></select>
			<select id="nuniv_dept" name="department"><option value="">选择院系</option></select>			
		</span>
		<span id="tab_hs_item" style="display:none;">
			<input type="text" id="hs_name_0" name="hs_name_0"  class="input-text  s-select s-hight" value="选择学校"/> 
			<input type="hidden" id='hs_name_0_id' name="hs_id_0"/>
			<select id="hsyear" name="hsyear"><option value="">入学年份</option></select>
		</span>
		<span id="tab_junior_item" style="display:none;">
			<input type="text" id="juniorhighschoolname" name="juniorhighschoolname"  class="input-text  s-select s-junior" value="选择学校"/> 
			<input type="hidden" id='juniorhighschoolname_id'/>
			<select id="juniyear" name="juniyear"><option value="">入学年份</option></select>
		</span>
		<span id="tab_tech_item" style="display:none;">
			<input type="text" id="sectname" name="sectname" class="input-text  s-select s-tech" value="选择学校"/>
			<input type="hidden" id='sectname_id'/>
			<select id="sectyear" name="sectyear"><option value="">入学年份</option></select>
		</span>
		<span id="tab_min_item" style="display:none;">
			<input type="text" id="elementaryschoolname" name="elementaryschoolname" class="input-text" value="填写小学"/> 
			<input type="hidden" id='elementaryschoolname_id'/>
			<select id="elemyear" name="elemyearr"><option value="">入学年份</option></select>
		</span>
		<span id="tab_hometown_item" style="display:none;">
			<select  id="homePro" class="cur-plc" name="homeProvince"><option value="">选择省份</option></select>   
			<select id="homePro_city"  name="codeCity"><option value="">选择城市</option></select>
			<select  id="guide-sex" name="gend"><option value="">性别</option><option value="男生">男</option><option value="女生">女</option></select>
			<input type="hidden" id="homePro_city_code" name="homecitycode"/> 
			<input type="hidden" id="homePro_city_name" name="homecityname"/> 
		</span>
		<span id="tab_worker_item" style="display:none;">
			<input type="text" name="workName"  id="workName" class="input-text" value="填写公司名称"/>
		</span>
		<span id="tab_all_item" style="display:none">
			<input style="width:348px" type="text" class="input-text" id='guname' name="q" value="可输入姓名、学校、单位、城市等"/>															
			<input type="button" id="searchBTN" value="搜索" class="input-btn"/>
			<a class="advanced-search" href="${applicationScope.urlBrowse}/search.do?ref=guide_people_advance">高级搜索</a>
			<input type="hidden" value="0" name="s"/>	
			<input type="hidden" id='searchGQ' name="p"/>
		</span>	
		 <input type="hidden" value="0" name="ajax"/>
	</form>
	<form style="display:none;" action="${applicationScope.urlInvite}/inviteDispatch.do" method="post" id="tab_msn_item">
		<input type="hidden" name="ref" value="sg_guide_search_msn_search"/>
		<input type="hidden" name="url" value="${applicationScope.urlGuide}/guidexf.do"/>	
		<input type="hidden" name="quickurl" value="${applicationScope.urlGuide}/guidexf.do"/>	
		MSN帐号: <input type="text" class="input-text" name="uAccount"/>
		<input type="hidden" value="@hotmail.com" name="importer"/>
		密码: <input type="password" class="input-text" name="pwd"/>
		<input type="submit" name="submit" value="搜索" class="input-btn"/>
		<a class="advanced-search" href="${applicationScope.urlBrowse}/search.do?ref=guide_people_advance">高级搜索</a>
	</form>	
</div>