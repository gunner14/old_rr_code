package com.xiaonei.reg.guide.flows.xfive.utils;

import java.util.List;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.datum.client.DatumNetworkClient;
import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;
import com.xiaonei.websecurity.adapter.SafeFilter;

public class GuideXFiveUserProfileUtil {
	
	public static void asyncAddUniversity(final User host,final UniversityInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					addUniversity(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	

	public static void asyncSaveCurrentRegion(final User host, final RegionInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveCurrentRegion(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	private static void saveCurrentRegion(User host, RegionInfo info){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		String msg = "";
		try {
			filter(info);
			DatumInfoClient.getInstance().modifyRegionInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg+info.getProvince_name()+ info.getCity_name()+ ""+info.getRegion_id());
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void filter(RegionInfo info) {
		//com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent()
		if(info==null)return;
		info.setCity_name(com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent(info.getCity_name()));
		info.setProvince_name(com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent(info.getProvince_name()));
		
	}



	public static void asyncSaveCurrentRegionAndNet(final User host, final RegionInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveCurrentRegionAndNet(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	private static void saveCurrentRegionAndNet(User host, RegionInfo info){
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.ERROR);
			return;
		}
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" info null!",GuideLogger.ERROR);
			return;
		}
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			DatumInfoClient.getInstance().modifyRegionInfoAndNet(host, info);
			DatumNetworkClient.getInstance().setCurrentNetWork(info.getRegion_id(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg+info.getProvince_name()+ info.getCity_name()+ ""+info.getRegion_id());
		} 
		catch (IllegalArgumentException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" info:"+info.toString(),GuideLogger.ERROR);
		}
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	public static void asyncAddWork(final User host, final WorkplaceInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					addWork(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public static void asyncUpdateWork(final User host, final WorkplaceInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					updateWork(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	
	private static void updateWork(User host, WorkplaceInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			List<WorkplaceInfo> list = DatumInfoClient.getInstance().getWorkplaceInfo(host);
			if(list == null || list.size() == 0){
				msg = DatumInfoClient.getInstance().modifyWorkplaceInfo(host, info);
			}
			else{
				WorkplaceInfo oldinfo = list.get(list.size()-1);
				msg = DatumInfoClient.getInstance().setWorkInfo(oldinfo.getId(), info);
			}
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void filter(WorkplaceInfo info) {
		if(info == null)return;
		info.setWorkplaceName(SafeFilter.getInstance().getPureContent(info.getWorkplaceName()));
		
	}



	public static void asyncUpdateUniversity(final User host, final UniversityInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					updateUniversity(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	private static void updateUniversity(User host, UniversityInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			List<UniversityInfo> list = DatumInfoClient.getInstance().getUniversityInfos(host);
			if(list == null || list.size() == 0){
				msg = DatumInfoClient.getInstance().modifyUniversityInfo(host, info);
			}
			else{
				UniversityInfo oldinfo = list.get(list.size()-1);
				msg = DatumInfoClient.getInstance().setUniversityInfo(oldinfo.getId(), info);
			}
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	private static void filter(UniversityInfo info) {
		if(info == null)return;
		info.setDepartment(SafeFilter.getInstance().getPureContent(info.getDepartment()));
		info.setTypeOfCourse(SafeFilter.getInstance().getPureContent(info.getTypeOfCourse()));
		info.setUniversityName(SafeFilter.getInstance().getPureContent(info.getUniversityName()));
		info.setDorm(SafeFilter.getInstance().getPureContent(info.getDorm()));
		
	}


	private static void addWork(User host, WorkplaceInfo info) {
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.ERROR);
			return;
		}
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" info null!",GuideLogger.ERROR);
			return;
		}
		
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			msg = DatumInfoClient.getInstance().modifyWorkplaceInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} 
		catch (IllegalArgumentException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" info:"+info.toString(),GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void addUniversity(User host, UniversityInfo info) {
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.ERROR);
			return;
		}
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" info null!",GuideLogger.ERROR);
			return;
		}
		
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			msg = DatumInfoClient.getInstance().modifyUniversityInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		}
		catch (IllegalArgumentException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" info:"+info.toString(),GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}

	public static void asyncSetUniversityNet(final User host, final UniversityInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					setUniversityNet(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	private static void setUniversityNet(User host, UniversityInfo info) {
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.ERROR);
			return;
		}
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" info null!",GuideLogger.ERROR);
			return;
		}
		
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			DatumNetworkClient.getInstance().removeNetworkByStage(host, Stage.STAGE_UNIV);
			msg = DatumNetworkClient.getInstance().addOneUnivNetwork(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(info.getUniversityId(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - 2");
		}
		catch (IllegalArgumentException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" info:"+info.toString(),GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	public static void asyncSetHighschoolNet(final User host, final HighSchoolInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					setHighschoolNet(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	private static void setHighschoolNet(User host, HighSchoolInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			DatumNetworkClient.getInstance().removeNetworkByStage(host, Stage.STAGE_HEIGHSCHOOL);
			msg = DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(info.getHighSchoolId(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - 2");
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void filter(HighSchoolInfo info) {
		if(info == null)return;
		info.setHighSchoolName(SafeFilter.getInstance().getPureContent(info.getHighSchoolName()));
		info.setHClass1(SafeFilter.getInstance().getPureContent(info.getHClass1()));
		info.setHClass2(SafeFilter.getInstance().getPureContent(info.getHClass2()));
		info.setHClass3(SafeFilter.getInstance().getPureContent(info.getHClass3()));
	}


	public static void asyncSetJuniorschoolNet(final User host, final JuniorHighSchoolInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					setJuniorschoolNet(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	private static void setJuniorschoolNet(User host, JuniorHighSchoolInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			DatumNetworkClient.getInstance().removeNetworkByStage(host, Stage.STAGE_HEIGHSCHOOL);
			msg = DatumNetworkClient.getInstance().addOneJunSchoolNetwork(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(info.getJunHighentarySchoolId(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - 2");
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void filter(JuniorHighSchoolInfo info) {
		if(info == null)return;
		info.setJunHighentarySchoolName(info.getJunHighentarySchoolName());
	}


	public static void asyncSetTechschoolNet(final User host, final CollegeInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					setTechschoolNet(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}

	private static void setTechschoolNet(User host, CollegeInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			DatumNetworkClient.getInstance().removeNetworkByStage(host, Stage.STAGE_HEIGHSCHOOL);
			msg = DatumNetworkClient.getInstance().addOneCollSchoolNetwork(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(info.getCollegeId(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - 2");
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void filter(CollegeInfo info) {
		if(info == null)return;
		info.setCollegeName(SafeFilter.getInstance().getPureContent(info.getCollegeName()));
		info.setDepartment(SafeFilter.getInstance().getPureContent(info.getDepartment()));
	}


	@SuppressWarnings("unused")
	private static void asyncSetRegionNet(final User host, final RegionInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					setRegionNet(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	private static void setRegionNet(User host, RegionInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			DatumNetworkClient.getInstance().removeNetworkByStage(host, Stage.STAGE_NONE);
			msg = DatumNetworkClient.getInstance().addOneRegionNetwork(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
			DatumNetworkClient.getInstance().setCurrentNetWork(info.getRegion_id(), host);
			GuideLogger.printLog(" host:"+host.getId()+" - 2");
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	
	private static UserBasic getUserBasic(int userId) {
		UserBasic ub = null;
		try {
			ub = SnsAdapterFactory.getUserBasicAdapter().get(userId);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+userId+" "+e.toString(),GuideLogger.ERROR);
		}
		return ub;
	}

	public static void asyncSaveHighschool(final User host, final HighSchoolInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveHighschool(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	private static void saveHighschool(User host, HighSchoolInfo info) {
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.ERROR);
			return;
		}
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		if(info == null){
			GuideLogger.printLog(" host:"+host.getId()+" info null!",GuideLogger.ERROR);
			return;
		}
		GuideTimeCost cost = GuideTimeCost.begin();
		String msg = "";
		try {
			filter(info);
			msg = DatumInfoClient.getInstance().modifyHighSchoolInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		}
		catch (IllegalArgumentException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString()+" info:"+info.toString(),GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
		
	}


	private static void saveHomeRegion(User host, RegionInfo info){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			UserBorn ub=SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId());
            ub.setHomeProvince(info.getProvince_name());
            ub.setHomeCity(info.getCity_name());
            ub.setCityCode(""+info.getRegion_id());
            SnsAdapterFactory.getUserBornAdapter().setUserBorn(host.getId(), ub);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg+info.getProvince_name()+ info.getCity_name()+ ""+info.getRegion_id());
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	public static void asyncSaveHomeRegion(final User host, final RegionInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveHomeRegion(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	private static void saveJuniorschool(User host,	JuniorHighSchoolInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			msg = DatumInfoClient.getInstance().modifyJuniorHighSchoolInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}

	public static void asyncSaveJuniorschool(final User host,	final JuniorHighSchoolInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveJuniorschool(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}

	public static void asyncSaveTechschool(final User host, final CollegeInfo info) {
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveTechschool(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}

	private static void saveTechschool(User host, CollegeInfo info) {
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			msg = DatumInfoClient.getInstance().modifyCollegeInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
		
	}
	public static void asyncSaveElementary(final User host, final ElementarySchoolInfo info){
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					GuideTimeCost cost = GuideTimeCost.begin();
					saveElementary(host,info);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( ""+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	private static void saveElementary(User host, ElementarySchoolInfo info){
		GuideLogger.printLog(" host:"+host.getId()+" - start");
		GuideTimeCost cost = GuideTimeCost.begin();
		
		String msg = "";
		try {
			filter(info);
			msg = DatumInfoClient.getInstance().modifyElementarySchoolInfo(host, info);
			GuideLogger.printLog(" host:"+host.getId()+" - "+msg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
	}
	
	private static void filter(ElementarySchoolInfo info) {
		if(info == null)return;
		info.setElementarySchoolName(SafeFilter.getInstance().getPureContent(info.getElementarySchoolName()));
	}


	public static UniversityInfo loadUniversity(User host){
		GuideTimeCost cost = GuideTimeCost.begin();
		cost.endStep();
		UniversityInfo university = null;
		try {
			List<UniversityInfo> universitylist = DatumInfoClient.getInstance().getUniversityInfos(host);
			if(universitylist == null || universitylist.size() == 0){
				GuideLogger.printLog(" - universitylist null or size 0"+host.getId());
			}
			else{
				university = universitylist.get(universitylist.size()-1);
				GuideLogger.printLog(" host:"+host.getId()+" - load univ:"+university.getUniversityName());
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return university;
	}


	public static CollegeInfo loadTechschool(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		CollegeInfo info = null;
		cost.endStep();
		
		try {
			List<CollegeInfo> collegelist = DatumInfoClient.getInstance().getCollegeInfos(host);
			if(collegelist == null || collegelist.size() == 0){
				GuideLogger.printLog(" - collegelist null or size 0"+host.getId());
			}
			else{
				info = collegelist.get(collegelist.size()-1);
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}


	public static ElementarySchoolInfo loadElementary(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		ElementarySchoolInfo info = null;
		cost.endStep();
		
		try {
			info = DatumInfoClient.getInstance().getElementarySchoolInfos(host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}


	public static HighSchoolInfo loadHighschool(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		HighSchoolInfo info = null;
		cost.endStep();
		
		try {
			List<HighSchoolInfo> list  = DatumInfoClient.getInstance().getHighSchoolInfos(host);
			if(list == null || list.size() == 0){
				GuideLogger.printLog(" - collegelist null or size 0"+host.getId());
			}
			else{
				info = list.get(list.size()-1);
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}


	public static JuniorHighSchoolInfo loadJuniorschool(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		JuniorHighSchoolInfo info = null;
		cost.endStep();
		
		try {
			info = DatumInfoClient.getInstance().getJuniorHighSchoolInfos(host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}


	public static WorkplaceInfo loadWorkplace(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		WorkplaceInfo info = null;
		cost.endStep();
		
		try {
			List<WorkplaceInfo> list  = DatumInfoClient.getInstance().getWorkplaceInfo(host);
			if(list == null || list.size() == 0){
				GuideLogger.printLog(" - collegelist null or size 0"+host.getId());
			}
			else{
				info = list.get(list.size()-1);
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}


	public static RegionInfo loadHomeRegion(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		RegionInfo info = null;
		cost.endStep();
		
		try {
			//TODO XXXXXXXXXXX
			UserBasic ub = getUserBasic(host.getId());
			info = new RegionInfo();
			info.setProvince_name(ub.getHomeProvince());
			info.setCity_name(ub.getHomeCity());
			
			info.setUserid(host.getId());
			
			
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
		
	}


	public static RegionInfo loadCurrentRegion(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		RegionInfo info = null;
		cost.endStep();
		
		try {
			info = DatumInfoClient.getInstance().getRegionInfo(host);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" - "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return info;
	}
	
	public static String toStringForLog(CollegeInfo info){
		if(info == null) return "";
		return info.getCollegeId()+" "+info.getCollegeName()+" "+info.getEnrollYear();
	}
	
	public static String toStringForLog(UniversityInfo info){
		if(info == null) return "";
		return info.getUniversityId()+" "+info.getUniversityName()+" "+info.getEnrollYear();
	}
	public static String toStringForLog(HighSchoolInfo info){
		if(info == null) return "";
		return info.getHighSchoolId()+" "+info.getHighSchoolName()+" "+info.getEnrollYear()+" "+info.getHClass1()+" "+info.getHClass2()+" "+info.getHClass3();
	}
	public static String toStringForLog(JuniorHighSchoolInfo info){
		if(info == null) return "";
		return info.getJunHighentarySchoolId()+" "+info.getJunHighentarySchoolName()+" "+info.getJunHighentarySchoolYear();
	}
	public static String toStringForLog(ElementarySchoolInfo info){
		if(info == null) return "";
		return info.getElementarySchoolId()+" "+info.getElementarySchoolName()+" "+info.getElementarySchoolYear();
	}
	public static String toStringForLog(RegionInfo info){
		if(info == null) return "";
		return info.getRegion_id()+" "+info.getProvince_name()+" "+info.getCity_name();
	}
	public static String toStringForLog(WorkplaceInfo info){
		if(info == null) return "";
		return info.getWorkplaceId()+" "+info.getWorkplaceName();
	}

}
