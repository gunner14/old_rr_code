<script>
	var redriectDef = new Array();
	redriectDef["http://reg.${applicationScope.sysdomain}/ac=ivate.do"] = "http://reg.${applicationScope.sysdomain}/activate.do";
	redriectDef["http://guide.${applicationScope.sysdomain}/;"] = "http://guide.${applicationScope.sysdomain}/guide";

	var regexsDef = new Array();
	regexsDef["http://reg\\.${applicationScope.sysdomain}/[\\w\\.-]+\\.((?!do$|html$|f$|jsp$|htm$).)*$"] = "http://reg.${applicationScope.sysdomain}/";

	var	thisUrl = new String(location.href).split("?");
	if (undefined != redriectDef[thisUrl[0]]) {
		if (thisUrl.length > 1) {
			location.href = redriectDef[thisUrl[0]] + "?" + thisUrl[1];
		} else {
			location.href = redriectDef[thisUrl[0]];
		}
	}
	for ( var regex in regexsDef) {
		if (regex == "") {
			continue;
		}
		if (new RegExp(regex).test(thisUrl[0])) {
			if (thisUrl.length > 1) {
				location.href = regexsDef[regex] + "?" + thisUrl[1];
			} else {
				location.href = regexsDef[regex];
			}
		}
	}
</script>
