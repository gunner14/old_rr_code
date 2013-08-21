package com.xiaonei.reg.guide.logic.fillinfo;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil;
import com.xiaonei.reg.guide.util.GuideLogger;


public class GuideUserProfileCacheLogic {
	private static final String MEM_CACHE_GUIDE_REGION_INFO_KEY 			= "mem_cache_guide_region_info_";
	private static final String MEM_CACHE_GUIDE_UNIVERSITY_INFO_KEY 		= "mem_cache_guide_university_info_";
	private static final String MEM_CACHE_GUIDE_HIGHSCHOOL_INFO_KEY 		= "mem_cache_guide_high_info_";
	private static final String MEM_CACHE_GUIDE_JUNIORSCHOOL_INFO_KEY 		= "mem_cache_guide_junior_info_";
	private static final String MEM_CACHE_GUIDE_TECHSCHOOL_INFO_KEY 		= "mem_cache_guide_tech_info_";
	private static final String MEM_CACHE_GUIDE_ELEMENTARYSCHOOL_INFO_KEY 	= "mem_cache_guide_elem_info_";
	private static final String MEM_CACHE_GUIDE_WORK_INFO_KEY 				= "mem_cache_guide_work_info_";
	private static final String MEM_CACHE_GUIDE_HOMEREGION_INFO_KEY 		= "mem_cache_guide_homeregion_info_";
	private static final String MEM_CACHE_GUIDE_CURRENTREGION_INFO_KEY 		= "mem_cache_guide_currentregion_info_";
	
	private static final int MINITS = 5;
	
	private static final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
	
	static class SingletonHolder {
		static GuideUserProfileCacheLogic instance = new GuideUserProfileCacheLogic();
	}

	public static GuideUserProfileCacheLogic getInstance() {
		return SingletonHolder.instance;
	}
	
	
	public void saveUserCurrentRegion(User host, RegionInfo info){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_CURRENTREGION_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public RegionInfo getUserCurrentRegion(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		RegionInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_CURRENTREGION_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (RegionInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getProvince_name());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}


	public void saveUserUniversity(User host, UniversityInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_UNIVERSITY_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public UniversityInfo getUserUniversity(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		UniversityInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_UNIVERSITY_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (UniversityInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getUniversityName());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}
	
	public void saveUserRegion(User host, RegionInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_REGION_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public RegionInfo getUserRegion(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		RegionInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_REGION_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (RegionInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getProvince_name());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}

	public void saveUserHighschool(User host, HighSchoolInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		String key = MEM_CACHE_GUIDE_HIGHSCHOOL_INFO_KEY + host.getId();
		try {
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+key+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public HighSchoolInfo getUserHighschool(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		HighSchoolInfo info = null;
		String key = MEM_CACHE_GUIDE_HIGHSCHOOL_INFO_KEY + host.getId();
		try {
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (HighSchoolInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getHighSchoolName());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+key+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}
	
	public void saveUserHomeRegion(User host, RegionInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_HOMEREGION_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public RegionInfo getUserHomeRegion(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		RegionInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_HOMEREGION_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (RegionInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getProvince_name());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}

	public void saveUserWork(User host, WorkplaceInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_WORK_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public WorkplaceInfo getUserWork(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		WorkplaceInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_WORK_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (WorkplaceInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getWorkplaceName());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}

	public void saveUserJuniorschool(User host, JuniorHighSchoolInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_JUNIORSCHOOL_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}


	public JuniorHighSchoolInfo getUserJuniorschool(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		JuniorHighSchoolInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_JUNIORSCHOOL_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (JuniorHighSchoolInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getJunHighentarySchoolName());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}
	
	public void saveUserTechschool(User host, CollegeInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_TECHSCHOOL_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+GuideXFiveUserProfileUtil.toStringForLog(info));
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public CollegeInfo getUserTechschool(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		CollegeInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_TECHSCHOOL_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (CollegeInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getCollegeName());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}

	public void saveUserElementary(User host, ElementarySchoolInfo info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_ELEMENTARYSCHOOL_INFO_KEY + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+info.toString());
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public ElementarySchoolInfo getUserElementary(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		ElementarySchoolInfo info = null;
		try {
			String key = MEM_CACHE_GUIDE_ELEMENTARYSCHOOL_INFO_KEY + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (ElementarySchoolInfo)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+info.getElementarySchoolName());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}



	



}
