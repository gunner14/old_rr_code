package com.renren.sns.lead.dataproviders.impl;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.lead.dataproviders.AbstractLeadDataProvider;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.feed.FeedLogicAdapter;

public class GuideBuddyReplyNotifyProvider extends AbstractLeadDataProvider{

	@SuppressWarnings("unchecked")
	@Override
	protected <T> T doGetData(User host, long timeout, Object... param) {
		if(host == null){
			GuideLogger.printLog(" host: null",GuideLogger.WARN);
		}
		else if(param == null){
			GuideLogger.printLog(" host:"+host.getId()+" param null!",GuideLogger.WARN);
		}
		else{
			List<Integer> idList = null;
			try {
				idList = (List<Integer>)param[0];
				GuideLogger.printLog(" host:"+host.getId()+" get start!" + " prarm:" + (List<Integer>)param[0]);
			} catch (Exception e) {
				e.printStackTrace();
			}
			if (idList != null) {
				for (int i = 0; i < idList.size(); i++) {
					int fid = idList.get(i);
					//对方的隐私设置不为仅是好友可见(getBasicConfig()!= 0)
					if (SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(fid).getBasicConfig() != 0) {
						FeedLogicAdapter.getInstance().guideBuddyReplyNotify(host.getId(), fid);
						GuideLogger.printLog(" host:"+host.getId()+" get=> "+fid);
					}
				}
			}
			GuideLogger.printLog(" host:"+host.getId()+" get end!");
		}
		return null;
	}

	
	@SuppressWarnings("unchecked")
	private static void fun(int id,Object ... a){
		if(a == null) {
			System.out.println("a is null!");
			return;
		}
		List<Integer>  list = (List<Integer>)a[0];
		System.out.println(list);
	}
	public static void main(String[] args) {
		List<Integer> aaa = new ArrayList<Integer>();
		aaa.add(1);
		aaa.add(1);
		aaa.add(1);
		aaa.add(1);
		fun(1,aaa);
	}
}
