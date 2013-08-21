function replace(url){
    window.location.replace(url);
}
function search() {
	var ele = document.getElementById("id_search");
	var url = "?pattern=" + ele.value;
	replace(url);
}
function deleteUser(o) {
	var row = o.parentNode.parentNode;
	var eles = row.getElementsByTagName("td");
	var name = eles[0].firstChild.textContent;
	var url = "actionDelete?name=" + name;
	replace(url);
}

/*
function sendMail(o) {
	var row = o.parentNode.parentNode;
	var eles = row.getElementsByTagName("td");
	var mail = eles[2].firstChild.textContent;
	var url = "actionSendMail?mail=" + mail;
	replace(url);
}*/