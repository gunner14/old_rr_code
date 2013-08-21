/**
 * 
 */
package com.xiaonei.reg.guide.action.a090729;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.admin.biz.model.school.SchoolType;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.form.f090729.FormZhongxue;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.logic.pretreatment.validate.Antispam;

/**
 * 无阶段的fillinfo
 * 
 * @author wangtai
 * 
 */
public class FillInfoActionZhongxue extends GuideFlowBaseAction {
	// public class NoStageFillInfoAction extends BaseAnonymousAction {

	private static final String FORWARD_SUCC = "succ";
	@SuppressWarnings("unused")
	private static final int CurrentNet = NetworkStatus.CURRENT_NETWORK;
	@SuppressWarnings("unused")
	private static final int PassNet = NetworkStatus.PASS_NETWORK;
	@SuppressWarnings("unused")
	private static final int NotNet = 9999999;
	@SuppressWarnings("unused")
	private static final FillInfoLogic userinfoLogic = FillInfoLogic
			.getInstance();

	/**
	 * 成功后跳转
	 * 
	 * @param request
	 * @param form
	 * @param mapping
	 * @param response
	 * 
	 * @param host
	 * 
	 * @return
	 */
	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		String rt = check(form, request);
		if (rt != null) {
			return err(rt, request, mapping, response);
		}
		rt = sb(request, form, mapping, response);
		if (rt != null) {
			return err(rt, request, mapping, response);
		}
		return null;
	}

	/**
	 * 出错流程
	 * 
	 * @param msg
	 * @param request
	 * @param mapping
	 * @param response
	 * @return
	 */
	private ActionForward err(String msg, HttpServletRequest request,
			ActionMapping mapping, HttpServletResponse response) {
		BaseActionUtil.addErr(request, msg);
		return initPage(request, mapping);
	}

	/**
	 * 检查用户提交数据
	 * 
	 * @param form
	 * @param request
	 * 
	 * @return
	 */
	private String check(ActionForm form, HttpServletRequest request) {
		GuideLogger.writeLine(this, "check() -- start");
		FormZhongxue fform = (FormZhongxue) form;
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+MyRequestUtil.getBrowserInfo(request));
		String rt = null;
		if(form==null || fform == null){
			rt = "系统错误，请重试";
			return rt;
		}
		int type = 0;
		try {
			type = Integer.valueOf(fform.getSchooltype()) + 2;
		} catch (NumberFormatException e) {
			GuideLogger.printLog(" host:"+host.getId()+" err:"+e.toString());
			//e.printStackTrace();
		}
		GuideLogger.writeLine(this, "check() -- type"+type);
		int largeyear = 0;
		int smallyear = 1;
		switch (type) {
		case SchoolType.TYPE_HIGHSCHOOL: //gaozhong
			if(MyStringUtil.isEmpty(fform.getHighschoolname())){
				rt = IRegCheckCodeDef.ERR_NO_INPUT_HIGHSCHOOL;
			}
			if(MyStringUtil.isEmpty(fform.getJuniorhighschoolname())){
				rt = IRegCheckCodeDef.ERR_NO_INPUT_JUNIORSCHOOL;
			}
			largeyear = fform.getHighschoolyear();
			smallyear = fform.getJuniorhighschoolyear();
		
			break;
		case SchoolType.TYPE_JUNIORSCHOOL: //chuzhong
			if(MyStringUtil.isEmpty(fform.getJuniorhighschoolname())){
				rt = IRegCheckCodeDef.ERR_NO_INPUT_JUNIORSCHOOL;
			}
			if(MyStringUtil.isEmpty(fform.getElementaryschoolname())){
				rt = IRegCheckCodeDef.ERR_NO_INPUT_ELEMENTARYSCHOOL;
			}
			largeyear = fform.getJuniorhighschoolyear();
			smallyear = fform.getElementaryschoolyear();
			
			break;
		case SchoolType.TYPE_COLLEGESCHOOL: //jixiao
			if(MyStringUtil.isEmpty(fform.getTechschoolname())){
				rt = IRegCheckCodeDef.ERR_NO_INPUT_COLLEGE;
			}
			if(MyStringUtil.isEmpty(fform.getJuniorhighschoolname())){
				rt = IRegCheckCodeDef.ERR_NO_INPUT_JUNIORSCHOOL;
			}
			largeyear = fform.getTechschoolyear();
			smallyear = fform.getElementaryschoolyear();
			
			break;
		}
		
		if(largeyear < smallyear){
			switch (type) {
			case SchoolType.TYPE_HIGHSCHOOL: // gaozhong
				rt = "初中入学年不能大于高中入学年";
				break;
			case SchoolType.TYPE_JUNIORSCHOOL: // chuzhong
				rt = "小学入学年不能大于初中入学年";
				break;
			case SchoolType.TYPE_COLLEGESCHOOL: // jixiao
				rt = "小学入学年不能大于中专/技校入学年";
				break;
			}
		}
		
		
		
		List<String> antispamList = new ArrayList<String>();
		antispamList.add(fform.getHighschoolname());
		antispamList.add(fform.getJuniorhighschoolname());
		antispamList.add(fform.getTechschoolname());
		antispamList.add(fform.getElementaryschoolname());
		for (String antispamStr : antispamList) {
			if (StringUtils.isEmpty(antispamStr))
				continue;
			CheckResult cr = Antispam.checkAndFilterCR(host.getId(),
					antispamStr);
			switch (cr.getFlag()) {
			case CheckResult.SAFE:
				break;
			case CheckResult.PROHIBITED:
				request.setAttribute("err_msg", DodoyoResource.getString("errors.anti.web"));
				return DodoyoResource.getString("errors.anti.web");
			default:
				break;
			}
		}
		request.setAttribute("err_msg", rt);
		GuideLogger.writeLine(this, "check() -- end rt = "+rt);
		return rt;
	}

	/**
	 * 处理用户提交数据
	 * 
	 * @param response
	 * @param mapping
	 * @param request
	 * @param form
	 * 
	 * @return
	 */
	private String sb(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		FormZhongxue fform = (FormZhongxue) form;
		GuideLogger.writeLine(this,"sb() -- save 1");
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - start");
		try {
			GuideLogger.writeLine(this,"Schooltype int is "+fform.getSchooltype()+" "+fform.getHighschoolname()+" "+fform.getJuniorhighschoolname()+" "+fform.getElementaryschoolname()+" "+fform.getTechschoolname());
			int stype = Integer.valueOf(fform.getSchooltype()) + 2;
			boolean iscurrentnetvalid = checkMsNet(fform,stype);
			switch (stype) {
			case SchoolType.TYPE_HIGHSCHOOL:
				GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - TYPE_HIGHSCHOOL");
				
				GuideUserProfileLogic.getInstance().asyncSaveJuniorschool(host, fform);
				GuideUserProfileLogic.getInstance().saveHighschool(host, fform);
				if(iscurrentnetvalid){
					GuideUserProfileLogic.getInstance().asyncSaveHighschoolNet(host, fform);
				}
				break;
			case SchoolType.TYPE_JUNIORSCHOOL:
				GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - TYPE_JUNIORSCHOOL");
				
				GuideUserProfileLogic.getInstance().asyncSaveElementaryschool(host, fform);
				GuideUserProfileLogic.getInstance().saveJuniorschool(host, fform);
				
				//2009-10-21 11:07:55 加上 存初中网络
				if(iscurrentnetvalid){
					GuideUserProfileLogic.getInstance().asyncSaveJuniorschoolNet(host, fform);
				}
				break;
			case SchoolType.TYPE_COLLEGESCHOOL:
				GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - TYPE_COLLEGESCHOOL");
				GuideUserProfileLogic.getInstance().asyncSaveJuniorschool(host, fform);
				GuideUserProfileLogic.getInstance().saveCollege(host, fform);
				
				//2009-10-21 11:07:55 加上 存技校网络
				if(iscurrentnetvalid){
					GuideUserProfileLogic.getInstance().asyncSaveCollegeNet(host, fform);
				}
				break;
			}
			
		} catch (RuntimeException e) {
			GuideLogger.writeLine(this, "sb() error zhongxue "+e.getMessage(),1);
		}
		
		try {
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - savestage - stage:"+Stage.STAGE_HEIGHSCHOOL);
			//阶段
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_HEIGHSCHOOL);
			
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() saveStage error "+host.getId()+" "+e.toString(),1);
		}

		/*//2010-2-5 14:14:00 Inshion 去掉
		//新手帮
		A090729Util.asyncBeFansofPage(600002351, host);*/
		return null;
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
				GuideLogger.printLog("checkMsNet() high true");
				return true;
			}
			break;
		case SchoolType.TYPE_JUNIORSCHOOL: //chuzhong
			if (StringUtils.isNotEmpty(fform.getJuniorhighschoolname())){
				GuideLogger.printLog("checkMsNet() juni true");
				return true;
			}
			break;
		case SchoolType.TYPE_COLLEGESCHOOL: //jixiao
			if (StringUtils.isNotEmpty(fform.getTechschoolname())){
				GuideLogger.printLog("checkMsNet() tech true");
				return true;
			}
			break;
		}
		GuideLogger.writeLine(this,"checkMsNet() false schooltype = "+type );
		return false;
	}

	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		GuideLogger.writeLine(this,"init()");
		A090729Util.setInitAttributes(request);
		return mapping.findForward(FORWARD_SUCC);
	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return !"post".equalsIgnoreCase(request.getMethod());
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/fi-090729.do";
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_FILLINFO;
	}

	@Override
	protected String nextUrl() {
		return Globals.urlGuide + "/pv-090729.do?h=true";
	}
}
