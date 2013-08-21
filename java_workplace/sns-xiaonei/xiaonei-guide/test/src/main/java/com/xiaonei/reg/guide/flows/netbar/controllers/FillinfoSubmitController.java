package com.xiaonei.reg.guide.flows.netbar.controllers;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.util.Enumeration;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.action.a090729.MyStringUtil;
import com.xiaonei.reg.guide.action.netbarflow.FillinfoFormBean;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

@LoginRequired
@Path("finbs")
public class FillinfoSubmitController {
	private final int thisStep = StepStatusLogic.STEP_FILLINFO;
	private final static String SUCC = "f:lead";
	private final static String ERROR = "f:finb";
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

	private FillinfoFormBean getFormBean(HttpServletRequest request){
		String [] props = FillinfoFormBean.getProps();
		String para = null;
		String svalue = null;
		FillinfoFormBean form = new FillinfoFormBean();
		Field f = null;
		for(String prop : props){
			try {
				para = prop;
				svalue = request.getParameter("fi_"+para);
				f = form.getClass().getDeclaredField(prop);
				//验证是否必填
				if((svalue==null || svalue.trim().equals("")) /*&& keymap.containsKey(para)*/){
					GuideLogger.printLog("need parameter:fi_"+para,GuideLogger.ERROR);
					return null;
				}
				//验证是否合法数据
				Object ovalue = MyStringUtil.stringToObject(svalue, f.getType());
				if(svalue==null || svalue.trim().equals("")){
					ovalue = MyStringUtil.getDefaultObject(f.getType());
				}
				if(ovalue == null){
					GuideLogger.printLog(" parameter is not valid!:fi_"+para+":"+svalue,GuideLogger.ERROR);
					return null;
				}
				else{
					if(MyStringUtil.getUpperHead(prop).indexOf("Highschoolclass")>=0){
						ovalue = ovalue.toString().replace("班", "");
					}
					form.getClass().getMethod("set"+MyStringUtil.getUpperHead(prop), f.getType()).invoke(form, ovalue);
					GuideLogger.printLog("set"+MyStringUtil.getUpperHead(prop)+"() called :"+ovalue);
				}
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
		return form;
	}
	
	@Get
	public String get(Invocation inv){
		return ERROR;
	}
	
	@Post
	public String post(Invocation inv){
		HttpServletRequest request = inv.getRequest();
		GuideTimeCost cost = GuideTimeCost.begin();
		printParams(request);
		cost.endStep();
		FillinfoFormBean fform = getFormBean(request);
		cost.endStep();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		if(fform == null){
			GuideLogger.printLog("| host:"+host.getId()+" getForm err");
			return (ERROR);
		}
		cost.endStep();
		
		//保存家乡
		GuideUserProfileLogic.getInstance().saveHomeRegion(host, fform);
		cost.endStep();
		
		//保存当前地
		GuideUserProfileLogic.getInstance().saveCurrentRegionAndNet(host, fform);
		
		//保存当前Step
		cost.endStep();
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
		
		//保存当前Stage
		cost.endStep();
		GuideUserProfileLogic.getInstance().saveStage(host, Stage.STAGE_NONE);
		
		/*//2010-2-5 14:14:00 Inshion 去掉
		//新手帮
		cost.endStep();
		A090729Util.asyncBeFansofPage(600002351, host);*/
		cost.endStep();
		cost.endFinally();
		return (SUCC);
	}
}
