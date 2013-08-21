package com.xiaonei.reg.guide.flows.xfive.logics;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormBailing;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormDaxue;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormQita;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormZhongxue;
import com.xiaonei.reg.guide.flows.xfive.formbeans.VKFormDaxue;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileCacheLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

public class GuideXFiveUserProfileLogic {
	private static GuideXFiveUserProfileLogic instance = new GuideXFiveUserProfileLogic();

    public static GuideXFiveUserProfileLogic getInstance() {
        return instance;
    }
    
    private String cutString (String s){
    	if(s!=null){
			int len = s.length();
			if(len >= MAX_LENTH){
				s = s.substring(0,MAX_LENTH);
			}
		}
    	return s;
    }
    
	private void escapeString(FormBailing form){
		String s = null;
		
		s = form.getCompanyname();
		s = cutString(s);
		form.setCompanyname(Escape.stringToHTMLString(s));
	}
    
	private static final int MAX_LENTH = 100;
    private void escapeString(FormZhongxue form){
		String s = null;
		
//		s = form.getHighschoolname();
//		form.setHighschoolname(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass1();
		s = cutString(s);
		form.setHighschoolclass1(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass2();
		s = cutString(s);
		form.setHighschoolclass2(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass3();
		s = cutString(s);
		form.setHighschoolclass3(Escape.stringToHTMLString(s));
		
//		s = form.getJuniorhighschoolname();
//		form.setJuniorhighschoolname(Escape.stringToHTMLString(s));
//		
//		s = form.getTechschoolname();
//		form.setTechschoolname(Escape.stringToHTMLString(s));
//		
		s = form.getElementaryschoolname();
		s = cutString(s);
		form.setElementaryschoolname(Escape.stringToHTMLString(s));
	}
    
	/**
	 * TODO escapeString
	 *
	 * @param form
	 *
	 */
	private void escapeString(FormDaxue form) {
	}

	public void addUniversityAndNetDaxue(User host, FormDaxue form){
		GuideTimeCost cost = GuideTimeCost.begin();
		
		UniversityInfo info = new UniversityInfo();
		escapeString(form);
		cost.endStep();
		
		info.setUniversityId(form.getUniversitycode());
		info.setDepartment(form.getUniversitydepartment());
		info.setUniversityName(form.getUniversityname());
		info.setEnrollYear(form.getUniversityyear());
		info.setTypeOfCourse(form.getCoursetype());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncAddUniversity(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncSetUniversityNet(host, info);
		cost.endStep();
		cost.endFinally();
	}
	
	public void addVKUniversityAndNetDaxue(User host, VKFormDaxue form){
		GuideTimeCost cost = GuideTimeCost.begin();
		
		UniversityInfo info = new UniversityInfo();
		//escapeString(form);
		cost.endStep();
		
		info.setUniversityId(form.getUniversitycode());
		info.setDepartment(form.getUniversitydepartment());
		info.setUniversityName(form.getUniversityname());
		info.setEnrollYear(form.getUniversityyear());
		info.setTypeOfCourse(form.getCoursetype());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncAddUniversity(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncSetUniversityNet(host, info);
		cost.endStep();
		cost.endFinally();
	}
	

	public void addWorkBailing(User host, FormBailing fform) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(fform);
		WorkplaceInfo info = new WorkplaceInfo();
		
		if(fform.getCompanyname() == null 
			|| fform.getCompanyname().trim().equals("")
			|| fform.getCompanyname().trim().contains("选择单位")
		){
			GuideLogger.printLog(" host:"+host.getId()+" com not good:"+fform.getCompanyname(),GuideLogger.WARN);
			return;
		}
		info.setWorkplaceName(fform.getCompanyname());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserWork(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncAddWork(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveCurrentRegionAndNetBailing(User host, FormBailing fform) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(fform);
		RegionInfo info = new RegionInfo();
		
		info.setRegion_id(fform.getCurrentcitycode());
		info.setProvince_name(fform.getCurrentprovince());
		info.setCity_name(fform.getCurrentcityname());
		
		info.setUserid(host.getId());
		cost.endStep();
		
		GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
		GuideUserProfileCacheLogic.getInstance().saveUserCurrentRegion(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveCurrentRegionAndNet(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveCollegeNetZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		CollegeInfo info = new CollegeInfo();
		
		info.setCollegeName(form.getTechschoolname());
		info.setCollegeId(form.getTechschoolcode());
		info.setEnrollYear(form.getTechschoolyear());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSetTechschoolNet(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveHighschoolNetZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		HighSchoolInfo info = new HighSchoolInfo();
		
		info.setHighSchoolName(form.getHighschoolname());
		info.setHighSchoolId(form.getHighschoolcode());
		info.setEnrollYear(form.getHighschoolyear());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSetHighschoolNet(host, info);
		cost.endStep();
		cost.endFinally();
	}

	public void saveJuniorschoolNetZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
		
		info.setJunHighentarySchoolName(form.getJuniorhighschoolname());
		info.setJunHighentarySchoolId(form.getJuniorhighschoolcode());
		info.setJunHighentarySchoolYear(form.getJuniorhighschoolyear());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSetJuniorschoolNet(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveHighschoolDaxue(User host,FormDaxue form){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		HighSchoolInfo info = new HighSchoolInfo();
		
		info.setHighSchoolName(form.getHighschoolname());
		info.setHighSchoolId(form.getHighschoolcode());
		info.setEnrollYear(form.getHighschoolyear());
		
		info.setHClass1("");
        info.setHClass2("");
        info.setHClass3("");
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveHighschool(host, info);
		cost.endStep();
		cost.endFinally();
	}
	
	public void saveVKHighschoolDaxue(User host,VKFormDaxue form){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		HighSchoolInfo info = new HighSchoolInfo();
		
		info.setHighSchoolName(form.getHighschoolname());
		info.setHighSchoolId(form.getHighschoolcode());
		info.setEnrollYear(form.getHighschoolyear());
		
		info.setHClass1(form.getHighschoolclass1());
		info.setHClass2(form.getHighschoolclass2());
		info.setHClass3(form.getHighschoolclass3());
		
		//info.setHClass1("");
        //info.setHClass2("");
       // info.setHClass3("");
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveHighschool(host, info);
		cost.endStep();
		cost.endFinally();
	}
	
	

	public void saveHomeRegionQita(User host, FormQita fform) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		RegionInfo info = new RegionInfo();
		
		info.setRegion_id(fform.getHomecitycode());
		info.setProvince_name(fform.getHomeprovince());
		info.setCity_name(fform.getHomecityname());
		
		info.setUserid(host.getId());
		cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
		GuideUserProfileCacheLogic.getInstance().saveUserHomeRegion(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveHomeRegion(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveCurrentRegionAndNetQita(User host, FormQita fform) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		RegionInfo info = new RegionInfo();
		
		info.setRegion_id(fform.getCurrentcitycode());
		info.setProvince_name(fform.getCurrentprovince());
		info.setCity_name(fform.getCurrentcityname());
		
		info.setUserid(host.getId());
		cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
		GuideUserProfileCacheLogic.getInstance().saveUserCurrentRegion(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveCurrentRegionAndNet(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveHomeRegionBailing(User host, FormBailing fform) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(fform);
		RegionInfo info = new RegionInfo();
		
		info.setRegion_id(fform.getHomecitycode());
		info.setProvince_name(fform.getHomeprovince());
		info.setCity_name(fform.getHomecityname());
		
		info.setUserid(host.getId());
		cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
		GuideUserProfileCacheLogic.getInstance().saveUserHomeRegion(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveHomeRegion(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveJuniorschoolZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
		
		info.setJunHighentarySchoolName(form.getJuniorhighschoolname());
		info.setJunHighentarySchoolId(form.getJuniorhighschoolcode());
		info.setJunHighentarySchoolYear(form.getJuniorhighschoolyear());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserJuniorschool(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveJuniorschool(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveHighschoolZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		HighSchoolInfo info = new HighSchoolInfo();
		
		info.setHighSchoolName(form.getHighschoolname());
		info.setHighSchoolId(form.getHighschoolcode());
		info.setEnrollYear(form.getHighschoolyear());
		
		info.setHClass1(form.getHighschoolclass1());
		info.setHClass2(form.getHighschoolclass2());
		info.setHClass3(form.getHighschoolclass3());
		
		info.setUserId(host.getId());
		cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+info.getHClass1()+info.getHClass2()+info.getHClass3());
		GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveHighschool(host, info);
		cost.endStep();
		cost.endFinally();
		
	}

	public void saveElementaryschoolZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		ElementarySchoolInfo info = new ElementarySchoolInfo();
		
		info.setElementarySchoolName(form.getElementaryschoolname());
		info.setElementarySchoolId(form.getElementaryschoolcode());
		info.setElementarySchoolYear(form.getElementaryschoolyear());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserElementary(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveElementary(host, info);
		cost.endStep();
		cost.endFinally();
	}


	public void saveCollegeZhongxue(User host, FormZhongxue form) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		escapeString(form);
		CollegeInfo info = new CollegeInfo();
		
		info.setCollegeName(form.getTechschoolname());
		info.setCollegeId(form.getTechschoolcode());
		info.setEnrollYear(form.getTechschoolyear());
		
		info.setUserId(host.getId());
		cost.endStep();
		
		GuideUserProfileCacheLogic.getInstance().saveUserTechschool(host, info);
		cost.endStep();
		
		GuideXFiveUserProfileUtil.asyncSaveTechschool(host, info);
		cost.endStep();
		cost.endFinally();
		
	}
	
	public UniversityInfo loadUniversity(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		UniversityInfo info = GuideUserProfileCacheLogic.getInstance().getUserUniversity(host);
		cost.endStep();
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadUniversity(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getUniversityName());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	
	public CollegeInfo loadTechschool(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		CollegeInfo info = GuideUserProfileCacheLogic.getInstance().getUserTechschool(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadTechschool(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getCollegeName());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	public ElementarySchoolInfo loadElementary(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		ElementarySchoolInfo info = GuideUserProfileCacheLogic.getInstance().getUserElementary(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadElementary(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getElementarySchoolName());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	public HighSchoolInfo loadHighschool(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		HighSchoolInfo info = GuideUserProfileCacheLogic.getInstance().getUserHighschool(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadHighschool(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getHighSchoolName());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	public JuniorHighSchoolInfo loadJuniorschool(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		JuniorHighSchoolInfo info = GuideUserProfileCacheLogic.getInstance().getUserJuniorschool(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadJuniorschool(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getJunHighentarySchoolName());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	public WorkplaceInfo loadWorkplace(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		WorkplaceInfo info = GuideUserProfileCacheLogic.getInstance().getUserWork(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadWorkplace(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getWorkplaceName());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	
	public RegionInfo loadCurrentRegion(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		RegionInfo info = GuideUserProfileCacheLogic.getInstance().getUserCurrentRegion(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadCurrentRegion(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getProvince_name());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	
	public RegionInfo loadHomeRegion(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		RegionInfo info = GuideUserProfileCacheLogic.getInstance().getUserHomeRegion(host);
		if(info == null){
			info = GuideXFiveUserProfileUtil.loadHomeRegion(host);
		}
		cost.endStep();
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" load null");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getProvince_name());
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	
	public void asyncUpdateWork(final User host, final WorkplaceInfo info) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		GuideUserProfileCacheLogic.getInstance().saveUserWork(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncUpdateWork(host, info);
		cost.endStep();
		cost.endFinally();
	}

	public void asyncUpdateUniversity(final User host, final UniversityInfo info) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+" info:"+info.toString());
		GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncUpdateUniversity(host, info);
		cost.endStep();
		GuideLogger.printLog(" host:"+host.getId());
		cost.endStep();
		cost.endFinally();
	}

	public void asycSaveElementary(User host, ElementarySchoolInfo info) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+" info:"+info.toString());
		GuideUserProfileCacheLogic.getInstance().saveUserElementary(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncSaveElementary(host, info);
		GuideLogger.printLog(" host:"+host.getId());
		cost.endStep();
		cost.endFinally();
	}

	public void asyncSaveJuniorschool(User host, JuniorHighSchoolInfo info) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+" info:"+info.toString());
		GuideUserProfileCacheLogic.getInstance().saveUserJuniorschool(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncSaveJuniorschool(host, info);
		GuideLogger.printLog(" host:"+host.getId());
		cost.endStep();
		cost.endFinally();
	}

	public void asyncSaveHighschool(User host, HighSchoolInfo info) {
		GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		GuideLogger.printLog(" host:"+host.getId()+" info:"+info.toString());
		GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, info);
		cost.endStep();
		GuideXFiveUserProfileUtil.asyncSaveHighschool(host, info);
		GuideLogger.printLog(" host:"+host.getId());
		cost.endStep();
		cost.endFinally();
	}
}
