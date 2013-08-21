<%@ page contentType="text/html;charset=UTF-8" session="false" %>

<section data-type="univ">
	<dl class="f-s">
		<dt><label for="">大学：</label></dt>
		<dd>
			<input class="inputtext s-select s-univ" id="nuniv" data-type="name" data-tip="请填写大学信息" />
		</dd>
	</dl>
	<dl>
		<dt><label for="">入学年份：</label></dt>
		<dd>
			<select class="y-comp" data-type="year" data-tip="请填写入学年份">
				<option>--</option>
			</select>
		</dd>
	</dl>
	<dl>
		<dt><label for="">类型：</label></dt>
		<dd>
			<select data-tip="请填写学位信息">
				<option>请选择类型</option>
				<option>大学生</option>
				<option>硕士</option>
				<option>博士</option>
				<option>其他</option>
			</select>
		</dd>
	</dl>
	<dl>
		<dt><label for="">院系：</label></dt>
		<dd>
			<select id="nuniv_dept" data-type="depart" data-tip="请填写院系信息"></select>
		</dd>
	</dl>
</section>