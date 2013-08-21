package com.xiaonei.reg.guide.action.netbarflow;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.Enumeration;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.action.a090729.MyStringUtil;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

public class FillinfoSubmitAction extends ProtectAction{
	private final int thisStep = StepStatusLogic.STEP_FILLINFO;
	private final static String SUCC = "succ";
	private final static String ERROR = "error";
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
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		MyTimeCost cost = MyTimeCost.begin();
		printParams(request);
		cost.endStep();
		FillinfoFormBean fform = getFormBean(request);
		cost.endStep();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		if(fform == null){
			GuideLogger.printLog("| host:"+host.getId()+" getForm err");
			return mapping.findForward(ERROR);
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
		
		//新手帮
		//cost.endStep();
		//A090729Util.beFansofPage(""+600002351, host);
		
		cost.endStep();
		cost.endFinally();
		return mapping.findForward(SUCC);
	}
}
