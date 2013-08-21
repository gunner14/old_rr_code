package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.io.IOException;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.admin.biz.model.school.SchoolType;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.action.a090729.MyRequestUtil;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.a090821.UserProfileUtil;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.form.f090729.FormBailing;
import com.xiaonei.reg.guide.form.f090729.FormDaxue;
import com.xiaonei.reg.guide.form.f090729.FormQita;
import com.xiaonei.reg.guide.form.f090729.FormZhongxue;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileCacheLogic;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileLogic;
import com.xiaonei.reg.guide.root.interfaces.IInfoElementaryschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoJuniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoSeniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoTechschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoUniversity;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoCurrentPlace;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoHomePlace;
import com.xiaonei.reg.guide.root.interfaces.IWorkInfoCompany;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.dao.IpTableDAO;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckUniversity;

/**
 * AjaxSubmitFillInfoController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明 
 */
@LoginRequired
@Path("axsmt.do")
public class AjaxSubmitFillInfoController {

	private static final String [] props_daxue = new String [] {
		"universityname",
		"universityyear",
		"universitycode",
		"universitydepartment",
		"highschoolname",
		"highschoolyear",
		"highschoolcode",
	}; 
	
	private static final String [] props_gaozhong = new String [] {
		"schooltype",
		"highschoolname",
		"highschoolyear",
		"highschoolcode",
		"juniorhighschoolname",
		"juniorhighschoolyear",
		"juniorhighschoolcode",
		"highschoolclass1",
		"highschoolclass2",
		"highschoolclass3",
	};

	private static final String [] props_chuzhong = new String [] {
		"schooltype",
		"juniorhighschoolname",
		"juniorhighschoolyear",
		"juniorhighschoolcode",
		"elementaryschoolname",
		"elementaryschoolyear",
//		"elementaryschoolcode",
	};
	
	private static final String [] props_jixiao = new String [] {
		"schooltype",
		"techschoolname",
		"techschoolyear",
		"techschoolcode",
		"juniorhighschoolname",
		"juniorhighschoolyear",
		"juniorhighschoolcode",
	};
	
	private static final String [] props_bailing = new String [] {
		"companyname",
		"homeprovince",
		"homecityname",
		"homecitycode",
		"universityname",
		"universityyear",
		"universitycode",
		"universitydepartment",
	};
	
	private static final String [] props_qita = new String [] {
		"homeprovince",
		"homecityname",
		"homecitycode",
		"universityname",
		"universityyear",
		"universitycode",
		"universitydepartment",
	};
	
	private static final Map<String, Integer> map_daxue = new HashMap<String, Integer>();
	private static final Map<String, Integer> map_gaozhong = new HashMap<String, Integer>();
	private static final Map<String, Integer> map_chuzhong = new HashMap<String, Integer>();
	private static final Map<String, Integer> map_jixiao = new HashMap<String, Integer>();
	private static final Map<String, Integer> map_bailing = new HashMap<String, Integer>();
	private static final Map<String, Integer> map_qita = new HashMap<String, Integer>();
	static{
		for (String s:props_daxue){
			map_daxue.put(s, 1);
		}
		for (String s : props_gaozhong) {
			if(s.equals("highschoolclass1"))break;
			map_gaozhong.put(s, 1);
		}
		for (String s : props_chuzhong) {
			if(s.equals("elementaryschoolcode"))break;
			map_chuzhong.put(s, 1);
		}
		for (String s : props_jixiao) {
			map_jixiao.put(s, 1);
		}
		for (String s : props_bailing) {
			if(s.equals("universityname")) break;
			map_bailing.put(s, 1);
		}
		for (String s : props_qita) {
			if(s.equals("universityname")) break;
			map_qita.put(s, 1);
		}
	}
	
	private Object stringToObject(String src,Class<?> type){
		Object obj = null;
		GuideLogger.printLog("stringToObject() - src:"+src+" type:"+type.getName());
		if(type.getName().equals("int") || type.getName().contains("Integer")){
			try {
				obj = Integer.parseInt(src);
			} catch (Exception e) {
				GuideLogger.printLog("error - "+src+type.getName(),GuideLogger.DEBUG);
			}
		}
		if(type.getName().contains("String")){
			obj = src;
		}
		return obj;
	}	
	private Object getDefaultObject(Class<?> type) {
		Object obj = null;
		GuideLogger.printLog("getDefaultObject() - type:"+type.getName());
		if(type.getName().equals("int") || type.getName().contains("Integer")){
			obj = 0;
		}
		if(type.getName().contains("String")){
			obj = "";
		}
		return obj;
	}
	private String getUpperHead(String s){
		if(s==null || s.trim().equals("")){
			return s;
		}
		if(s.length()==1){
			return s.toUpperCase();
		}
		return s.toUpperCase().substring(0,1)+s.substring(1);
	}
	
	@Post
	@Get
	public String post(Invocation inv){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			MyTimeCost cost = MyTimeCost.begin();
			cost.endStep();
			String str_stage = request.getParameter("fi_stage");
			cost.endStep();
			if(null == str_stage || "".equals(str_stage.toString())){
				writeAjax(response, constructJSONString("msg", "fi_stage is empty!"));
				return null;
			}
			cost.endStep();
			
			int i_stage = 0;
			cost.endStep();
			try {
				i_stage = Integer.parseInt(str_stage);
				cost.endStep();
			} catch (Exception e) {
				writeAjax(response, constructJSONString("msg", "fi_stage is not valid:"+str_stage));
				return null;
			}
			cost.endStep();
			GuideLogger.printLog(" host:"+host.getId()+MyRequestUtil.getBrowserInfo(request));
			
			cost.endStep();
			int saveresult = 0;
			cost.endStep();
			switch(i_stage){
			case 10: //zhongxue
				String str_schooltype = request.getParameter("fi_schooltype");
				if("0".equals(str_schooltype)){//gaozhong
					FormZhongxue fzhongxue = (FormZhongxue) getForm(request,response,new FormZhongxue(),props_gaozhong,map_gaozhong);
					saveresult = saveZhongxue(fzhongxue,host,request,response);
				}
				else if("1".equals(str_schooltype)){//chuzhong
					FormZhongxue fzhongxue = (FormZhongxue) getForm(request,response,new FormZhongxue(),props_chuzhong,map_chuzhong);
					saveresult = saveZhongxue(fzhongxue,host,request,response);
				}
				else if("2".equals(str_schooltype)){//jixiao
					FormZhongxue fzhongxue = (FormZhongxue) getForm(request,response,new FormZhongxue(),props_jixiao,map_jixiao);
					saveresult = saveZhongxue(fzhongxue,host,request,response);
				}
				else{
					writeAjax(response, constructJSONString("msg", "fi_schooltype is not valid!:"+str_schooltype));
					return null;
				}
				cost.endStep();
				break;
			case 20: //daxue
				FormDaxue fdaxue =(FormDaxue) getForm(request,response,new FormDaxue(),props_daxue,map_daxue);
				saveresult = saveDaxue(fdaxue,host,request,response);
				break;
			case 30: //bailing
				FormBailing fbailing =(FormBailing) getForm(request,response,new FormBailing(),props_bailing,map_bailing);
				saveresult = saveBailing(fbailing,host,request,response);
				break;
			case 90: //qita
				FormQita fqita = (FormQita) getForm(request, response, new FormQita(), props_qita, map_qita);
				saveresult = saveQita(fqita,host,request,response);
				break;
			default:
				writeAjax(response, constructJSONString("msg", "number of stage is not valid:"+str_stage));
				cost.endStep();
				return null;
			}
			cost.endStep();
			if(saveresult == 1){
				writeAjax(response, constructJSONString("msg", "OK"));
				try {
					StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
				} catch (Exception e) {
					GuideLogger.printLog("executePt() - save step error! host:"+host.getId(),1);
				}
				cost.endStep();
			}
			else{
				cost.endStep();
				return null;
				//writeAjax(response, constructJSONString("msg", "save error:"+saveresult));
			}
			cost.endStep();
			cost.endFinally();
		}
		return null;
		
	}
	@SuppressWarnings("unused")
	private void printParams(HttpServletRequest request) {
		try {
			Enumeration<?> params = request.getParameterNames();
			while (params.hasMoreElements()) {
				String para = (String) params.nextElement();
				if (para.startsWith("fi_")) {
					String valu = request.getParameter(para);
					GuideLogger.printLog("" + para + " : "	+ valu);
				}
			}
		} catch (Exception e) {
			GuideLogger.printLog("printParams() - "+e.toString(),GuideLogger.ERROR);
		}
	}
	private int saveQita(FormQita fform, User host, HttpServletRequest request, HttpServletResponse response) {
		if(fform == null) return 90;
		MyTimeCost cost = MyTimeCost.begin();
		cost.endStep();
		try {
			GuideLogger.printLog("(FormQita) - hostid:"+ host.getId()+ " - saveuniv - univ:"+fform.getUniversityname());
			cost.endStep();
			//大学
			String chk = new CheckUniversity().checkUniversity("1", fform.getUniversitycode(), fform.getUniversityname());
			if(StringUtils.equals(chk, "OK") || StringUtils.equals(chk, "请填写大学（选填）")){
				UserProfileUtil.saveUniversity(host, fform);
				UniversityInfo university = new UniversityInfo();
				university.setUniversityId(fform.getUniversitycode());
				university.setDepartment(fform.getUniversitydepartment());
				university.setUniversityName(fform.getUniversityname());
				university.setEnrollYear(fform.getUniversityyear());
				university.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, university);
				cost.endStep();
			}
			cost.endStep();
			
			
		} catch (Exception e) {
			GuideLogger.printLog("(FormQita) saveUniv() error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		
		try {
			GuideLogger.printLog("(FormQita) - hostid:"+ host.getId()+ " - savecity - city:"+fform.getHomecityname());
			cost.endStep();
			//地理
			if(checkCityNet(fform)){ 
				UserProfileUtil.saveRegionAndNet(host, fform);
				cost.endStep();
				RegionInfo region = new RegionInfo();
				region.setProvince_name(fform.getHomeprovince());
				region.setCity_name(fform.getHomecityname());
				region.setRegion_id(fform.getHomecitycode());
				region.setUserid(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserHomeRegion(host, region);
			}
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.printLog("(FormQita) saveCity() error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		
		try {
			GuideLogger.printLog("(FormQita) - hostid:"+ host.getId()+ " - savestage - stage:"+Stage.STAGE_NONE);
			cost.endStep();
			//阶段
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_NONE);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("(FormQita) saveStage() error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		cost.endFinally();
		return 1;
		
	}
	private int saveBailing(FormBailing fform, User host,	HttpServletRequest request, HttpServletResponse response) {
		if(fform == null) return 30;
		MyTimeCost cost = MyTimeCost.begin();
		cost.endStep();
		try {
			GuideLogger.printLog("(FormBailing) - hostid:"+ host.getId()+ " - savework - com:"+fform.getCompanyname());
			cost.endStep();
			//工作
			UserProfileUtil.saveWork(host, fform);
			cost.endStep();
			WorkplaceInfo work = new WorkplaceInfo();
			work.setWorkplaceName(fform.getCompanyname());
			GuideUserProfileCacheLogic.getInstance().saveUserWork(host, work);
			
		} catch (Exception e) {
			GuideLogger.printLog("(FormBailing) saveWork() error "+host.getId()+" "+e.toString(),1);
			e.printStackTrace();
		}
		cost.endStep();
		
		try {
			GuideLogger.printLog("(FormBailing) - hostid:"+ host.getId()+ " - saveuniv - univ:"+fform.getUniversityname());
			cost.endStep();
			//大学
			String chk = new CheckUniversity().checkUniversity("1", fform.getUniversitycode(), fform.getUniversityname());
			if(StringUtils.equals(chk, "OK") || StringUtils.equals(chk, "请填写大学（选填）")){
				UniversityInfo university = new UniversityInfo();
				university.setUniversityId(fform.getUniversitycode());
				university.setDepartment(fform.getUniversitydepartment());
				university.setUniversityName(fform.getUniversityname());
				university.setEnrollYear(fform.getUniversityyear());
				university.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, university);
				UserProfileUtil.saveUniversity(host, fform);
				cost.endStep();
			}
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.printLog("(FormBailing) saveUniv() error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		
		try {
			GuideLogger.printLog("(FormBailing) - hostid:"+ host.getId()+ " - savecity - city:"+fform.getHomecityname());
			cost.endStep();
			//地理
			if(checkCityNet(fform)){ 
				UserProfileUtil.saveRegionAndNet(host, fform);
				RegionInfo region = new RegionInfo();
				region.setProvince_name(fform.getHomeprovince());
				region.setCity_name(fform.getHomecityname());
				region.setRegion_id(fform.getHomecitycode());
				region.setUserid(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserHomeRegion(host, region);
				cost.endStep();
			}
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.printLog("(FormBailing) saveCity() error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		
		try {
			GuideLogger.printLog("(FormBailing) - hostid:"+ host.getId()+ " - savestage - stage:"+Stage.STAGE_MAJOR);
			cost.endStep();
			//阶段
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_MAJOR);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("(FormBailing) saveStage() error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		cost.endFinally();
		return 1;
	}
	private int saveZhongxue(FormZhongxue fform, User host, HttpServletRequest request, HttpServletResponse response) {
		if(fform == null) return 10;
		MyTimeCost cost = MyTimeCost.begin();
		cost.endStep();
		GuideLogger.printLog("(FormZhongxue) -- save 1");
		cost.endStep();
		GuideLogger.printLog("(FormZhongxue) - hostid:"+ host.getId()+ " - start");
		cost.endStep();
		try {
			GuideLogger.printLog("Schooltype int is "+fform.getSchooltype()+" "+fform.getHighschoolname()+" "+fform.getJuniorhighschoolname()+" "+fform.getElementaryschoolname()+" "+fform.getTechschoolname());
			int stype = Integer.valueOf(fform.getSchooltype()) + 2;
			cost.endStep();
			boolean iscurrentnetvalid = checkMsNet(fform,stype);
			switch (stype) {
			case SchoolType.TYPE_HIGHSCHOOL:
				GuideLogger.printLog("(FormZhongxue) - hostid:"+ host.getId()+ " - TYPE_HIGHSCHOOL");
				cost.endStep();
				/*
				UserProfileUtil.saveJuniorschool(host, fform);
				UserProfileUtil.saveHighschool(host, fform);
				if(iscurrentnetvalid){
					UserProfileUtil.saveHighschoolNet(host, fform);
				}
				*/
				/*Util用Logic代替*/
				GuideUserProfileLogic.getInstance().asyncSaveJuniorschool(host, fform);
				cost.endStep();
				GuideUserProfileLogic.getInstance().saveHighschool(host, fform);
				cost.endStep();
				if(iscurrentnetvalid){
					GuideUserProfileLogic.getInstance().asyncSaveHighschoolNet(host, fform);
					cost.endStep();
				}
				
				JuniorHighSchoolInfo juniorchool = new JuniorHighSchoolInfo();
				juniorchool.setJunHighentarySchoolId(fform.getJuniorhighschoolcode());
				juniorchool.setJunHighentarySchoolName(fform.getJuniorhighschoolname());
				juniorchool.setJunHighentarySchoolYear(fform.getJuniorhighschoolyear());
				juniorchool.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserJuniorschool(host, juniorchool);
				
				HighSchoolInfo highschool = new HighSchoolInfo();
				highschool.setHighSchoolName(fform.getHighschoolname());
				highschool.setHighSchoolId(fform.getHighschoolcode());
				highschool.setEnrollYear(fform.getHighschoolyear());
				highschool.setHClass1(fform.getHighschoolclass1());
				highschool.setHClass2(fform.getHighschoolclass2());
				highschool.setHClass3(fform.getHighschoolclass3());
				highschool.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, highschool);
				cost.endStep();
				break;
			case SchoolType.TYPE_JUNIORSCHOOL:
				cost.endStep();
				/*
				MyLogger.printLog("(FormZhongxue) - hostid:"+ host.getId()+ " - TYPE_JUNIORSCHOOL");
				UserProfileUtil.saveElementaryschool(host, fform);
				UserProfileUtil.saveJuniorschool(host, fform);
				*/
				/*Util用Logic代替*/
				GuideUserProfileLogic.getInstance().asyncSaveElementaryschool(host, fform);
				cost.endStep();
				GuideUserProfileLogic.getInstance().saveJuniorschool(host, fform);
				cost.endStep();
				//2009-11-11 14:07:59 加上 存初中网络
				if(iscurrentnetvalid){
					GuideUserProfileLogic.getInstance().asyncSaveJuniorschoolNet(host, fform);
					cost.endStep();
				}
				
				JuniorHighSchoolInfo juniorchool_junior = new JuniorHighSchoolInfo();
				juniorchool_junior.setJunHighentarySchoolId(fform.getJuniorhighschoolcode());
				juniorchool_junior.setJunHighentarySchoolName(fform.getJuniorhighschoolname());
				juniorchool_junior.setJunHighentarySchoolYear(fform.getJuniorhighschoolyear());
				juniorchool_junior.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserJuniorschool(host, juniorchool_junior);
				
				ElementarySchoolInfo elementaryschool = new ElementarySchoolInfo();
				elementaryschool.setElementarySchoolId(fform.getElementaryschoolcode());
				elementaryschool.setElementarySchoolName(fform.getElementaryschoolname());
				elementaryschool.setElementarySchoolYear(fform.getElementaryschoolyear());
				elementaryschool.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserElementary(host, elementaryschool);
				cost.endStep();
				break;
			case SchoolType.TYPE_COLLEGESCHOOL:
				cost.endStep();
				/*
				MyLogger.printLog("(FormZhongxue) - hostid:"+ host.getId()+ " - TYPE_COLLEGESCHOOL");
				UserProfileUtil.saveJuniorschool(host, fform);
				UserProfileUtil.saveCollege(host, fform);
				*/
				GuideUserProfileLogic.getInstance().asyncSaveJuniorschool(host, fform);
				cost.endStep();
				GuideUserProfileLogic.getInstance().saveCollege(host, fform);
				/*Util用Logic代替*/
				//2009-11-11 14:08:04 加上 存技校网络
				if(iscurrentnetvalid){
					GuideUserProfileLogic.getInstance().asyncSaveCollegeNet(host, fform);
				}
				JuniorHighSchoolInfo juniorchool_tech = new JuniorHighSchoolInfo();
				juniorchool_tech.setJunHighentarySchoolId(fform.getJuniorhighschoolcode());
				juniorchool_tech.setJunHighentarySchoolName(fform.getJuniorhighschoolname());
				juniorchool_tech.setJunHighentarySchoolYear(fform.getJuniorhighschoolyear());
				juniorchool_tech.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserJuniorschool(host, juniorchool_tech);
				
				CollegeInfo college = new CollegeInfo();
				college.setCollegeId(fform.getTechschoolcode());
				college.setCollegeName(fform.getTechschoolname());
				college.setEnrollYear(fform.getTechschoolyear());
				college.setUserId(host.getId());
				GuideUserProfileCacheLogic.getInstance().saveUserTechschool(host, college);
				cost.endStep();
				break;
			}
			
		} catch (RuntimeException e) {
			GuideLogger.printLog("(FormZhongxue) error zhongxue "+e.getMessage(),1);
		}
		cost.endStep();
		
		try {
			GuideLogger.printLog("(FormZhongxue) - hostid:"+ host.getId()+ " - savestage - stage:"+Stage.STAGE_HEIGHSCHOOL);
			cost.endStep();
			//阶段
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_HEIGHSCHOOL);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("(FormZhongxue) saveStage error "+host.getId()+" "+e.toString(),1);
		}
		cost.endStep();
		cost.endFinally();
		return 1;
	}
	
	private int saveDaxue(final FormDaxue fform, final User host,final HttpServletRequest request,final HttpServletResponse response) {
		if(fform == null) return 20;
		//check!()
		MyTimeCost cost = MyTimeCost.begin();
		cost.endStep();
		UserProfileUtil.saveUniversity(host, fform);
		cost.endStep();
		
		Runnable taskSave_NET = new Runnable() {
			public void run() {
				MyTimeCost cost = MyTimeCost.begin();
				//checkUnivNet(fform, request);
				boolean cansave = true ;//把加网络的去了
				GuideLogger.printLog("(FormDaxue) -- save 3");
				cost.endStep();
				if(cansave){
					UserProfileUtil.saveUniversityNet(host, fform);
				}
				cost.endStep();
				cost.endFinally();
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
		
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
				GuideLogger.printLog("host:"+host.getId());
				cost.endFinally();
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSaveMS);
		
		UniversityInfo university = new UniversityInfo();
		university.setUniversityId(fform.getUniversitycode());
		university.setDepartment(fform.getUniversitydepartment());
		university.setUniversityName(fform.getUniversityname());
		university.setEnrollYear(fform.getUniversityyear());
		university.setUserId(host.getId());
		GuideUserProfileCacheLogic.getInstance().saveUserUniversity(host, university);
		
		HighSchoolInfo highschool = new HighSchoolInfo();
		highschool.setHighSchoolName(fform.getHighschoolname());
		highschool.setHighSchoolId(fform.getHighschoolcode());
		highschool.setEnrollYear(fform.getHighschoolyear());
		highschool.setUserId(host.getId());
		GuideUserProfileCacheLogic.getInstance().saveUserHighschool(host, highschool);
		
		//写阶段很关键 要同步
		FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_UNIV);
		cost.endStep();
		GuideLogger.printLog("(FormDaxue) - saveStage: "+host.getId());
		cost.endStep();
		
		GuideLogger.printLog("(FormDaxue) -- save OK");
		/*//2010-2-5 14:14:00 Inshion 去掉
		A090729Util.beFansof09(host);*/
		cost.endStep();
		GuideLogger.printLog("(FormDaxue) - hostid:"+ host.getId()+ " - end - "+fform.getUniversitycode()+" | "+fform.getHighschoolcode());
		cost.endStep();
		cost.endFinally();
		return 1;
	}
	private boolean checkCityNet(FormQita fform) {
		GuideLogger.printLog("checkCityNet() -- start");
		if (StringUtils.isNotEmpty(fform.getHomecityname())
				|| StringUtils.isNotEmpty(fform.getHomeprovince())) {
			GuideLogger.printLog("checkCityNet() -- end true");
			return true;
		}
		GuideLogger.printLog("checkCityNet() -- end false");
		return false;
	}
	
	private boolean checkCityNet(FormBailing fform) {
		GuideLogger.printLog("checkCityNet() -- start");
		if (StringUtils.isNotEmpty(fform.getHomecityname())
				|| StringUtils.isNotEmpty(fform.getHomeprovince())) {
			GuideLogger.printLog("checkCityNet() -- end true");
			return true;
		}
		GuideLogger.printLog("checkCityNet() -- end false");
		return false;
	}

	/**
	 * 判断时候可以加入中学网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkMsNet(FormZhongxue fform,int type) {
		//int type = Integer.valueOf(fform.getSchooltype()) + 2;
		switch (type) {
		case SchoolType.TYPE_HIGHSCHOOL: //gaozhong 
			if (StringUtils.isNotEmpty(fform.getHighschoolname())){
				GuideLogger.printLog("checkMsNet() high true" );
				return true;
			}
			break;
		case SchoolType.TYPE_JUNIORSCHOOL: //chuzhong
			if (StringUtils.isNotEmpty(fform.getJuniorhighschoolname())){
				GuideLogger.printLog("checkMsNet() juni true");
				return true;
			}
		case SchoolType.TYPE_COLLEGESCHOOL: //jixiao
			if (StringUtils.isNotEmpty(fform.getTechschoolname())){
				GuideLogger.printLog("checkMsNet() tech true" );
				return true;
			}
			break;
		}
		GuideLogger.printLog("checkMsNet() false" );
		return false;
	}
	private ActionForm getForm(HttpServletRequest request, HttpServletResponse response, ActionForm form, String[] props,Map<String,Integer> keymap){
		MyTimeCost cost = MyTimeCost.begin();
		cost.endStep();
		String para = null;
		String svalue = null;
		Field f = null;
		cost.endStep();
		for(String prop : props){
			cost.endStep();
			try {
				para = prop;
				svalue = request.getParameter("fi_"+para);
				cost.endStep();
				f = form.getClass().getDeclaredField(prop);
				cost.endStep();
				//验证是否必填
				if((svalue==null || svalue.trim().equals("")) && keymap.containsKey(para)){
					writeAjax(response, constructJSONString("msg", "need parameter:fi_"+para));
					cost.endFinally();
					return null;
				}
				cost.endStep();
				//验证是否合法数据
				Object ovalue = stringToObject(svalue, f.getType());
				if(svalue==null || svalue.trim().equals("")){
					ovalue = getDefaultObject(f.getType());
				}
				cost.endStep();
				if(ovalue == null){
					writeAjax(response, constructJSONString("msg", " parameter is not valid!:fi_"+para+":"+svalue));
					cost.endStep();
					cost.endFinally();
					return null;
				}
				else{
					if(getUpperHead(prop).indexOf("Highschoolclass")>=0){
						ovalue = ovalue.toString().replace("班", "");
					}
					cost.endStep();
					form.getClass().getMethod("set"+getUpperHead(prop), f.getType()).invoke(form, ovalue);
					cost.endStep();
					GuideLogger.printLog("set"+getUpperHead(prop)+"() called :"+ovalue);
					cost.endStep();
				}
				cost.endStep();
			} catch (SecurityException e) {
				GuideLogger.printLog(""+e.toString()+" - "+para+" | "+svalue+" | "+f.getName(),GuideLogger.ERROR);
			} catch (NoSuchMethodException e) {
				GuideLogger.printLog(""+e.toString()+" - "+para+" | "+svalue+" | "+f.getName(),GuideLogger.ERROR);
			} catch (IllegalArgumentException e) {
				GuideLogger.printLog(""+e.toString()+" - "+para+" | "+svalue+" | "+f.getName(),GuideLogger.ERROR);
			} catch (IllegalAccessException e) {
				GuideLogger.printLog(""+e.toString()+" - "+para+" | "+svalue+" | "+f.getName(),GuideLogger.ERROR);
			} catch (InvocationTargetException e) {
				GuideLogger.printLog(""+e.toString()+" - "+para+" | "+svalue+" | "+f.getName(),GuideLogger.ERROR);
			} catch (NoSuchFieldException e) {
				GuideLogger.printLog(""+e.toString()+" - "+para+" | "+svalue+" | "+f.getName(),GuideLogger.ERROR);
			}
		}
		cost.endStep();
		cost.endFinally();
		return form;
	}
	
	@SuppressWarnings("unused")
	private boolean checkUnivNet(FormDaxue fform,
			HttpServletRequest request) {
		MyTimeCost cost = MyTimeCost.begin();
		List<University> univList = null;
		cost.endStep();
		try {
			String ip = BaseActionUtil.getClientIP(request);
			cost.endStep();
			univList = IpTableDAO.getInstance().getUnivByIp(ip);
			cost.endStep();
		} catch (SQLException e) {
			e.printStackTrace();
			return false;
		}
		cost.endStep();
		// 搞ip
		if (univList != null && univList.size() > 0) {
			for (int i = 0; i < univList.size(); i++) {
				if (fform.getUniversitycode() == univList.get(i).getId()) {
					return true;
				}
			}
		}
		cost.endFinally();
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

	
	private String constructJSONString(String key, String value){
//		JSONArray ja = new JSONArray();
		JSONObject jo = new JSONObject();
		jo.put(key, value);
		
//		ja.add(jo);
		return jo.toString();
	}

	private ActionForward writeAjax(final HttpServletResponse response,
			final String str) {
		try {
			response.setContentType("text/html; charset=UTF-8");
			response.getWriter().print(str);
			response.getWriter().flush();
			response.getWriter().close();
		} catch (IOException e) {

		} finally {
		}
		return null;
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
