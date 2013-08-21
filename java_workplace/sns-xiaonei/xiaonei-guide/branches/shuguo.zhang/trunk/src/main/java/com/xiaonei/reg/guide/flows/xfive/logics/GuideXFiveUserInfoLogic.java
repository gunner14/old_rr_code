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
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileCacheLogic;
import com.xiaonei.reg.guide.root.interfaces.IInfoElementaryschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoJuniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoSeniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoTechschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoUniversity;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoCurrentPlace;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoHomePlace;
import com.xiaonei.reg.guide.root.interfaces.IWorkInfoCompany;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

public class GuideXFiveUserInfoLogic {
	private static GuideXFiveUserInfoLogic instance = new GuideXFiveUserInfoLogic();

    public static GuideXFiveUserInfoLogic getInstance() {
        return instance;
    }
    private static final int MAX_LENTH = 100;
    private String cutString (String s){
    	if(s!=null){
			int len = s.length();
			if(len >= MAX_LENTH){
				s = s.substring(0,MAX_LENTH);
			}
		}
    	return s;
    }
    
    /**
     * TODO escapeString
     *
     * @param form
     *
     */
    private void escapeString(IInfoTechschool form) {
    	if(form == null) return;
		String s = null;
		
		
		s = form.getTechschoolname();
		s = cutString(s);
		form.setTechschoolname(Escape.stringToHTMLString(s));
		
	}

	/**
	 * TODO escapeString
	 *
	 * @param form
	 *
	 */
	private void escapeString(IInfoJuniorhighschool form) {
		if(form == null) return;
		String s = null;
		
		
		s = form.getJuniorhighschoolname();
		s = cutString(s);
		form.setJuniorhighschoolname(Escape.stringToHTMLString(s));
	}

	/**
	 * TODO escapeString
	 *
	 * @param fform
	 *
	 */
	private void escapeString(IRegionInfoHomePlace form) {
		if(form == null) return;
		String s = null;
		
		s = form.getHomecityname();
		s = cutString(s);
		form.setHomecityname(Escape.stringToHTMLString(s));
		
		s = form.getHomeprovince();
		s = cutString(s);
		form.setHomeprovince(Escape.stringToHTMLString(s));
		
	}

	private void escapeString(IRegionInfoCurrentPlace form) {
		if(form == null) return;
		String s = null;
		
		s = form.getCurrentprovince();
		s = cutString(s);
		form.setCurrentprovince(Escape.stringToHTMLString(s));
		
		s = form.getCurrentcityname();
		s = cutString(s);
		form.setCurrentcityname(Escape.stringToHTMLString(s));
	}

	/**
	 * TODO escapeString
	 *
	 * @param form
	 *
	 */
	private void escapeString(IInfoUniversity form) {
		if(form == null) return;
		String s = null;
		
		
		s = form.getUniversityname();
		s = cutString(s);
		form.setUniversityname(Escape.stringToHTMLString(s));
		
		s = form.getUniversitydepartment();
		s = cutString(s);
		form.setUniversitydepartment(Escape.stringToHTMLString(s));
		
		s = form.getCoursetype();
		s = cutString(s);
		form.setCoursetype(Escape.stringToHTMLString(s));
		
	}

	private void escapeString(IInfoSeniorhighschool form) {
		if(form == null) return;
		String s = null;
		
		s = form.getHighschoolname();
		s = cutString(s);
		form.setHighschoolname(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass1();
		s = cutString(s);
		form.setHighschoolclass1(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass2();
		s = cutString(s);
		form.setHighschoolclass2(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass3();
		s = cutString(s);
		form.setHighschoolclass3(Escape.stringToHTMLString(s));
		
	}

	private void escapeString(IInfoElementaryschool form) {
		if(form == null) return;
		String s = null;
		
		s = form.getElementaryschoolname();
		s = cutString(s);
		form.setElementaryschoolname(Escape.stringToHTMLString(s));
	
		
	}

	private void escapeString(IWorkInfoCompany form) {
		if(form == null) return;
		String s = null;
		
		s = form.getCompanyname();
		s = cutString(s);
		form.setCompanyname(Escape.stringToHTMLString(s));
		
	}

	public void addUniversityAndNet(User host, IInfoUniversity form){
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			
			escapeString(form);
			cost.endStep();
			
			UniversityInfo info = extractInfoUniversity(host, form);
			
			cost.endStep();
			
			GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncAddUniversity(host, info);
			cost.endStep();
			GuideXFiveUserProfileUtil.asyncSetUniversityNet(host, info);
			cost.endStep();
			cost.endFinally();
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public UniversityInfo extractInfoUniversity(User host, IInfoUniversity form) {
		UniversityInfo info = new UniversityInfo();
		info.setUniversityId(form.getUniversitycode());
		info.setDepartment(form.getUniversitydepartment());
		info.setUniversityName(form.getUniversityname());
		info.setEnrollYear(form.getUniversityyear());
		info.setTypeOfCourse(form.getCoursetype());
		info.setUserId(host.getId());
		return info;
	}


	
	public void addUniversity(User host, IInfoUniversity form){
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			
			escapeString(form);
			cost.endStep();
			
			UniversityInfo info = extractInfoUniversity(host, form);
			
			cost.endStep();
			
			GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncAddUniversity(host, info);
			cost.endStep();
			cost.endFinally();
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public void saveUniversity(User host, IInfoUniversity form){
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
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
			
			GuideXFiveUserProfileUtil.asyncUpdateUniversity(host, info);
			cost.endStep();
			cost.endFinally();
			
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public void saveUniversityNet(User host, IInfoUniversity info) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			UniversityInfo uinfo = new UniversityInfo();
			escapeString(info);
			
			uinfo.setUniversityId(info.getUniversitycode());
			uinfo.setDepartment(info.getUniversitydepartment());
			uinfo.setUniversityName(info.getUniversityname());
			uinfo.setEnrollYear(info.getUniversityyear());
			uinfo.setTypeOfCourse(info.getCoursetype());
			
			uinfo.setUserId(host.getId());
			GuideXFiveUserProfileUtil.asyncSetUniversityNet(host, uinfo);
			
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public void addWork(User host, IWorkInfoCompany fform) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(fform == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(fform);
			WorkplaceInfo info = new WorkplaceInfo();
			
			if(fform.getCompanyname() == null 
					|| fform.getCompanyname().trim().equals("")
					|| fform.getCompanyname().trim().contains("选择单位")
					|| fform.getCompanyname().trim().contains("填写单位")
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
			
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	public void saveWork(User host, IWorkInfoCompany fform) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(fform == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(fform);
			WorkplaceInfo info = new WorkplaceInfo();
			
			if(fform.getCompanyname() == null 
					|| fform.getCompanyname().trim().equals("")
					|| fform.getCompanyname().trim().contains("选择单位")
					|| fform.getCompanyname().trim().contains("填写单位")
			){
				GuideLogger.printLog(" host:"+host.getId()+" com not good:"+fform.getCompanyname(),GuideLogger.WARN);
				return;
			}
			info.setWorkplaceName(fform.getCompanyname());
			
			info.setUserId(host.getId());
			cost.endStep();
			
			GuideUserProfileCacheLogic.getInstance().saveUserWork(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncUpdateWork(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public void saveHighschool(User host, IInfoSeniorhighschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(form);
			
			HighSchoolInfo info = extractInfoHighschool(host, form);
			
			cost.endStep();
			GuideLogger.printLog(" host:"+host.getId()+info.getHClass1()+info.getHClass2()+info.getHClass3());
			GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSaveHighschool(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public HighSchoolInfo extractInfoHighschool(User host,IInfoSeniorhighschool form) {
		HighSchoolInfo info = new HighSchoolInfo();
		info.setHighSchoolName(form.getHighschoolname());
		info.setHighSchoolId(form.getHighschoolcode());
		info.setEnrollYear(form.getHighschoolyear());
		info.setHClass1(form.getHighschoolclass1());
		info.setHClass2(form.getHighschoolclass2());
		info.setHClass3(form.getHighschoolclass3());
		info.setUserId(host.getId());
		return info;
	}
	
	public void saveCollegeNet(User host, IInfoTechschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(form);
			CollegeInfo info = extractInfoTechschool(host, form);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSetTechschoolNet(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}


	public void saveJuniorschoolNet(User host, IInfoJuniorhighschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
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
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public void saveHighschoolNet(User host, IInfoSeniorhighschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
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
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
    
	public void saveJuniorschool(User host, IInfoJuniorhighschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
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
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public void saveElementaryschool(User host, IInfoElementaryschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(form);
			
			ElementarySchoolInfo info = extractInfoElementaryschool(host, form);
			
			cost.endStep();
			
			GuideUserProfileCacheLogic.getInstance().saveUserElementary(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSaveElementary(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}

	public ElementarySchoolInfo extractInfoElementaryschool(User host,	IInfoElementaryschool form) {
		ElementarySchoolInfo info = new ElementarySchoolInfo();
		info.setElementarySchoolName(form.getElementaryschoolname());
		info.setElementarySchoolId(form.getElementaryschoolcode());
		info.setElementarySchoolYear(form.getElementaryschoolyear());
		info.setUserId(host.getId());
		return info;
	}


	public void saveCollege(User host, IInfoTechschool form) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(form == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(form);
			
			CollegeInfo info = extractInfoTechschool(host, form);
			
			cost.endStep();
			
			GuideUserProfileCacheLogic.getInstance().saveUserTechschool(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSaveTechschool(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public CollegeInfo extractInfoTechschool(User host, IInfoTechschool form) {
		CollegeInfo info = new CollegeInfo();
		info.setCollegeName(form.getTechschoolname());
		info.setCollegeId(form.getTechschoolcode());
		info.setEnrollYear(form.getTechschoolyear());
		info.setUserId(host.getId());
		return info;
	}

	
	public void saveCurrentRegion(User host, IRegionInfoCurrentPlace fform) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(fform == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(fform);
			
			RegionInfo info = extractInfoCurrentregion(host, fform);
			
			cost.endStep();
			GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
			GuideUserProfileCacheLogic.getInstance().saveUserCurrentRegion(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSaveCurrentRegion(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public RegionInfo extractInfoCurrentregion(User host,
			IRegionInfoCurrentPlace fform) {
		RegionInfo info = new RegionInfo();
		info.setRegion_id(fform.getCurrentcitycode());
		info.setProvince_name(fform.getCurrentprovince());
		info.setCity_name(fform.getCurrentcityname());
		info.setUserid(host.getId());
		return info;
	}
	
	public void saveCurrentRegionNet(User host, IRegionInfoCurrentPlace fform) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(fform == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(fform);
			
			RegionInfo info = extractInfoCurrentregion(host, fform);
			cost.endStep();
			GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
			
			GuideXFiveUserProfileUtil.asyncSaveCurrentRegionAndNet(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public void saveCurrentRegionAndNet(User host, IRegionInfoCurrentPlace fform) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(fform == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(fform);
			
			RegionInfo info = extractInfoCurrentregion(host, fform);
			cost.endStep();
			GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
			GuideUserProfileCacheLogic.getInstance().saveUserCurrentRegion(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSaveCurrentRegionAndNet(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public void saveHomeRegion(User host, IRegionInfoHomePlace fform) {
		if(host == null){
			GuideLogger.printLog(" host null!", GuideLogger.ERROR);
			return;
		}
		if(fform == null){
			GuideLogger.printLog(" host:"+host.getId()+" form null",GuideLogger.ERROR);
			return;
		}
		
		try {
			GuideLogger.printLog(" host:"+host.getId());
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			escapeString(fform);
			
			RegionInfo info = extractInfoHomeregion(host, fform);
			
			cost.endStep();
			GuideLogger.printLog(" host:"+host.getId()+info.getProvince_name()+info.getCity_name()+info.getRegion_id());
			GuideUserProfileCacheLogic.getInstance().saveUserHomeRegion(host, info);
			cost.endStep();
			
			GuideXFiveUserProfileUtil.asyncSaveHomeRegion(host, info);
			cost.endStep();
			cost.endFinally();
			
		} 
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public RegionInfo extractInfoHomeregion(User host, IRegionInfoHomePlace fform) {
		RegionInfo info = new RegionInfo();
		info.setRegion_id(fform.getHomecitycode());
		info.setProvince_name(fform.getHomeprovince());
		info.setCity_name(fform.getHomecityname());
		info.setUserid(host.getId());
		return info;
	}
	
	public IInfoUniversity getInfoUniversity(User host){
		if(host == null)return null;
		final UniversityInfo info = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
		if(info == null)return null;
		IInfoUniversity iinfo = new IInfoUniversity(){

			@Override
			public String getCoursetype() {
				return info.getTypeOfCourse();
			}

			@Override
			public int getUniversitycode() {
				return info.getUniversityId();
			}

			@Override
			public String getUniversitydepartment() {
				return info.getDepartment();
			}

			@Override
			public String getUniversityname() {
				return info.getUniversityName();
			}

			@Override
			public int getUniversityyear() {
				return info.getEnrollYear();
			}

			@Override
			public void setCoursetype(String coursetype) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setUniversitycode(int universitycode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setUniversitydepartment(String universitydepartment) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setUniversityname(String universityname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setUniversityyear(int universityyear) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getUniversityid() {
				return info.getId();
			}

			@Override
			public void setUniversityid(int universityid) {
				// TODO Auto-generated method stub
				
			}


			
		};
		return iinfo;
		
	}
	
	public IInfoElementaryschool getInfoElementaryschool(User host){
		if(host == null)return null;
		final ElementarySchoolInfo info = GuideXFiveUserProfileLogic.getInstance().loadElementary(host);
		if(info == null)return null;
		IInfoElementaryschool iinfo = new IInfoElementaryschool(){

			@Override
			public int getElementaryschoolcode() {
				return info.getElementarySchoolId();
			}

			@Override
			public String getElementaryschoolname() {
				return info.getElementarySchoolName();
			}

			@Override
			public int getElementaryschoolyear() {
				return info.getElementarySchoolYear();
			}

			@Override
			public void setElementaryschoolcode(int elementaryschoolcode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setElementaryschoolname(String elementaryschoolname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setElementaryschoolyear(int elementaryschoolyear) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getElementaryschoolid() {
				return 0;//return info.getElementarySchoolId();
			}

			@Override
			public void setElementaryschoolid(int elementaryschoolid) {
				// TODO Auto-generated method stub
				
			}
			
		};
		
		return iinfo;
	}
	
	public IInfoTechschool getInfoTechschool(User host){
		if(host == null)return null;
		final CollegeInfo info = GuideXFiveUserProfileLogic.getInstance().loadTechschool(host);
		if(info == null)return null;
		IInfoTechschool iinfo = new IInfoTechschool(){

			@Override
			public int getTechschoolcode() {
				return info.getCollegeId();
			}

			@Override
			public String getTechschoolname() {
				return info.getCollegeName();
			}

			@Override
			public int getTechschoolyear() {
				return info.getEnrollYear();
			}

			@Override
			public void setTechschoolcode(int techschoolcode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setTechschoolname(String techschoolname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setTechschoolyear(int techschoolyear) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getTechschoolid() {
				return info.getId();
			}

			@Override
			public void setTechschoolid(int techschoolid) {
				// TODO Auto-generated method stub
				
			}
			
		};
		return iinfo;
	}
	
	public IInfoSeniorhighschool getInfoSeniorhighschool(User host){
		if(host == null)return null;
		final HighSchoolInfo info = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
		if(info == null)return null;
		IInfoSeniorhighschool iinfo = new IInfoSeniorhighschool(){

			@Override
			public String getHighschoolclass1() {
				return info.getHClass1();
			}

			@Override
			public String getHighschoolclass2() {
				return info.getHClass2();
			}

			@Override
			public String getHighschoolclass3() {
				return info.getHClass3();
			}

			@Override
			public int getHighschoolcode() {
				return info.getHighSchoolId();
			}

			@Override
			public String getHighschoolname() {
				return info.getHighSchoolName();
			}

			@Override
			public int getHighschoolyear() {
				return info.getEnrollYear();
			}

			@Override
			public void setHighschoolclass1(String highschoolclass1) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHighschoolclass2(String highschoolclass2) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHighschoolclass3(String highschoolclass3) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHighschoolcode(int highschoolcode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHighschoolname(String highschoolname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHighschoolyear(int highschoolyear) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getHighschoolid() {
				return info.getId();
			}

			@Override
			public void setHighschoolid(int highschoolid) {
				// TODO Auto-generated method stub
				
			}
			
		};
		return iinfo;
	}
	
	public IInfoJuniorhighschool getInfoJuniorhighschool(User host){
		if(host == null)return null;
		final JuniorHighSchoolInfo info = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
		if(info == null)return null;
		IInfoJuniorhighschool iinfo = new IInfoJuniorhighschool(){

			@Override
			public int getJuniorhighschoolcode() {
				return info.getJunHighentarySchoolId();
			}

			@Override
			public String getJuniorhighschoolname() {
				return info.getJunHighentarySchoolName();
			}

			@Override
			public int getJuniorhighschoolyear() {
				return info.getJunHighentarySchoolYear();
			}

			@Override
			public void setJuniorhighschoolcode(int juniorhighschoolcode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setJuniorhighschoolname(String juniorhighschoolname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setJuniorhighschoolyear(int juniorhighschoolyear) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getJuniorhighschoolid() {
				return 0;//return info.getJunHighentarySchoolId();
			}

			@Override
			public void setJuniorhighschoolid(int juniorhighschoolid) {
				// TODO Auto-generated method stub
				
			}
			
		};
		return iinfo;
	}
	
	public IRegionInfoCurrentPlace getRegionInfoCurrentPlace(User host){
		if(host == null)return null;
		final RegionInfo info = GuideXFiveUserProfileLogic.getInstance().loadCurrentRegion(host);
		if(info == null)return null;
		IRegionInfoCurrentPlace iinfo = new IRegionInfoCurrentPlace(){

			@Override
			public int getCurrentcitycode() {
				return info.getRegion_id();
			}

			@Override
			public String getCurrentcityname() {
				return info.getCity_name();
			}

			@Override
			public String getCurrentprovince() {
				return info.getProvince_name();
			}

			@Override
			public void setCurrentcitycode(int currentcitycode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setCurrentcityname(String currentcityname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setCurrentprovince(String currentprovince) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getCurrentregionid() {
				return info.getId();
			}

			@Override
			public void setCurrentregionid(int currentregionid) {
				// TODO Auto-generated method stub
				
			}
			
		};
		return iinfo;
		
	}
	public IWorkInfoCompany getWorkInfoCompany(User host){
		if(host == null)return null;
		final WorkplaceInfo info = GuideXFiveUserProfileLogic.getInstance().loadWorkplace(host);
		if(info == null)return null;
		IWorkInfoCompany iinfo = new IWorkInfoCompany(){

			@Override
			public String getCompanyname() {
				return info.getWorkplaceName();
			}

			@Override
			public void setCompanyname(String companyname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getCompanyid() {
				return info.getId();
			}

			@Override
			public void setCompanyid(int companyid) {
				// TODO Auto-generated method stub
				
			}
			
		};
		return iinfo;
	}
	public IRegionInfoHomePlace getRegionInfoHomePlace(User host){
		if(host == null)return null;
		final RegionInfo info = GuideXFiveUserProfileLogic.getInstance().loadHomeRegion(host);
		if(info == null)return null;
		IRegionInfoHomePlace iinfo = new IRegionInfoHomePlace(){

			@Override
			public int getHomecitycode() {
				return info.getRegion_id();
			}

			@Override
			public String getHomecityname() {
				return info.getCity_name();
			}

			@Override
			public String getHomeprovince() {
				return info.getProvince_name();
			}

			@Override
			public void setHomecitycode(int homecitycode) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHomecityname(String homecityname) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void setHomeprovince(String homeprovince) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public int getHomeregionid() {
				return info.getId();
			}

			@Override
			public void setHomeregionid(int homeregionid) {
				// TODO Auto-generated method stub
				
			}
		};
		
		return iinfo;

	}


}
