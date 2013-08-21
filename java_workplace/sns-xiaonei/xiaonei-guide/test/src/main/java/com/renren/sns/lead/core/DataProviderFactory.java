package com.renren.sns.lead.core;

import java.util.HashMap;
import java.util.Map;

import com.renren.sns.lead.dataproviders.AbstractLeadDataProvider;
import com.renren.sns.lead.dataproviders.impl.GuideBuddyReplyNotifyProvider;
import com.renren.sns.lead.dataproviders.impl.PreviewOfferfriendProvider;
import com.renren.sns.lead.dataproviders.impl.PreviewProfileProvider;

public class DataProviderFactory {
	private static final Map<String,AbstractLeadDataProvider> map = new HashMap<String, AbstractLeadDataProvider>();
	static{
		reloadStepMap();
	}
	private static void reloadStepMap() {
		map.put("PreviewProfile", new PreviewProfileProvider());
		map.put("PreviewOfferfriend", new PreviewOfferfriendProvider());
		map.put("GuideBuddyReplyNotify", new GuideBuddyReplyNotifyProvider());
		
	}
	public static AbstractLeadDataProvider getDataProvider(String string) {
		return map.get(string);
	}
	
}
