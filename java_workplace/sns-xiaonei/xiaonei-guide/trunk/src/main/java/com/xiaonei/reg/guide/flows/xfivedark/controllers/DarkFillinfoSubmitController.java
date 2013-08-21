package com.xiaonei.reg.guide.flows.xfivedark.controllers;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserNameLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.interfaces.IXProfilable;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path({"fidarks","fidarks-bl","fidarks-qt"})
@IgnoreUserStatusImportedValidation
public class DarkFillinfoSubmitController {
	
	//private final static String INIT = "f:fillinfo";
	private final int INT_THIS_STEP = IStepStatusDefinition.STEP_FILLINFO;
	public final static String SUCC = "f:lead";
	
	private final static String info_dx = "info_dx";
	private final static String info_gz = "info_gz";
	private final static String info_jx = "info_jx";
	private final static String info_cz = "info_cz";
	//private final static String info_xx = "info_xx";

	private final static String ERROR_TOINIT = "r:fillinfo?e=";
	
	@Get
	public String get(Invocation inv,CoralFormInfo form){
		return "r:lead";
	}
	
	@Post
	public String index(Invocation inv,CoralFormInfo form){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		String ret = SUCC;
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			boolean isdone = GuideXFiveStepCheckLogic.getInstance().isTheStepDone(INT_THIS_STEP, host);
			if(isdone){
				GuideLogger.printLog(" host:"+host.getId()+" refill",GuideLogger.WARN);
			}
			else{
				String presave = GuideXFiveUserNameLogic.saveUserNameAndGendar(host,inv);
				if("OK".equals(presave)){
					GuideLogger.printLog(" host:"+ host.getId()+ " savenameok");
				}
				else{
					GuideLogger.printLog(" host:"+ host.getId()+ " savenamee:"+presave);
					return ERROR_TOINIT+presave;
				}
				
				
				int userstage = form.getStage();
				GuideXFiveUseIdentityLogic.getInstance().saveIdentity(host.getId(), userstage);
				
				Map<String,IXProfilable> param =  new HashMap<String, IXProfilable>();
				String checkrst = "";
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAUniversity(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //大学
					param.put("info_dx", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //高中
					param.put("info_gz", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //初中 
					param.put("info_cz", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isATechschool(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //技校
					param.put("info_jx", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAWorkInfo(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //公司
					param.put("info_gs", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //小学
					param.put("info_xx", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isACurrentPlaceInfo(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //所在地
					param.put("info_dd", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAHomePlaceInfo(form);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //家乡
					param.put("info_gx", form);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				
				int savestage = userstage;
				switch(userstage){
				case Stage.STAGE_HEIGHSCHOOL:
					//saveZhongxuesheng();
					break;
					
				case GuideXFiveUseIdentityLogic.IDENTITY_JUNIORHIGHSCHOOL:
					doSaveJuni(host,form);
					savestage =  Stage.STAGE_HEIGHSCHOOL;
					break;
				case GuideXFiveUseIdentityLogic.IDENTITY_SENIORHIGHSCHOOL:
					//doSaveHigh(host,form);
					savestage =  Stage.STAGE_HEIGHSCHOOL;
					break;
				case GuideXFiveUseIdentityLogic.IDENTITY_TECHNOLOGYSCHOOL:
					//doSaveTech(host,form);
					savestage =  Stage.STAGE_HEIGHSCHOOL;
					break;
				case Stage.STAGE_UNIV:
					//doSaveUniv(host,form);
					savestage = Stage.STAGE_UNIV;
					break;
				case Stage.STAGE_MAJOR:
					//doSaveMajo(host,form);
					savestage = Stage.STAGE_MAJOR;
					break;
				case Stage.STAGE_NONE:
					//doSaveNone(host,form);
					savestage = chageNoneStage(host,form,param);
					//savestage = Stage.STAGE_NONE;
					break;
				default:
					GuideLogger.printLog(" host:"+host.getId()+"stage:"+userstage,GuideLogger.WARN);
				break;
				}
				doSaveAll(host,form,savestage,param);
				
				
				
				try {
					//改标志
					FillInfoLogic.getInstance().saveStage(host, savestage);
					StepStatusLogic.updateStepStatus(host.getId(), INT_THIS_STEP);
					GuideLogger.printLog(" host:"+host.getId()+" update step:"+INT_THIS_STEP);
					
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		
		if("l".equals(request.getParameter("f")) && SUCC.equals(ret)){
			ret = "r:lead";
		}
		return ret;
	}

	private void doSaveAll(User host, CoralFormInfo form, int savestage, Map<String, IXProfilable> param) {
		doSaveAllInfo(host, form);
		doSaveNetwork(host,form,savestage,param);
	}

	private void doSaveNetwork(User host, CoralFormInfo form, int savestage, Map<String, IXProfilable> param) {
		switch(savestage){
			case Stage.STAGE_HEIGHSCHOOL:
				if(param.containsKey(info_gz)){
					GuideXFiveUserInfoLogic.getInstance().saveHighschoolNet(host, form);
				}
				else if(param.containsKey(info_jx)){
					GuideXFiveUserInfoLogic.getInstance().saveCollegeNet(host, form);
				}
				else if(param.containsKey(info_cz)){
					GuideXFiveUserInfoLogic.getInstance().saveJuniorschoolNet(host, form);
				}
				break;
			case Stage.STAGE_UNIV:
				GuideXFiveUserInfoLogic.getInstance().saveUniversityNet(host, form);
				break;
			case Stage.STAGE_MAJOR:
				GuideXFiveUserInfoLogic.getInstance().saveCurrentRegionNet(host, form);
				break;
			case Stage.STAGE_NONE:
				GuideXFiveUserInfoLogic.getInstance().saveCurrentRegionNet(host, form);
				break;
			default:
				GuideLogger.printLog(" host:"+host.getId()+"stage:"+savestage,GuideLogger.WARN);
			break;
		}
		
	}

	private int chageNoneStage(User host, CoralFormInfo form, Map<String, IXProfilable> param) {
		int ret = 90;
		try {
			if(param.containsKey(info_dx)){
				if(MyStringUtils.contains(form.getCoursetype(),"本科")){
					if(getCurrentYear() - form.getUniversityyear() <= 4){
						ret = Stage.STAGE_UNIV;
						GuideLogger.printLog(" host:"+host.getId()+" 1");
					}
				}
				else if(MyStringUtils.contains(form.getCoursetype(),"硕士")){
					if(getCurrentYear() - form.getUniversityyear() <= 3){
						ret = Stage.STAGE_UNIV;
						GuideLogger.printLog(" host:"+host.getId()+" 2");
					}
				}
				else{
					if(getCurrentYear() - form.getUniversityyear() <= 4){
						ret = Stage.STAGE_UNIV;
						GuideLogger.printLog(" host:"+host.getId()+" 3");
					}
				}
			}
			else if(param.containsKey(info_gz)){
				if(getCurrentYear() - form.getHighschoolyear() <= 3){
					ret = Stage.STAGE_HEIGHSCHOOL;
					GuideLogger.printLog(" host:"+host.getId()+" 4");
				}
			}
			else if(param.containsKey(info_jx)){
				if(getCurrentYear() - form.getTechschoolyear() <= 3){
					ret = Stage.STAGE_HEIGHSCHOOL;
					GuideLogger.printLog(" host:"+host.getId()+" 5");
				}
			}
			else if(param.containsKey(info_cz)){
				if(getCurrentYear() - form.getJuniorhighschoolyear() <= 4){
					ret = Stage.STAGE_HEIGHSCHOOL;
					GuideLogger.printLog(" host:"+host.getId()+" 6");
				}
			}
			else {
				//别的不管
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		GuideLogger.printLog(" host:"+host.getId()+" stagechangeto:"+ret);
		return ret;
	}
	
	private int getCurrentYear(){
		int ret = 0;
		try {
			Date currenttime = new Date();
			SimpleDateFormat formatyear = new SimpleDateFormat("yyyy");
			String year = formatyear.format(currenttime);
			ret = Integer.parseInt(year);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return ret;
	}

	public static void main(String[] args) {
		Date currenttime = new Date();
		SimpleDateFormat formatyear = new SimpleDateFormat("yyyy");
		String year = formatyear.format(currenttime);
		System.out.println(year);
		
	}

	private void doSaveAllInfo(User host,CoralFormInfo form){
		String checkrst = "";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isACurrentPlaceInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //所在地
			//GuideXFiveUserInfoLogic.getInstance().saveCurrentRegionNet(host, form);
			GuideXFiveUserInfoLogic.getInstance().saveCurrentRegion(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAHomePlaceInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //家乡
			GuideXFiveUserInfoLogic.getInstance().saveHomeRegion(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAWorkInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //公司
			GuideXFiveUserInfoLogic.getInstance().saveWork(host, form);
			
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		//学校们
		
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAUniversity(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //大学
			GuideXFiveUserInfoLogic.getInstance().addUniversity(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //高中
			GuideXFiveUserInfoLogic.getInstance().saveHighschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //初中 
			GuideXFiveUserInfoLogic.getInstance().saveJuniorschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isATechschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //技校
			GuideXFiveUserInfoLogic.getInstance().saveCollege(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //小学
			GuideXFiveUserInfoLogic.getInstance().saveElementaryschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
	}

	@SuppressWarnings("unused")
	private void doSaveTech(User host, CoralFormInfo form) {
		String checkrst = "";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isATechschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //技校
			GuideXFiveUserInfoLogic.getInstance().saveCollege(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //小学
			GuideXFiveUserInfoLogic.getInstance().saveElementaryschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
	}




	@SuppressWarnings("unused")
	private void doSaveNone(User host, CoralFormInfo form) {
		String checkrst = "";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isACurrentPlaceInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //所在地
			GuideXFiveUserInfoLogic.getInstance().saveCurrentRegionNet(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAHomePlaceInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //家乡
			GuideXFiveUserInfoLogic.getInstance().saveHomeRegion(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		
	}




	@SuppressWarnings("unused")
	private void doSaveMajo(User host, CoralFormInfo form) {
		String checkrst = "";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isACurrentPlaceInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //所在地
			GuideXFiveUserInfoLogic.getInstance().saveCurrentRegionNet(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAHomePlaceInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //家乡
			GuideXFiveUserInfoLogic.getInstance().saveHomeRegion(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAWorkInfo(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //公司
			GuideXFiveUserInfoLogic.getInstance().saveWork(host, form);
			
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		//学校们
		
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAUniversity(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //大学
			GuideXFiveUserInfoLogic.getInstance().addUniversity(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //高中
			GuideXFiveUserInfoLogic.getInstance().saveHighschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //初中 
			GuideXFiveUserInfoLogic.getInstance().saveJuniorschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isATechschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //技校
			GuideXFiveUserInfoLogic.getInstance().saveCollege(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //小学
			GuideXFiveUserInfoLogic.getInstance().saveElementaryschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
	}




	@SuppressWarnings("unused")
	private void doSaveHigh(User host, CoralFormInfo form) {
		String checkrst = "";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //高中
			GuideXFiveUserInfoLogic.getInstance().saveHighschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //初中 
			GuideXFiveUserInfoLogic.getInstance().saveJuniorschoolNet(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
	}




	private void doSaveJuni(User host, CoralFormInfo form) {
		String checkrst = "";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //初中 
			GuideXFiveUserInfoLogic.getInstance().saveJuniorschoolNet(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //小学
			GuideXFiveUserInfoLogic.getInstance().saveElementaryschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
	}




	@SuppressWarnings("unused")
	private void doSaveUniv(User host, CoralFormInfo form) {
		String  checkrst ="";
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAUniversity(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //大学
			GuideXFiveUserInfoLogic.getInstance().addUniversity(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(form);
		if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //高中
			GuideXFiveUserInfoLogic.getInstance().saveHighschool(host, form);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
		}
		
	}




}
