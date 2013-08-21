package com.xiaonei.reg.guide.flows.xfive.utils;

import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.model.FriendApply;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideRemoteLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class GuideXFiveSupportFriendsUtil {
	private static void beFriends(final User host,final String [] ids,final String currentDomain,String reasonflag){
		if(host==null)return;
		GuideTimeCost cost = GuideTimeCost.begin();
		cost.endStep();
		StringBuilder sb = new StringBuilder();
		if (ids != null && ids.length > 0) {// 分发多个请求
			GuideLogger.printLog(" host:"+host.getId()+" request count:"+ ids.length);
			for (int i = 0; i < ids.length; i++) {
				cost.endStep();
				int accepterId = 0;
				try {
					accepterId = Integer.parseInt(ids[i]);
				} catch (NumberFormatException e) {
					//BaseActionUtil.addMsg(request, "您没有向任何人发出邀请");
					break;
				}
				if(i > 24){
					break;
				}
				cost.endStep();
				if (accepterId > 0) {
					sb.append(",").append(accepterId);
					User touser = SnsAdapterFactory.getUserAdapter().get(accepterId);									    
                    if(host!=null && touser!=null){
                        FriendApply fa=new FriendApply(host, touser,currentDomain);
                        fa.setFrom(reasonflag);
                        if(reasonflag==null || "".equals(reasonflag)){
                            GuideLogger.printLog(" host:"+host.getId()+" null ",GuideLogger.WARN);
                        }
                        else if("_".equals(reasonflag)){
                            GuideLogger.printLog(" host:"+host.getId()+" _ ",GuideLogger.WARN);
                        }
                        try {
                            FriendsHome.getInstance().requestFriendApply(fa);
                        } catch (Exception e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                        
                    }	
				}
				cost.endStep();
			}
			try {
            	String a = "["+reasonflag+"] "+ids.length+" ["+sb.toString()+"]";
            	GuideRemoteLogger.getInstance().log(host.getId(),GuideRemoteLogger.TYPE_REQUESTMULTFIRENDS,  a);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			}
			cost.endStep();
		} else {
			GuideLogger.printLog(" host:"+host.getId()+"to print 0 ...");
		}					
		cost.endFinally();
	}
	public static void beFriendsAsync(final User host,final String [] ids,final String currentDomain,String reasonflag){
		GuideTimeCost cost = GuideTimeCost.begin();
		cost.endStep();
		// 用户id组
		cost.endStep();
		try {
			cost.endStep();
			//final String currentDomain = OpiConstants.domain.toString();		
			final String from_flag = reasonflag;
			if(from_flag == null){
				GuideLogger.printLog(" host:"+host.getId()+" from null!",GuideLogger.WARN);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" from:"+from_flag);
			}
			//异步请求开始
			Runnable taskSave = new Runnable() {
				public void run() {
					beFriends(host,ids,currentDomain,from_flag);
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
			//异步请求结束
			cost.endStep();
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
	}
}
