package com.renren.sns.lead.core;

import java.util.HashMap;
import java.util.Map;

import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.renren.sns.lead.core.mapper.impl.DefaultMapper;
import com.renren.sns.lead.core.mapper.impl.FillinfoMapper;
import com.renren.sns.lead.core.mapper.impl.ImportMapper;
import com.renren.sns.lead.core.mapper.impl.MergeuserMapper;
import com.renren.sns.lead.core.mapper.impl.MorefriendMapper;
import com.renren.sns.lead.core.mapper.impl.PortraitMapper;
import com.renren.sns.lead.core.mapper.impl.PreviewMapper;

public class MapperFactory {
	private static final Map<String,AbstractLeadPageMapper> map = new HashMap<String, AbstractLeadPageMapper>();
	static{
		reloadStepMap();
	}
	private static void reloadStepMap() {
		map.put("Fillinfo", new FillinfoMapper());
		map.put("Preview", new PreviewMapper());
		map.put("Morefriend", new MorefriendMapper());
		map.put("Portrait", new PortraitMapper());
		map.put("Import", new ImportMapper());
		map.put("Mergeuser", new MergeuserMapper());
		map.put("Default", new DefaultMapper());
		
	}
	
	/**
	 * 简单的取Mapper的方法 
	 * @param string
	 * @return
	 */
	public static AbstractLeadPageMapper getMapper(String string) {
		// TODO Auto-generated method stub
		return map.get(string);
	}
}
