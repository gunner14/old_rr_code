package com.xiaonei.reg.guide.flows.xfivedark.controllers;

import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.model.FriendApply;
import com.xiaonei.platform.component.friends.model.FriendApplyResult;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.dao.VKGuideDAO;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormZhongxue;
import com.xiaonei.reg.guide.flows.xfive.formbeans.VKFormDaxue;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserNameLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.interfaces.IXProfilable;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.pojo.VKCoralFormInfo;
import com.xiaonei.reg.register.pojo.VKFriendInfo;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@LoginRequired
@Path({"guidejumpstep"})
@IgnoreUserStatusImportedValidation
public class VKJumpGuideStepFillInfoController {
	
	@Autowired
	private VKGuideDAO vKGuideDAO;
	
	
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
		return index(inv);
	}
	
	@Post
	public String index(Invocation inv){
		HttpServletRequest request =inv.getRequest();
		String stage=request.getParameter("stage");
		String guideUserid=request.getParameter("userid");
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		if(stage==null || stage.trim().length()<=0 || guideUserid==null || guideUserid.trim().length()<=0){
			GuideLogger.printLog("happen exception userid:"+host.getId()+"guideUserid"+guideUserid);
			return "r:http://www.renren.com/";
		}
		GuideLogger.printLog("guideUserid:"+guideUserid+" stage:"+stage);
		String schoolType="";
		try {
			if("10".equals(stage)){
				FormZhongxue formZhongxue=vKGuideDAO.selectMid(guideUserid);
				if(formZhongxue==null){
					guideUserid=vKGuideDAO.selectUseridByRealUserid(host.getId());
					formZhongxue=vKGuideDAO.selectMid(guideUserid);
					GuideLogger.printLog("pegging guideUserid:"+guideUserid);
				}
				if(formZhongxue==null){
					GuideLogger.printLog("zhongxue userid have not record guideUserid:"+guideUserid);
					//阶段
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_HEIGHSCHOOL);
					//改标志
					StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
					sendFriendRequest(guideUserid,host);
					return "r:http://www.renren.com";
				}
				GuideLogger.printLog("formZhongxue:"+formZhongxue.getInfo());
				schoolType=formZhongxue.getSchooltype();
				//导入上高中资料
				if("highschool".equals(schoolType)){
					
					GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolZhongxue(host, formZhongxue);
					GuideXFiveUserProfileLogic.getInstance().saveHighschoolZhongxue(host, formZhongxue);
					if(formZhongxue.getHighschoolname()!=null && formZhongxue.getHighschoolname().length()>0){
						GuideXFiveUserProfileLogic.getInstance().saveHighschoolNetZhongxue(host, formZhongxue);
					}
				}
				//导入初中资料
				else if("juniorhighschool".equals(schoolType)){
					GuideXFiveUserProfileLogic.getInstance().saveElementaryschoolZhongxue(host, formZhongxue);
					GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolZhongxue(host, formZhongxue);
					if(formZhongxue.getJuniorhighschoolname()!=null && formZhongxue.getJuniorhighschoolname().length()>0){
						GuideXFiveUserProfileLogic.getInstance().saveCollegeNetZhongxue(host, formZhongxue);
					}
				}
				
				//导入中专资料
				else if("techschool".equals(schoolType)){
					GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolZhongxue(host, formZhongxue);
					GuideXFiveUserProfileLogic.getInstance().saveCollegeZhongxue(host, formZhongxue);
					if(formZhongxue.getTechschoolname()!=null && formZhongxue.getTechschoolname().length()>0){
						GuideXFiveUserProfileLogic.getInstance().saveCollegeNetZhongxue(host, formZhongxue);
					}
					
				}
				//阶段
				FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_HEIGHSCHOOL);
				//改标志
				StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
			}
			else if ("20".equals(stage)) {
				VKFormDaxue vKFromDaxue=vKGuideDAO.selectUniv(guideUserid);
				
				if(vKFromDaxue==null){
					guideUserid=vKGuideDAO.selectUseridByRealUserid(host.getId());
					vKFromDaxue=vKGuideDAO.selectUniv(guideUserid);
					GuideLogger.printLog("pegging guideUserid:"+guideUserid);
				}
				
				if(vKFromDaxue==null){
					GuideLogger.printLog("univ userid have not record guideUserid:"+guideUserid);
					
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_UNIV);
					StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
					sendFriendRequest(guideUserid, host);
					return "r:http://www.renren.com";
				}
				
				GuideLogger.printLog("vKFromDaxue:"+vKFromDaxue.getInfo());
				
				GuideXFiveUserProfileLogic.getInstance().addVKUniversityAndNetDaxue(host, vKFromDaxue);
				GuideXFiveUserProfileLogic.getInstance().saveVKHighschoolDaxue(host, vKFromDaxue);
				FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_UNIV);
				StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);

			}
			
			else if ("30".equals(stage) || "90".equals(stage)) {
				CoralFormInfo vKCoralFormInfo=null;
				if("30".equals(stage)){
					vKCoralFormInfo=vKGuideDAO.selectWork(guideUserid);
					if(vKCoralFormInfo==null){
						guideUserid=vKGuideDAO.selectUseridByRealUserid(host.getId());
						vKCoralFormInfo=vKGuideDAO.selectWork(guideUserid);
						GuideLogger.printLog("pegging guideUserid:"+guideUserid);
					}
				}
				else{
					vKCoralFormInfo=vKGuideDAO.selectNone(guideUserid);
					
					if(vKCoralFormInfo==null){
						guideUserid=vKGuideDAO.selectUseridByRealUserid(host.getId());
						vKCoralFormInfo=vKGuideDAO.selectNone(guideUserid);
						GuideLogger.printLog("pegging guideUserid:"+guideUserid);
					}
					
				}
				
				if(vKCoralFormInfo==null){
					GuideLogger.printLog("worknone userid have not record guideUserid:"+guideUserid+" stage:"+stage);
					FillInfoLogic.getInstance().saveStage(host, new Integer(stage));
					StepStatusLogic.updateStepStatus(host.getId(), INT_THIS_STEP);
					sendFriendRequest(guideUserid, host);
					return "r:http://www.renren.com";
				}
				
				GuideLogger.printLog("vKCoralFormInfo:"+vKCoralFormInfo.toString());
				int userstage =new Integer(stage);
				GuideXFiveUseIdentityLogic.getInstance().saveIdentity(host.getId(), userstage);
				
				Map<String,IXProfilable> param =  new HashMap<String, IXProfilable>();
				String checkrst = "";
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAUniversity(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //大学
					param.put("info_dx", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //高中
					param.put("info_gz", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //初中 
					param.put("info_cz", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isATechschool(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //技校
					param.put("info_jx", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAWorkInfo(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //公司
					param.put("info_gs", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //小学
					param.put("info_xx", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isACurrentPlaceInfo(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //所在地
					param.put("info_dd", vKCoralFormInfo);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
				}
				checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAHomePlaceInfo(vKCoralFormInfo);
				if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){ //家乡
					param.put("info_gx", vKCoralFormInfo);
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
					doSaveJuni(host,vKCoralFormInfo);
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
					GuideLogger.printLog("import none user data");
					//doSaveNone(host,form);
					savestage = chageNoneStage(host,vKCoralFormInfo,param);
					//savestage = Stage.STAGE_NONE;
					break;
				default:
					GuideLogger.printLog(" host:"+host.getId()+"stage:"+userstage,GuideLogger.WARN);
				break;
				}
				doSaveAll(host,vKCoralFormInfo,savestage,param);
				
				try {
					//改标志
					FillInfoLogic.getInstance().saveStage(host, savestage);
					StepStatusLogic.updateStepStatus(host.getId(), INT_THIS_STEP);
					GuideLogger.printLog(" host:"+host.getId()+" update step:"+INT_THIS_STEP);
					
				} catch (Exception e) {
					e.printStackTrace();
				}
				
			
			}
			else{
				GuideLogger.printLog("happen exception stage is not correct userid:"+host.getId());
				return "r:http://reg.renren.com/enterbaseinfojsp";
			}
			
			sendFriendRequest(guideUserid,host);
			
		} catch (SQLException e1) {
			e1.printStackTrace();
		}
				
		return "r:lead";
	}

	
	 private void returnMessage(FriendApplyResult friendApplyResult) {
	        JSONObject jo = new JSONObject();
	        jo.put("code", friendApplyResult.getResult());
	        jo.put("message", friendApplyResult.getMessage());
	        try {
	            jo.put("targetfriendcount", FriendsHome.getInstance().getFCount(friendApplyResult.getFriendApply().getRecipient().getId()));
	        } catch (Exception e) {
	            System.err.println(e.toString());
	        }
	        jo.put("ta", friendApplyResult.getFriendApply().getRecipient().getGender());
	        jo.put("id", friendApplyResult.getFriendApply().getRecipient().getId());
	        jo.put("name", friendApplyResult.getFriendApply().getRecipient().getName());
	        jo.put("tiny_head_url", friendApplyResult.getFriendApply().getRecipient().getTinyFullUrl());
	        jo.put("hostIsVip", friendApplyResult.getFriendApply().getApplicant().isVipMember());
	        jo.put("guestIsVip", friendApplyResult.getFriendApply().getRecipient().isVipMember());
	        GuideLogger.printLog("addfriendresult:"+jo.toString());
	        //writeAjaxStream(response, jo.toString());
	    }
	private void doSaveAll(User host, CoralFormInfo form, int savestage, Map<String, IXProfilable> param) {
		doSaveAllInfo(host, form);
		doSaveNetwork(host,form,savestage,param);
	}
	
	private void sendFriendRequest(String guideUserid,User host){
		//发好友请求
		String domain = OpiConstants.getCurrentDomain();
		List<VKFriendInfo> vKFriendInfoList=null;
		try {
			vKFriendInfoList = vKGuideDAO.selectUseridByUserid(guideUserid);
			if(vKFriendInfoList!=null && vKFriendInfoList.size()>0){
				VKFriendInfo vKFriendInfo=null;
				int size=vKFriendInfoList.size();
				if(size>51){
					size=10;
				}
				String fromSource="";
				User to=null;
				for(int i=0;i<size;i++){
					vKFriendInfo=vKFriendInfoList.get(i);
					if(vKFriendInfo==null || vKFriendInfo.getFriendlist()==null || vKFriendInfo.getFriendlist().trim().length()<=0){
						GuideLogger.printLog("friendlist is null");
						break;
					}
					
					fromSource=vKFriendInfo.getFromsource();
					to=SnsAdapterFactory.getUserAdapter().get(new Integer(vKFriendInfo.getFriendlist()));
					if(to==null){
						GuideLogger.printLog("no user:"+vKFriendInfo.getFriendlist());
						break;
					}
					FriendApply friendApply = new FriendApply(host, to, domain);
		            friendApply.setFrom(fromSource);
		            FriendApplyResult friendApplyResult=null;
					try {
						friendApplyResult = FriendsHome.getInstance().requestFriendApply(friendApply);
						//returnMessage(friendApplyResult);
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
			else{
				GuideLogger.printLog("current user no friend userid:"+guideUserid);
			}
		} catch (SQLException e1) {
			GuideLogger.printLog("exception add friend:"+guideUserid);
			e1.printStackTrace();
		}
		
	}
	
	
	
	/*private void sendFriendRequest(String guideUserid,User host){
		//发好友请求
		String domain = OpiConstants.getCurrentDomain();
		VKFriendInfo vKFriendInfo;
		try {
			vKFriendInfo = vKGuideDAO.selectUseridByUserid(guideUserid);
			if(vKFriendInfo!=null){
				String friendList=vKFriendInfo.getFriendlist();
				String fromSource=vKFriendInfo.getFromsource();
				User to=null;
				if(friendList!=null && friendList.length()>0){
					GuideLogger.printLog("friendList:"+friendList);
					String realUserid[]= friendList.split(":");
					int size=realUserid.length;
					for(int i=0;i<size;i++){
						to=SnsAdapterFactory.getUserAdapter().get(new Integer(realUserid[i]));
						if(to==null){
							GuideLogger.printLog("no user:"+realUserid[i]);
							break;
							
						}
						FriendApply friendApply = new FriendApply(host, to, domain);
			            friendApply.setFrom(fromSource);
			            FriendApplyResult friendApplyResult=null;
						try {
							friendApplyResult = FriendsHome.getInstance().requestFriendApply(friendApply);
							//returnMessage(friendApplyResult);
						} catch (Exception e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						
					}
					
				}
			}
			else{
				GuideLogger.printLog("current user no friend userid:"+guideUserid);
			}
		} catch (SQLException e1) {
			e1.printStackTrace();
		}
		
	}*/
	

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
