<script>
	var redriectDef = new Array();
	redriectDef["http://reg.${applicationScope.sysdomain}/ac=ivate.do"] = "http://reg.${applicationScope.sysdomain}/activate.do";
	redriectDef["http://reg.${applicationScope.sysdomain}/agreement.html"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/alumnidistibute.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/alumniregister.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/collect.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/ctivate.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/ctivte.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/developerIndex.jsp"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/enren.com"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/getalbum.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/GetEntry.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/googlesearch.htm"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/gatfntry.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/list.html"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/Login.do"] = "http://passport.${applicationScope.sysdomain}/PLogin.do";
	redriectDef["http://reg.${applicationScope.sysdomain}/linreg.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/muchang"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/oms.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/PLogin.do"] = "http://passport.${applicationScope.sysdomain}/PLogin.do";
	redriectDef["http://reg.${applicationScope.sysdomain}/phone/add.jsp"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/profile.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/share/buttonshare.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/setpasswd.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/setpass.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/http"] = "http://${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/xnww.renren.com6212.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/wwxn5110.do"] = "http://reg.${applicationScope.sysdomain}/";
	redriectDef["http://reg.${applicationScope.sysdomain}/mobile-activate-mail.do"] = "http://guide.${applicationScope.sysdomain}/mobile-activate-mail.do";
	redriectDef["http://reg.${applicationScope.sysdomain}/initmsntabreg.do"] = "http://reg.${applicationScope.sysdomain}/";

	var regexsDef = new Array();
	regexsDef["http://reg\\.${applicationScope.sysdomain}/[\\w\\.-]+\\.((?!do$|html$|f$|jsp$|htm$).)*$"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.renren\\.com/kx\\d{1,}\\.do$"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/x?n?(\\d\\S*)*\\.do$"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/ac[tivate\\.;rc=]{4,10}\\w?\\.?do$"] = "http://reg.${applicationScope.sysdomain}/activate.do";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/([sS][yY][sS])?[hH]ome\\.do"] = "http://${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/[Rr]egister\\d{0,2}\\.do"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/(index|404|500)\\.(html|htm|php)"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/s-c-reg\\.do[^\\?]"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://reg\\.${applicationScope.sysdomain}/.*test999\.jsp"] = "http://reg.${applicationScope.sysdomain}/";
	regexsDef["http://guide\\.${applicationScope.sysdomain}/guide\\.do.+"] = "http://guide.${applicationScope.sysdomain}/guide.do";
	regexsDef["http://guide\\.${applicationScope.sysdomain}/home\\.do"] = "http://www.${applicationScope.sysdomain}/home.do";
	regexsDef["http://guide\\.${applicationScope.sysdomain}/preview\\.do"] = "http://guide.${applicationScope.sysdomain}/guide.do";
	regexsDef["http://guide\\.${applicationScope.sysdomain}/guide1\\.do"] = "http://guide.${applicationScope.sysdomain}/guide.do";
	regexsDef["http://guide\\.${applicationScope.sysdomain}/ajaxGuideBuddyReplyNotify\\.do"] = "http://guide.${applicationScope.sysdomain}/ajaxGuideBuddyReplyNotify";
	regexsDef["http://guide\\.${applicationScope.sysdomain}/ajaxGuideSetMemcacheWithExpiry\\.do"] = "http://guide.${applicationScope.sysdomain}/ajaxGuideSetMemcacheWithExpiry";

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
