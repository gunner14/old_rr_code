function modifyReplace(o) {
	var row = o.parentNode.parentNode;
	var eles = row.getElementsByTagName("td");
	var id = eles[0].firstChild.textContent;
	if (eles[1].firstChild == null) {
		alert("configKey为空，系统错误..抱歉");
		return false;
	}
	if (eles[2].firstChild == null) {
		alert("configKey为空，系统错误..抱歉");
		return false;
	}
	var configKey = eles[1].firstChild.textContent;
	var configValue = eles[2].firstChild.textContent;
	var currentPage = document.getElementById("Page").attributes.currentPages;
	
	var content = "<td colspan='6'>" +
				  "<form action='actionModify' method='post'>" +
				  "编辑：                            id : " + id + ";   configKey : " + configKey + ";   configValue : " +
				  	 "<input type='hidden' name='currentPage' value='" + currentPage + "'/>" + 
				  	 "<input type='hidden' name='configKey' value='" + configKey + "'/>" +
				  	 "<input id='oldValue' type='hidden' name='oldValue' value='" + configValue + "'/>" +
				  	 "<input id='newValue' type='text' name='newValue' value='" + configValue + "'/>" +
				     "<input type='submit' value='提交' onclick='return checkModify();'/>" +
				     "</form>" +
				  "</td>";
	normalReplace(row, content);
}

function addReplace(o) {
	var row = o.parentNode.parentNode;
	var content = "<td colspan='6'>" +
				  "<form action='actionAdd' method='post'>" +
				  		"configKey:<input id='add_key' type='text' name='newKey'/>" +
				  		"configValue:<input id='add_value' type='text' name='newValue'>" +
				  		"&nbsp;&nbsp;<input type='submit' value='提交' onclick='return checkValue(this);'>" +
				  "</form>" +
				  "</td>";
	normalReplace(row, content);
}

var replace = {flag:false, sourceNode:"", sourceHTML:""};

function normalReplace(o, content) {
	if(replace.flag != false) {
		console.log("111");
		replace.sourceNode.innerHTML = replace.sourceHTML;
		console.log(replace.sourceNode.innerHTML);
	}
	replace.flag = true;
	replace.sourceNode = o;
	replace.sourceHTML = o.innerHTML;
	o.innerHTML = content;
}

function reset() {
	replace.flag = false;
	replace.sourceNode.innerHTML = replace.sourceHTML;
	
}

function checkValue(o) {
	var key = document.getElementById("add_key");
	var value = document.getElementById("add_value");
	if (isEmpty(key.value)) {
		alert("configKey不能为空");
		return false;
	}
	if (isEmpty(value.value)) {
		alert("configValue不能为空");
		return false;
	}
	return true;
}

function checkModify() {
	var oldValue = document.getElementById("oldValue").value;
	var newValue = document.getElementById("newValue").value;
	if (isEmpty(newValue) == false) {
		if (newValue == oldValue) {
			alert("值没有修改");
			return false;
		}
		return true;
	}
	alert("空值，不能提交");
	return false;
}

function isEmpty(str) {
	var s = str;
	var i = 0;
	for(i=0; i<s.length; i++) {
		if(s[i] != " ")
			return false;
	}
	return true;
}