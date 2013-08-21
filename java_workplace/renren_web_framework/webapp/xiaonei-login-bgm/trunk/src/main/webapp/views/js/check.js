function check() {
	var userName = document.getElementById("_name");
	var mail = document.getElementById("_mail");
	var psw = document.getElementById("_password");
	var rePsw = document.getElementById("_repeatPassword");
	
	if (userName.value == ""){
		alert("请输入用户名");
		return false;
	}
	if (mail.value == "") {
		alert("请输入邮箱地址，往后的通知可能会发送到该邮箱");
		return false;
	} 
	if (psw.value == ""){
		alert("请输入密码");
		return false;
	} 
	if (rePsw.value == "") {
		alert("确认密码没有填");
		return false;
	}
	
	if (psw.value != rePsw.value) {
		alert("密码与确认密码不一致!");
		psw.value = "";
		rePsw.value = "";
		return false;
	}	
	return true;
}

function loginCheck() {
	var userName = document.getElementById("name");
	var psw = document.getElementById("password");
	if (userName.value==""){
		alert("请输入名字");
		return false;
	}
	if (psw.value=="") {
		alert("请输入密码");
		return false;
	}
	return true;
}

function isNewValue() {
	var oldValue = document.getElementById("id_oldValue");
	var newValue = document.getElementById("id_newValue");
	if (oldValue == newValue) {
		alert("the value isn't change, don't need be submited");
		return false;
	}
	return true;
}