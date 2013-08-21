<%@page import="com.renren.sns.friend.logic.HiddenFootprintLogic"%>
<%@page import="com.renren.sns.friend.model.HiddenFootprint"%>
<%@page import="com.xiaonei.xce.scorecache.ScoreCache.ScoreSignType"%>
<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_awardinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.xce.scorecache.DefaultScoreCache"%>
<%@page import="com.xiaonei.xce.scorecache.ScoreCacheAdapter"%>
<%@page import="com.renren.sns.guide.score.AwardDetail"%>
<%@page import="com.renren.sns.guide.dao.AwardDao"%>
<%@page import="java.util.Collections"%>
<%@page import="java.util.ArrayList"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Award Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    String award = request.getParameter("award");
    if(MyStringUtils.equals("on",award)){
    	//User host = BaseThreadUtil.currentHost(inv.getRequest());
        int userId = host.getId();
        boolean opened = "true".equals("" + request.getParameter("opened"));
        final DefaultScoreCache cache = ScoreCacheAdapter.getInstance().getScoreData(userId);
        DefaultScoreCache sc = ScoreCacheAdapter.getInstance().getScoreData(userId);
		out.println("level:"+sc.getLevel()+"<br />");
		out.println("lv TYPE_SUPER_EMOTION:"+sc.isSignTrue(ScoreSignType.TYPE_SUPER_EMOTION)+"<br />");
		out.println("lv TYPE_NO_FOOTPRINT:"+sc.isSignTrue(ScoreSignType.TYPE_NO_FOOTPRINT)+"<br />");
		out.println("lv TYPE_HOME_SKIN:"+sc.isSignTrue(ScoreSignType.TYPE_HOME_SKIN)+"<br />");
		out.println("lv TYPE_COLOURFUL_TALKBOX:"+sc.isSignTrue(ScoreSignType.TYPE_COLOURFUL_TALKBOX)+"<br />");
		
        request.setAttribute("scorecache",cache);
        out.println("days:"+cache.getContinueLoginDays()+" / "+cache.getHistoryLoginDays()+"<br />");
        out.println("score:"+cache.getScore()+"<br />");
        List<AwardDetail> details = AwardDao.getAwardDetails(userId, opened);
        if (details != null && details.size() > 0) {
            Collections.sort(details);
            Collections.reverse(details);
            for (AwardDetail d : details) {
                //格式化参数
                d.setLogDetail(String.format(d.getLogDetail(), new Object[] { "", "#" }));
            }
            // =============================================================
            //                   处理是否查看未处理奖励
            // =============================================================
            if ((!opened)) {
                List<AwardDetail> levelDetails = new ArrayList<AwardDetail>();
                List<AwardDetail> continueDaysDetails = new ArrayList<AwardDetail>();
                List<AwardDetail> historyDaysDetails = new ArrayList<AwardDetail>();
                for (AwardDetail d : details) {
                    switch (d.getType() / 100) {
                        case 3:
                            historyDaysDetails.add(d);
                            break;
                        case 2:
                            continueDaysDetails.add(d);
                            break;
                        case 1:
                            levelDetails.add(d);
                            break;
                        default:
                            break;
                    }
                }
                final int totalAwardCount = levelDetails.size() + continueDaysDetails.size() + historyDaysDetails.size();
                //                    if(cacheAwardCounts!=null) {//对于计数有误，以真实的技术为准
                //                        if(cacheAwardCounts[0] > levelDetails.size()) {
                //                            AwardDao.clearAwardCount(AwardCountType.LEVEL_AWARD, userId, levelDetails.size());
                //                        }
                //                        if(cacheAwardCounts[1] > continueDaysDetails.size()) {
                //                            AwardDao.clearAwardCount(AwardCountType.CONTINUE_DAYS_AWARD, userId, continueDaysDetails.size());
                //                        }
                //                        if(cacheAwardCounts[2] > historyDaysDetails.size()) {
                //                            AwardDao.clearAwardCount(AwardCountType.HISTORY_DAYS_AWARD, userId, historyDaysDetails.size());
                //                        }
                //                    }
                out.println("<hr />");
                if (levelDetails.size() > 0) {
                	request.setAttribute("levelDetails", levelDetails);
                	for(AwardDetail d : levelDetails){
                		out.println(d.getLogDetail()+"<br />");
                	}
                }
                out.println("<hr />");
                if (continueDaysDetails.size() > 0) {
                	request.setAttribute("contineDaysDetails", continueDaysDetails);
                	for(AwardDetail d : continueDaysDetails){
                		out.println(" - "+d.getLogDetail()+"<br />");
                	}
                }
                out.println("<hr />");
                if (historyDaysDetails.size() > 0) {
                	request.setAttribute("historyDaysDetails", historyDaysDetails);
                	for(AwardDetail d : historyDaysDetails){
                		out.println(" + "+d.getLogDetail()+"<br />");
                	}
                }
                out.println("<hr />");
                request.setAttribute("awardSize", totalAwardCount);
                if (totalAwardCount != cache.getAwardCount()) {
                    ScoreCacheAdapter.getInstance().setAwardCount(userId, totalAwardCount);
                }
            }
            request.setAttribute("details", details);//将所有类型的奖券加入Model
            out.println("<hr />");
            for(AwardDetail d :details){
            	out.println(d.getAwardKey()+" - "+d.getLogDetail()+" "+d.isOpened()+" "+d.getUpdateTime()+"<br />");
            }
            
            HiddenFootprint hiddenFootprint = HiddenFootprintLogic.getInstance().getHiddenFootprint(host.getId());
            List<Integer> hps = hiddenFootprint.list();
            for(int hpid :hps){
            	out.println("hide:"+hpid+"<br />");
            }
        }
    }
    %>
    
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
</div>
<!-- {------------ --> 
<%
}
%>