package com.xiaonei.reg.guide.action.a090821;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.Network;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.datum.client.DatumNetworkClient;
import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.guide.action.a090729.A090729Util;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.netbarflow.FillinfoFormBean;
import com.xiaonei.reg.guide.form.f090729.FormBailing;
import com.xiaonei.reg.guide.form.f090729.FormDaxue;
import com.xiaonei.reg.guide.form.f090729.FormQita;
import com.xiaonei.reg.guide.form.f090729.FormZhongxue;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;

public class UserProfileUtil {
	
	public static boolean is09FreshMan(User host){
		MyTimeCost cost = MyTimeCost.begin();
		boolean ret = false;
		try {
			UniversityInfo univ = loadUniversity(host);
			if(univ!=null && univ.getEnrollYear()==2009){
				ret = true;
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
		}
		cost.endFinally();
		return ret;
	}
	
	public static List<Network> loadUniversityNetwork(User host){
		MyTimeCost cost = MyTimeCost.begin();
		List<Network> list = null;
		try {
			list = DatumNetworkClient.getInstance().getUserUnivNetwork(host.getId());
			int i = 0;
			if(list != null){
				for(Network item : list){
					GuideLogger.printLog(" host:"+host.getId()+":"+ i++ +":"+item.getNetworkName()+" "+item.getJoinTime());
				}
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" no univ network");
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		cost.endFinally();
		return list;
		
	}
	
	public static WorkplaceInfo loadWork(User host){
		MyTimeCost cost = MyTimeCost.begin();
		List<WorkplaceInfo> worklist = DatumInfoClient.getInstance().getWorkplaceInfo(host);
		if(worklist == null || worklist.size() == 0){
			GuideLogger.printLog(" - worklist null or size 0"+host.getId());
			return null;
		}
		WorkplaceInfo work = worklist.get(worklist.size()-1);
		cost.endFinally();
		return work;
	}
	
	public static UniversityInfo loadUniversity(User host){
		MyTimeCost cost = MyTimeCost.begin();
		UniversityInfo university = null;
		try {
			List<UniversityInfo> universitylist = DatumInfoClient.getInstance().getUniversityInfos(host);
			if(universitylist == null || universitylist.size() == 0){
				GuideLogger.printLog(" - universitylist null or size 0"+host.getId());
				return null;
			}
			else{
				int i = 0;
				for(UniversityInfo item : universitylist){
					GuideLogger.printLog(" host:"+host.getId()+":"+ i++ +":"+item.getUniversityName()+" "+item.getUniversityId());
				}
			}
			university = universitylist.get(universitylist.size()-1);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
		return university;
	}
	
	public static HighSchoolInfo loadHighschool(User host){
		MyTimeCost cost = MyTimeCost.begin();
		List<HighSchoolInfo> highschoollist = DatumInfoClient.getInstance().getHighSchoolInfos(host);
		if(highschoollist == null || highschoollist.size() == 0){
			GuideLogger.printLog(" - highschoollist null or size 0"+host.getId());
			return null;
		}
		HighSchoolInfo highschool = highschoollist.get(highschoollist.size()-1);
		cost.endFinally();
		return highschool;
	}
	
	public static CollegeInfo loadCollege(User host){
		MyTimeCost cost = MyTimeCost.begin();
		List<CollegeInfo> collegelist = DatumInfoClient.getInstance().getCollegeInfos(host);
		if(collegelist == null || collegelist.size() == 0){
			GuideLogger.printLog(" - collegelist null or size 0"+host.getId());
			return null;
		}
		CollegeInfo college = collegelist.get(collegelist.size()-1);
		cost.endFinally();
		return college;
	}
	public static JuniorHighSchoolInfo loadJuniorhighschool(User host){
		MyTimeCost cost = MyTimeCost.begin();
		JuniorHighSchoolInfo juniorhighschool = DatumInfoClient.getInstance().getJuniorHighSchoolInfos(host);
		cost.endFinally();
		return juniorhighschool;
	}
	
	public static ElementarySchoolInfo loadElementaryschool(User host){
		MyTimeCost cost = MyTimeCost.begin();
		ElementarySchoolInfo elementaryschool = DatumInfoClient.getInstance().getElementarySchoolInfos(host);
		cost.endFinally();
		return elementaryschool;
	}
	
	public static RegionInfo loadRegion(User host) {
		MyTimeCost cost = MyTimeCost.begin();
		RegionInfo region = null;
		try {
			region = DatumInfoClient.getInstance().getRegionInfo(host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
		return region;
	}

	//zhongxue start
	//高中 - 中学会用
	public static void saveHighschool(User host, FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		HighSchoolInfo highschool = new HighSchoolInfo();
		
		highschool.setHighSchoolName(form.getHighschoolname());
		highschool.setHighSchoolId(form.getHighschoolcode());
		highschool.setEnrollYear(form.getHighschoolyear());

		highschool.setHClass1(form.getHighschoolclass1());
		highschool.setHClass2(form.getHighschoolclass2());
		highschool.setHClass3(form.getHighschoolclass3());
		
		highschool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyHighSchoolInfo(host, highschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//高中网络 - 中学会用
	public static void saveHighschoolNet(User host, FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		HighSchoolInfo highschool = new HighSchoolInfo();
		
		highschool.setHighSchoolName(form.getHighschoolname());
		highschool.setHighSchoolId(form.getHighschoolcode());
		highschool.setEnrollYear(form.getHighschoolyear());
		
		highschool.setHClass1(form.getHighschoolclass1());
		highschool.setHClass2(form.getHighschoolclass2());
		highschool.setHClass3(form.getHighschoolclass3());
		
		highschool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host, highschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(highschool.getHighSchoolId(), host);
		}
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" = \""+e.unknown+"\" msg:"+msg,GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//2009-10-21 10:58:09 加上
	//初中网络 - 中学会用
	public static void saveJuniorschoolNet(User host, FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		JuniorHighSchoolInfo juniorchool = new JuniorHighSchoolInfo();
		
		juniorchool.setJunHighentarySchoolId(form.getJuniorhighschoolcode());
		juniorchool.setJunHighentarySchoolName(form.getJuniorhighschoolname());
		juniorchool.setJunHighentarySchoolYear(form.getJuniorhighschoolyear());
		juniorchool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneJunSchoolNetwork(host, juniorchool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(juniorchool.getJunHighentarySchoolId(), host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+ msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//2009-10-21 10:58:09 加上
	//技校网络 - 中学会用
	public static void saveCollegeNet(User host, FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		CollegeInfo college = new CollegeInfo();
		
		college.setCollegeId(form.getTechschoolcode());
		college.setCollegeName(form.getTechschoolname());
		college.setEnrollYear(form.getTechschoolyear());
		college.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneCollSchoolNetwork(host, college);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(college.getCollegeId(), host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+ msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//2009-9-19 17:13:28 加上
	//高中并网络 - 中学会用
	public static void saveHighschoolAndNet(User host, FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		HighSchoolInfo highschool = new HighSchoolInfo();
		
		highschool.setHighSchoolName(form.getHighschoolname());
		highschool.setHighSchoolId(form.getHighschoolcode());
		highschool.setEnrollYear(form.getHighschoolyear());
		
		highschool.setHClass1(form.getHighschoolclass1());
		highschool.setHClass2(form.getHighschoolclass2());
		highschool.setHClass3(form.getHighschoolclass3());
		
		highschool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyHighSchoolInfo(host, highschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			msg = DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host, highschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(highschool.getHighSchoolId(), host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//初中 - 中学会用
	public static void saveJuniorschool(User host,FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		JuniorHighSchoolInfo juniorchool = new JuniorHighSchoolInfo();
		
		juniorchool.setJunHighentarySchoolId(form.getJuniorhighschoolcode());
		juniorchool.setJunHighentarySchoolName(form.getJuniorhighschoolname());
		juniorchool.setJunHighentarySchoolYear(form.getJuniorhighschoolyear());
		juniorchool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyJuniorHighSchoolInfo(host, juniorchool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
		
	}
	
	//技校 - 中学会用
	public static void saveCollege(User host,FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		CollegeInfo college = new CollegeInfo();
		
		college.setCollegeId(form.getTechschoolcode());
		college.setCollegeName(form.getTechschoolname());
		college.setEnrollYear(form.getTechschoolyear());
		college.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyCollegeInfo(host, college);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//小学 - 中学会用
	public static void saveElementaryschool(User host,FormZhongxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		ElementarySchoolInfo elementaryschool = new ElementarySchoolInfo();
		
		elementaryschool.setElementarySchoolId(form.getElementaryschoolcode());
		elementaryschool.setElementarySchoolName(form.getElementaryschoolname());
		elementaryschool.setElementarySchoolYear(form.getElementaryschoolyear());
		elementaryschool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyElementarySchoolInfo(host, elementaryschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//zhongxue end
	
	
	//bailing start
	//单位 - 白领会用
	public static void saveWork(User host,FormBailing form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		WorkplaceInfo work = new WorkplaceInfo();

		work.setWorkplaceName(form.getCompanyname());
//		work.setCityId(form.getHomecitycode());
//		work.setCityName(form.getHomecityname());
//		work.setProvince(form.getHomeprovince());
		work.setUserId(host.getId());
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyWorkplaceInfo(host, work);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
//			MyLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),MyLogger.ERROR);
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" param: "+(work==null)+" | "+(work.getWorkplaceName()+" | "+work.getUserId())+" || "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	//所在地 - 白领不会用（用信息并网络的）
	public static void saveRegion(User host, FormBailing form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		RegionInfo region = new RegionInfo();
		region.setProvince_name(form.getHomeprovince());
		region.setCity_name(form.getHomecityname());
		region.setRegion_id(form.getHomecitycode());
		region.setUserid(host.getId());
		try {
			DatumInfoClient.getInstance().modifyRegionInfo(host, region);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	//所在地并网络 - 白领会用
	public static void saveRegionAndNet(User host, FormBailing form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		RegionInfo region = new RegionInfo();
		region.setProvince_name(form.getHomeprovince());
		region.setCity_name(form.getHomecityname());
		region.setRegion_id(form.getHomecitycode());
		region.setUserid(host.getId());
		try {
			DatumInfoClient.getInstance().modifyRegionInfoAndNet(host, region);
			DatumNetworkClient.getInstance().setCurrentNetWork(region.getRegion_id(), host);
		} 
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" = \""+e.unknown+"\" ",GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//所在地的网络 - 白领不会用 （用信息并网络的）
	public static void saveRegionNet(User host, FormBailing form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		RegionInfo region = new RegionInfo();
		region.setProvince_name(form.getHomeprovince());
		region.setCity_name(form.getHomecityname());
		region.setRegion_id(form.getHomecitycode());
		region.setUserid(host.getId());
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneRegionNetwork(host, region);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}

	//大学 - 白领会用
	public static void saveUniversity(User host, FormBailing form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		UniversityInfo university = new UniversityInfo();
		cost.endStep();
		
		university.setUniversityId(form.getUniversitycode());
		university.setDepartment(form.getUniversitydepartment());
		university.setUniversityName(form.getUniversityname());
		university.setEnrollYear(form.getUniversityyear());
		university.setUserId(host.getId());
		cost.endStep();
		String msg = "";
		try {
			cost.endStep();
			msg = DatumInfoClient.getInstance().modifyUniversityInfo(host, university);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
	}
	
	//大学并网络 - 白领不会用（白领用不带网络的）
	public static void saveUniversityNet(User host, FormBailing form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		UniversityInfo university = new UniversityInfo();
		
		university.setUniversityId(form.getUniversitycode());
		university.setDepartment(form.getUniversitydepartment());
		university.setUniversityName(form.getUniversityname());
		university.setEnrollYear(form.getUniversityyear());
		university.setUserId(host.getId());
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneUnivNetwork(host, university);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} 
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" = \""+e.unknown+"\" msg:"+msg,GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//bailing end
	
	//daxue start
	//大学 - 大学会用
	public static void saveUniversity(User host, FormDaxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		UniversityInfo university = new UniversityInfo();
		
		university.setUniversityId(form.getUniversitycode());
		university.setDepartment(form.getUniversitydepartment());
		university.setUniversityName(form.getUniversityname());
		university.setEnrollYear(form.getUniversityyear());
		university.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyUniversityInfo(host, university);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	//大学的网络 - 大学会用
	public static void saveUniversityNet(User host, FormDaxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		UniversityInfo university = new UniversityInfo();
		
		university.setUniversityId(form.getUniversitycode());
		university.setDepartment(form.getUniversitydepartment());
		university.setUniversityName(form.getUniversityname());
		university.setEnrollYear(form.getUniversityyear());
		university.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneUnivNetwork(host, university);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(university.getUniversityId(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - 2");
		}
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" = \""+e.unknown+"\" msg:"+msg,GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//高中 - 大学会用
	public static void saveHighschool(User host, FormDaxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		HighSchoolInfo highschool = new HighSchoolInfo();
		
		highschool.setHighSchoolName(form.getHighschoolname());
		highschool.setHighSchoolId(form.getHighschoolcode());
		highschool.setEnrollYear(form.getHighschoolyear());
		highschool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyHighSchoolInfo(host, highschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	//高中网络 - 大学会用
	public static void saveHighschoolNet(User host, FormDaxue form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		HighSchoolInfo highschool = new HighSchoolInfo();
		
		highschool.setHighSchoolName(form.getHighschoolname());
		highschool.setHighSchoolId(form.getHighschoolcode());
		highschool.setEnrollYear(form.getHighschoolyear());
		highschool.setUserId(host.getId());
		
		String msg = "";
		try {
			msg = DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host, highschool);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+msg +" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	//daxue end
	
	//qita start
	public static void saveUniversity(User host, FormQita form) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		UniversityInfo university = new UniversityInfo();
		
		university.setUniversityId(form.getUniversitycode());
		university.setDepartment(form.getUniversitydepartment());
		university.setUniversityName(form.getUniversityname());
		university.setEnrollYear(form.getUniversityyear());
		university.setUserId(host.getId());
		String msg = "";
		try {
			msg = DatumInfoClient.getInstance().modifyUniversityInfo(host, university);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	public static void saveRegionAndNet(User host, FormQita form) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		RegionInfo region = new RegionInfo();
		region.setProvince_name(form.getHomeprovince());
		region.setCity_name(form.getHomecityname());
		region.setRegion_id(form.getHomecitycode());
		region.setUserid(host.getId());
		try {
			DatumInfoClient.getInstance().modifyRegionInfoAndNet(host, region);
			DatumNetworkClient.getInstance().setCurrentNetWork(region.getRegion_id(), host);
		} 
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" = \""+e.unknown+"\"",GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void escapeString(FormQita form){
		String s = null;
		s = form.getHomecityname();
		form.setHomecityname(Escape.stringToHTMLString(s));
		
		s = form.getHomeprovince();
		form.setHomeprovince(Escape.stringToHTMLString(s));
		
		s = form.getUniversitydepartment();
		form.setUniversitydepartment(Escape.stringToHTMLString(s));
		
		s = form.getUniversityname();
		form.setUniversityname(Escape.stringToHTMLString(s));
		
	}
	
	private static void escapeString(FormDaxue form){
		String s = null;
		
		s = form.getUniversitydepartment();
		form.setUniversitydepartment(Escape.stringToHTMLString(s));
		
		s = form.getUniversityname();
		form.setUniversityname(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolname();
		form.setHighschoolname(Escape.stringToHTMLString(s));
	}
	
	private static void escapeString(FormBailing form){
		String s = null;
		
		s = form.getCompanyname();
		form.setCompanyname(Escape.stringToHTMLString(s));
		
		s = form.getHomecityname();
		form.setHomecityname(Escape.stringToHTMLString(s));
		
		s = form.getHomeprovince();
		form.setHomeprovince(Escape.stringToHTMLString(s));
		
		s = form.getUniversitydepartment();
		form.setUniversitydepartment(Escape.stringToHTMLString(s));
		
		s = form.getUniversityname();
		form.setUniversityname(Escape.stringToHTMLString(s));
		
	}
	
	private static void escapeString(FormZhongxue form){
		String s = null;
		
		s = form.getHighschoolname();
		form.setHighschoolname(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass1();
		form.setHighschoolclass1(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass2();
		form.setHighschoolclass2(Escape.stringToHTMLString(s));
		
		s = form.getHighschoolclass3();
		form.setHighschoolclass3(Escape.stringToHTMLString(s));
		
		s = form.getJuniorhighschoolname();
		form.setJuniorhighschoolname(Escape.stringToHTMLString(s));
		
		s = form.getTechschoolname();
		form.setTechschoolname(Escape.stringToHTMLString(s));
		
		s = form.getElementaryschoolname();
		form.setElementaryschoolname(Escape.stringToHTMLString(s));
	}
	
	//qita end
	
	private static void escapeString(FillinfoFormBean form) {
		String s = null;
		
		s = form.getCurrentcityname();
		form.setCurrentcityname(Escape.stringToHTMLString(s));
		
		s = form.getCurrentprovince();
		form.setCurrentprovince(Escape.stringToHTMLString(s));
		
		s = form.getHomecityname();
		form.setHomecityname(Escape.stringToHTMLString(s));
		
		s = form.getHomeprovince();
		form.setHomeprovince(Escape.stringToHTMLString(s));
		
		s = form.getWhocanseeme();
		form.setWhocanseeme(Escape.stringToHTMLString(s));
		
	}

	public static void saveCurrentRegionAndNet(User host,FillinfoFormBean form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		RegionInfo region = new RegionInfo();
		region.setProvince_name(form.getCurrentprovince());
		region.setCity_name(form.getCurrentcityname());
		region.setRegion_id(form.getCurrentcitycode());
		region.setUserid(host.getId());
		try {
			DatumInfoClient.getInstance().modifyRegionInfoAndNet(host, region);
			DatumNetworkClient.getInstance().setCurrentNetWork(region.getRegion_id(), host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}

	public static void saveHomeRegion(User host,FillinfoFormBean form){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		MyTimeCost cost = MyTimeCost.begin();
		escapeString(form);
		cost.endStep();
		RegionInfo region = new RegionInfo();
		region.setProvince_name(form.getHomeprovince());
		region.setCity_name(form.getHomecityname());
		region.setRegion_id(form.getHomecitycode());
		region.setUserid(host.getId());
		try {
			UserBorn ub=SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId());
			ub.setHomeProvince(form.getHomeprovince());
			ub.setHomeCity(form.getHomecityname());
			ub.setCityCode(""+form.getHomecitycode());
			SnsAdapterFactory.getUserBornAdapter().setUserBorn(host.getId(), ub);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" | "+" | "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}

	public static void setInitAttributes(HttpServletRequest request){
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		
		A090729Util.setInitAttributes(request);
		
		//看是否是APP引导
		Boolean is_app_guide = false;
		//2009-12-21 15:55:27 查证没有用 去掉
//		try {
//			if(A090729Util.isChannnelUser(host)){
//				is_app_guide = true;
//			}
//		} catch (Exception e) {
//			GuideLogger.printLog(" error "+e.toString(),GuideLogger.ERROR);
//		}
		cost.endStep();
		
		request.setAttribute("is_app_guide", is_app_guide);
		
		WorkplaceInfo work = loadWork(host);
		if(work!=null){
			GuideLogger.printLog(" host:"+host.getId()+work.toString());
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" work null");
		}
		cost.endStep();
		UniversityInfo university = loadUniversity(host);
		if(university!=null){
			GuideLogger.printLog(" host:"+host.getId()+university.toString());
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" university null");
		}
		cost.endStep();
		RegionInfo region = loadRegion(host);
		if(region!=null){
			GuideLogger.printLog(" host:"+host.getId()+region.toString());
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" region null");
		}
		cost.endStep();
		HighSchoolInfo highschool = loadHighschool(host);
		if(highschool!=null){
			GuideLogger.printLog(" host:"+host.getId()+highschool.toString());
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" highschool null");
		}
		cost.endStep();
		JuniorHighSchoolInfo juniorschool = loadJuniorhighschool(host);
		if(juniorschool!=null){
			GuideLogger.printLog(" host:"+host.getId()+juniorschool.toString());
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" juniorschool null");
		}
		cost.endStep();
		ElementarySchoolInfo elementaryschool = loadElementaryschool(host);
		if(elementaryschool!=null){
			GuideLogger.printLog(" host:"+host.getId()+elementaryschool.toString());
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" elementaryschool null");
		}
		cost.endStep();
		CollegeInfo college = loadCollege(host);
		cost.endStep();
		
		//Bailing start
		FormBailing fBL = new FormBailing();
		if(work != null){
			fBL.setCompanyname(work.getWorkplaceName());
			fBL.setHomecitycode(work.getCityId());
			fBL.setHomecityname(work.getCityName());
			fBL.setHomeprovince(work.getProvince());
		}
		else{
			fBL.setCompanyname("请填写公司.");
		}
		if(region != null){
			fBL.setHomeprovince(region.getProvince_name());
			fBL.setHomecityname(region.getCity_name());
			fBL.setHomecitycode(region.getRegion_id());
		}
		else{
			
		}
		if(university != null ){
			fBL.setUniversitycode(university.getId());
			fBL.setUniversitydepartment(university.getDepartment());
			fBL.setUniversityname(university.getUniversityName());
			fBL.setUniversityyear(university.getEnrollYear());
		}
		else{
			fBL.setUniversityname("请填写大学.");
		}
		request.setAttribute("fBL", fBL);
		//Bailing end
		
		//Qita start
		FormQita fQT = new FormQita();
		if(region != null){
			fQT.setHomeprovince(region.getProvince_name());
			fQT.setHomecityname(region.getCity_name());
			fQT.setHomecitycode(region.getRegion_id());
		}
		else{
			
		}
		if(university != null){
			fQT.setUniversitycode(university.getId());
			fQT.setUniversitydepartment(university.getDepartment());
			fQT.setUniversityname(university.getUniversityName());
			fQT.setUniversityyear(university.getEnrollYear());
		}
		else{
			fQT.setUniversityname("请填写大学.");
		}
		request.setAttribute("fQT", fQT);
		//Qita end
		
		//Daxue start
		FormDaxue fDX = new FormDaxue();
		if(university != null){
			fDX.setUniversitycode(university.getId());
			fDX.setUniversitydepartment(university.getDepartment());
			fDX.setUniversityname(university.getUniversityName());
			fDX.setUniversityyear(university.getEnrollYear());
		}
		else{
			fDX.setUniversityname("请填写大学.");
		}
		if(highschool != null){
			fDX.setHighschoolcode(highschool.getHighSchoolId());
			fDX.setHighschoolname(highschool.getHighSchoolName());
			fDX.setHighschoolyear(highschool.getEnrollYear());
		}
		else{
			fDX.setHighschoolname("请填写高中.");
		}
		request.setAttribute("fDX", fDX);
		//Daxue end
		
		//Zhongxue start
		FormZhongxue fZX = new FormZhongxue();
		if(highschool != null){
			fZX.setHighschoolcode(highschool.getHighSchoolId());
			fZX.setHighschoolname(highschool.getHighSchoolName());
			fZX.setHighschoolyear(highschool.getEnrollYear());
			fZX.setHighschoolclass1(highschool.getHClass1());
			fZX.setHighschoolclass2(highschool.getHClass2());
			fZX.setHighschoolclass3(highschool.getHClass3());
		}
		else{
			fZX.setHighschoolname("请填写高中.");
		}
		if(juniorschool !=null ){
			fZX.setJuniorhighschoolcode(juniorschool.getJunHighentarySchoolId());
			fZX.setJuniorhighschoolname(juniorschool.getJunHighentarySchoolName());
			fZX.setJuniorhighschoolyear(juniorschool.getJunHighentarySchoolYear());
		}
		else{
			fZX.setJuniorhighschoolname("请填写初中.");
		}
		if(elementaryschool != null){
			fZX.setElementaryschoolcode(elementaryschool.getElementarySchoolId());
			fZX.setElementaryschoolname(elementaryschool.getElementarySchoolName());
			fZX.setElementaryschoolyear(elementaryschool.getElementarySchoolYear());
		}
		else{
			fZX.setElementaryschoolname("请填写小学.");
		}
		if(college != null){
			fZX.setTechschoolcode(college.getCollegeId());
			fZX.setTechschoolname(college.getCollegeName());
			fZX.setTechschoolyear(college.getEnrollYear());
		}
		else{
			fZX.setTechschoolname("请填写中专/技校.");
		}
		request.setAttribute("fZX", fZX);
		//Zhongxue end 
		cost.endFinally();
		
		//结束设置
		GuideLogger.printLog(" -- end ");
	}






}
