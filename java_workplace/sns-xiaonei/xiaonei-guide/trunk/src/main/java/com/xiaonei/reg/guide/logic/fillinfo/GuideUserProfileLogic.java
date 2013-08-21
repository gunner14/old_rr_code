package com.xiaonei.reg.guide.logic.fillinfo;

import mop.hi.oce.domain.network.UniversityInfo;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.a090821.UserProfileUtil;
import com.xiaonei.reg.guide.action.netbarflow.FillinfoFormBean;
import com.xiaonei.reg.guide.form.f090729.FormBailing;
import com.xiaonei.reg.guide.form.f090729.FormDaxue;
import com.xiaonei.reg.guide.form.f090729.FormQita;
import com.xiaonei.reg.guide.form.f090729.FormZhongxue;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckUniversity;


public class GuideUserProfileLogic {
	static class SingletonHolder {
		static GuideUserProfileLogic instance = new GuideUserProfileLogic();
	}

	public static GuideUserProfileLogic getInstance() {
		return SingletonHolder.instance;
	}
	

	public void saveWork(User host, FormBailing fform){
		GuideLogger.printLog("[FormBailing] - host:"+ host.getId()+ " - com:"+fform.getCompanyname());
		try {
			UserProfileUtil.saveWork(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormBailing] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public void asyncSaveUniversity(final User host, final FormBailing fform) {
		GuideLogger.printLog( "[FormBailing] - host:"+ host.getId()+ " - com:"+fform.getUniversityname()+" - start");
		try {
			Runnable taskSave_University = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					String chk = new CheckUniversity().checkUniversity("1", fform.getUniversitycode(), fform.getUniversityname());
					if(StringUtils.equals(chk, "OK") || StringUtils.equals(chk, "请填写大学（选填）")){
						UserProfileUtil.saveUniversity(host, fform);
					}
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave_University);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormBailing] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public void asyncSaveRegionAndNet(final User host, final FormBailing fform){
		GuideLogger.printLog( "[FormBailing] - host:"+ host.getId()+ " - com:"+fform.getHomecityname()+" - start");
		try {
			RegionInfo region = new RegionInfo();
			region.setProvince_name(fform.getHomeprovince());
			region.setCity_name(fform.getHomecityname());
			region.setRegion_id(fform.getHomecitycode());
			region.setUserid(host.getId());
			GuideUserProfileCacheLogic.getInstance().saveUserRegion(host, region);
			Runnable taskSave_Region = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					if(checkCityNet(fform)){ 
						UserProfileUtil.saveRegionAndNet(host, fform);
					}
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave_Region);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormBailing] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	public void saveHighschoolAndNet(final User host, final FormZhongxue fform){
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getHighschoolname()+" - start");
		try {
			UserProfileUtil.saveHighschoolAndNet(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	public void asyncSaveHighschool(final User host, final FormZhongxue fform){
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getHighschoolname()+" - start");
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveHighschool(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public void asyncSaveJuniorschool(final User host, final FormZhongxue fform){
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getJuniorhighschoolname()+" - start");
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveJuniorschool(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	public void asyncSaveElementaryschool(final User host, final FormZhongxue fform) {
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getElementaryschoolname()+" - start");
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveElementaryschool(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	public void asyncSaveHighschoolNet(final User host, final FormZhongxue fform){
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getHighschoolname()+" - start");
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveHighschoolNet(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	//2009-10-21 11:06:19,MyLogger.ERROR上
	//异步存初中网络的
	public void asyncSaveJuniorschoolNet(final User host, final FormZhongxue fform){
		GuideLogger.printLog("[FormZhongxue]  - host:"+ host.getId()+ " - com:"+fform.getJuniorhighschoolname()+" - "+fform.getJuniorhighschoolcode()+" - start");
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveJuniorschoolNet(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue]  - host"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	//2009-10-21 11:06:19 加上
	//异步存技校网络的
	public void asyncSaveCollegeNet(final User host, final FormZhongxue fform){
		GuideLogger.printLog("[FormZhongxue]  - host:"+ host.getId()+ " - com:"+fform.getTechschoolname()+" - "+fform.getTechschoolcode()+" - start");
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveCollegeNet(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue]  - host"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public void saveHighschool(User host, FormZhongxue fform) {
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getHighschoolname()+" - start");
		try {
			UserProfileUtil.saveHighschool(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	
	public void saveJuniorschool(User host, FormZhongxue fform) {
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getJuniorhighschoolname()+" - start");
		try {
			UserProfileUtil.saveJuniorschool(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	public void saveCollege(User host, FormZhongxue fform) {
		GuideLogger.printLog( "[FormZhongxue] - host:"+ host.getId()+ " - com:"+fform.getTechschoolname()+" - start");
		try {
			UserProfileUtil.saveCollege(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormZhongxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	/*
	 * 下面是大学引导的部分
	 */
	public void saveUniversity(User host, FormDaxue fform) {
		GuideLogger.printLog( "[FormDaxue] - host:"+ host.getId()+ " - univ:"+fform.getUniversityname()+" - start");
		try {
			//2009-11-4 14:22:13 加上，看看大学网络是否真的已经有了
			UniversityInfo oldinfo = UserProfileUtil.loadUniversity(host);
			if(oldinfo != null){ //有老资料了，怎么办……
				GuideLogger.printLog("[FormDaxue] - host:"+ host.getId()+" has old university!",GuideLogger.WARN);
				if(mayEqual(host,oldinfo,fform)){//如果有相同的资料，打个log，就不存了
					GuideLogger.printLog("[FormDaxue] - host:"+ host.getId()+" has a SAME university! "+oldinfo.getId(),GuideLogger.WARN);
				}
				else{ //有老资料了，但是却不同。存吧
					UserProfileUtil.saveUniversity(host, fform);
				}
			}
			else{ //没有老资料。存吧
				UserProfileUtil.saveUniversity(host, fform);
			}
		} catch (Exception e) {
			GuideLogger.printLog( "[FormDaxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	private boolean mayEqual(User host, UniversityInfo oldinfo, FormDaxue fform) {
		try {
			if(host == null){
				GuideLogger.printLog( "[FormDaxue] error - host is null!",GuideLogger.ERROR);
				return false;
			}
			if(oldinfo == null || fform == null || oldinfo.getUniversityName() == null || fform.getUniversityname() == null){
				return false;
			}
			else if(oldinfo.getId() == fform.getUniversitycode() && oldinfo.getUniversityName().trim().equals(fform.getUniversityname().trim())){
				return true;
			}
		} catch (Exception e) {
			GuideLogger.printLog( "[FormDaxue] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return false;
	}


	public void asyncSaveUniversityNet(final User host, final FormDaxue fform) {
		GuideLogger.printLog( "[FormDaxue] - host:"+ host.getId()+ " - com:"+fform.getUniversityname()+" - start");
		
		try {
			Runnable taskSave = new Runnable() {
				public void run() {
					//2009-11-4 14:22:13 加上，看看大学网络是否真的已经有了
					UserProfileUtil.loadUniversityNetwork(host);
					MyTimeCost cost = MyTimeCost.begin();
					UserProfileUtil.saveUniversityNet(host, fform);
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormDaxue]  - host"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}


	public void asyncSaveHighschoolAndNet(final User host, final FormDaxue fform) {
		GuideLogger.printLog( "[FormDaxue] - host:"+ host.getId()+ " - com:"+fform.getHighschoolname()+" - start");
		Runnable taskSaveMS = new Runnable() {
			public void run() {
				MyTimeCost cost = MyTimeCost.begin();
				boolean cansavems = checkMsNet(fform);
				cost.endStep();
				if(cansavems){
					UserProfileUtil.saveHighschool(host, fform);
					cost.endStep();
					UserProfileUtil.saveHighschoolNet(host, fform);
					cost.endStep();
				}
				cost.endStep();
				GuideLogger.printLog(" host:"+host.getId());
				cost.endFinally();
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSaveMS);
	}


	/*
	 * 下面是网吧引导的部分
	 */
	public void asyncSaveCurrentRegionAndNet(final User host, final FillinfoFormBean fform){
		GuideLogger.printLog( "[FillinfoFormBean] - host:"+ host.getId()+ " - com:"+fform.getCurrentcityname()+" - start");
		Runnable taskSaveMS = new Runnable() {
			public void run() {
				try {
					UserProfileUtil.saveCurrentRegionAndNet(host, fform);
				} catch (Exception e) {
					GuideLogger.printLog( "[FillinfoFormBean] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSaveMS);
	}
	
	public void saveCurrentRegionAndNet(User host, FillinfoFormBean fform){
		GuideLogger.printLog( "[FillinfoFormBean] - host:"+ host.getId()+ " - com:"+fform.getCurrentcityname()+" - start");
		try {
			UserProfileUtil.saveCurrentRegionAndNet(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FillinfoFormBean] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	public void saveHomeRegion(User host, FillinfoFormBean fform){
		GuideLogger.printLog( "[FillinfoFormBean] - host:"+ host.getId()+ " - com:"+fform.getHomecityname()+" - start");
		try {
			UserProfileUtil.saveHomeRegion(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FillinfoFormBean] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	public void saveStage(User host, int stage) {
		GuideLogger.printLog( " - host:"+ host.getId()+ " - stage:"+stage);
		try {
			FillInfoLogic.getInstance().saveStage(host, stage);
			
		} catch (Exception e) {
			GuideLogger.printLog( " error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}
	
	/*
	 * 下面是本类用的私有方法
	 */
	
	/**
	 * 判断时候可以加入地域网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkCityNet(FormBailing fform) {
		GuideLogger.printLog(" - start");
		if (StringUtils.isNotEmpty(fform.getHomecityname())
				|| StringUtils.isNotEmpty(fform.getHomeprovince())) {
			GuideLogger.printLog(" - end true");
			return true;
		}
		GuideLogger.printLog(" - end false");
		return false;
	}
	
	/**
	 * 判断时候可以加入中学网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkMsNet(FormDaxue fform) {
		if(StringUtils.isNotEmpty(fform.getHighschoolname()))
				return true;
		return false;
	}
	
	
	/**
	 * 判断时候可以加入地域网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkCityNet(FormQita fform) {
		if (StringUtils.isNotEmpty(fform.getHomecityname())
				|| StringUtils.isNotEmpty(fform.getHomeprovince())) {
			return true;
		}
		return false;
	}


	public void saveUniversity(User host, FormQita fform) {
		GuideLogger.printLog( "[FormQita] - host:"+ host.getId()+ " - com:"+fform.getUniversityname()+" - start");
		try {
			UserProfileUtil.saveUniversity(host, fform);
		} catch (Exception e) {
			GuideLogger.printLog( "[FormQita] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}


	public void saveRegionAndNet(User host, FormQita fform) {
		GuideLogger.printLog( "[FormQita] - host:"+ host.getId()+ " - com:"+fform.getHomecityname()+" - start");
		try {
			if(checkCityNet(fform)){ 
				RegionInfo region = new RegionInfo();
				region.setProvince_name(fform.getHomeprovince());
				region.setCity_name(fform.getHomecityname());
				region.setRegion_id(fform.getHomecitycode());
				region.setUserid(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserRegion(host, region);

				UserProfileUtil.saveRegionAndNet(host, fform);
			}
		} catch (Exception e) {
			GuideLogger.printLog( "[FormQita] error - host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}

	}


}
