package com.xiaonei.reg.guide.action.a090729;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.portrait.PendingUserLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.RegPortraitLogic;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * 上传
 * 
 * @author wangtai
 * 
 */
public class PortraitAction extends GuideFlowBaseAction {

	/**
	 * 初始化页面
	 * 
	 * @param request
	 * @param mapping
	 * @return
	 */
	// @Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - start" +" - current GDomain is:"+Globals.urlGuide);
		cost.endStep();
		try {
			request.setAttribute("sex_flag", UserAdapter.get(host).getGender());
		} catch (Exception e) {
			GuideLogger.printLog("setAttribute failed!"+ host.getId(),GuideLogger.ERROR);
		}
		cost.endStep();
		// 跳转到head.upload域下
		String sex_flag = "1";
		if (User.genderFemale.equalsIgnoreCase(UserAdapter.get(host).getGender())) {
			sex_flag = "0";
		}
		cost.endStep();
		
		boolean hasHead = true;
		try {
			hasHead = RegPortraitLogic.hasUploaded(host);
		} catch (Exception e) {
			GuideLogger.printLog("hasUploaded failed!"+ host.getId(),GuideLogger.ERROR);
		}
		if(hasHead){
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - end - to guide - head = "+UserAdapter.get(host).getHeadUrl());
			cost.endStep();
			try {
				succOk(request, null, mapping, null);
			} catch (Exception e) {
				GuideLogger.printLog("succOk failed!"+ host.getId(),GuideLogger.ERROR);
			}
			cost.endStep();
			return new ActionForward(Globals.urlGuide+"/guide.do",true);
		}
		cost.endStep();
		
		//nav_bar=1
		String nav_bar = "";
		cost.endStep();
		try {
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"try nav_bar");
			cost.endStep();
			//2009-12-21 15:58:22 没用的 去掉
//			if(A090729Util.isChannnelUser(host)){
//				nav_bar = "&nav_bar=1";
//				GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"has nav_bar");
//				cost.endStep();
//			}
//			else{
//				nav_bar = "&nav_bar=0";
//				GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"no nav_bar");
//				cost.endStep();
//			}
			nav_bar = "&nav_bar=0";
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"no nav_bar");
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId(),1);
		}
		
		cost.endStep();
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - end - sex="+sex_flag);
		cost.endFinally();
		
		String skipurl = Globals.urlGuide + "/skipupload.do";
		try {
			//HtmlEncoder.encode(Globals.urlGuide + "/skipupload.do");
			skipurl = URLEncoder.encode(Globals.urlGuide + "/skipupload.do","UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		} 
		String headurl = "http://head.upload."+Globals.domain+"/registerguide/Entry.do?rurl="
		+ skipurl + "&sex_flag="
		+ sex_flag+nav_bar;
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - headurl = "+headurl);
		return new ActionForward(headurl, true);
	}

	@Override
	protected String nextUrl() {
		return Globals.urlGuide + "/guide.do";
	}
	
//	@Override
//	protected String nextUrl(HttpServletRequest request) {
//		User host = BaseThreadUtil.currentHost(request);
//		try {
//			if(A090729Util.isChannnelUser(host)){
//				return Globals.urlGuide + "/guide.do?iscl=1";
//			}
//		} catch (Exception e) {
//			MyLogger.writeLine(this,"nextUrl() - host:"+host.getId(),1);
//		}
//		return Globals.urlGuide + "/guide.do";
//	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		String skip = request.getParameter("skip");
		return !"2".equalsIgnoreCase(skip);
	}

	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		// 存入加星系统
		try {
			PendingUserLogic.save(host);
			cost.endStep();
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
		return null;
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_UPLOAD_HEAD;
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide
		+ "/hu-090729.do";
	}

}
