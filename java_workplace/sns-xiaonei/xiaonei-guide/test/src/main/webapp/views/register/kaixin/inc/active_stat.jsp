<c:if test="${isActivated == '1' && user.stage == 30  && !empty from && from != 'null' && from != ''}">
	<c:if test="${from == '18' || from == '21'}">
    	<img src="http://www.chanet.com.cn/action.cgi?t=1609&i=${user.id}" width="0" height="0">
    </c:if>
	<c:if test="${from == '29'}">
        <img src="http://61.152.242.247/sm/tb/xiaonei/getxn.php?uid=${user.id}&ayid=${ayid}&flag=1" width="0" height="0">
    </c:if>
    <c:if test="${from == '30'}">
		<iframe WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' SRC='http://www.pceggs.com/AdSolution/track/?PCADID=6texbwNQCTnWB%2ftRnRuCEQ%3d%3d&MerID=${user.id}'></IFRAME>
	</c:if>
	<c:if test="${from == '33'}">
		<img src="http://di.51cpm.net/pushcpa.php?user=xiaonei&time=<%=new java.util.Date().getTime()/1000%>&src=${ayid}&uid=${user.id}" width="0" height="0" />
	</c:if>
	<c:if test="${from == '34'}">
    	<img src="http://www.100tone.com/external/xiaonei_tijiao.jsp?tone=${ayid}&userid=${user.id}" width="0" height="0" />
    </c:if>
	<c:if test="${from == '35'}">
		<script type="text/Javascript">
        var alimm_cid="11073610_10000142";
        var alimm_ts="24";
        </script>
        <script type="text/Javascript" src="http://stats.alimama.com/etev.js"></script>
    </c:if>
	<c:if test="${from == '39'}">
		<iframe SRC="http://www.sinoads.com.cn/joint/xiaonei.htm?uid=${user.id}&a_time=<%=new java.util.Date().getTime()/1000%>&callback=${ayid}" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></IFRAME>
	</c:if>
	<c:if test="${from == '41'}">
		<iframe SRC="http://www.heima8.com/pc/cklog.htm?assign=1eb2be144e68592b5a8bb3ff4855e49b&date=<%=new java.util.Date().getTime()/1000%>&from=41&ayid=${ayid}&uid=${user.id}" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></IFRAME>
	</c:if>
	<c:if test="${from == '43'}">
		<iframe SRC="http://adtaste.pceggs.com/ADComate/PgXiaoNeiAnti.aspx?uid=${user.id}&a_time=<%=new java.util.Date().getTime()/1000%>&callback=${ayid}" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></IFRAME>
	</c:if>
	<c:if test="${from == '46'}">
		<iframe SRC="http://tc.longtailcn.com/xiaoneireg.html?uid=${user.id}&source=${ayid}" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></IFRAME>
	</c:if>
	<c:if test="${from == '47'}">
		<iframe src="http://www.doudou.com/ete.php?adID=60&annalID=${ayid}&idCode=${user.id}" width=0 height=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></iframe>
	</c:if>
	<c:if test="${from == '49'}">
		<iframe SRC="http://a5.myiee.com/xiaonei/xiaonei.php?uid=${user.id}&ayid=${ayid}&date=<%=new java.text.SimpleDateFormat("yyyy-MM-dd").format(new java.util.Date())%>" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></iframe>
	</c:if>
	<c:if test="${from == 'reg_sanguo'}">
		<iframe SRC="http://www.chanet.com.cn/action.cgi?t=1928&i=${user.id}" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></IFRAME>
	</c:if>
	<c:if test="${from == 'reg_maoyou'}">
		<iframe SRC="http://www.chanet.com.cn/action.cgi?t=1932&i=${user.id}" WIDTH=0 HEIGHT=0 MARGINWIDTH=0 MARGINHEIGHT=0 HSPACE=0 VSPACE=0 FRAMEBORDER=0 SCROLLING=no BORDERCOLOR='#000000' ></IFRAME>
	</c:if>
</c:if>
<c:if test="${isActivated == '1'}">

</c:if>