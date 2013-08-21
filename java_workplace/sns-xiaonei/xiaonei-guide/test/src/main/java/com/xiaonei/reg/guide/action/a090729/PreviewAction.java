/**
 * 
 */
package com.xiaonei.reg.guide.action.a090729;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.preview.CommonPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;

/**
 * @author wangtai
 * 
 */
public class PreviewAction extends GuideFlowBaseAction {

	private static final String SUCC_PAGE = "succ";

	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		return null;
	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return true;
	
	}

	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		GuideLogger.writeLine(this, "initPage() - 1");
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - start");
		cost.endStep();
		
		//Inshion 2009-9-4 Kaixin的没有推人页面
		//Inshion 2009-9-18 现在有了
		GuideLogger.writeLine(this,"initPage() host:"+host.getId()+" domain is:"+OpiConstants.domain.toString());
//		if("kaixin.com".equals(host.getDomain()) || "kaixin.com".equals(OpiConstants.domain.toString())){
//			return succGo(request, null, mapping, null);
//		}
		
//		List<SearchView> hsList = null;
//		cost.endStep();
//		NewGuideView n = getHSGuideView(host, 40);
//		cost.endStep();
//		if (n != null) {
//			hsList = n.getResult();
//		}
//		cost.endStep();
//		if (null == hsList || hsList.size() == 0) {
//			MyLogger.writeLine(this, "initPage() - 2 size is:");
//			return succGo(request, null, mapping, null);
//		}
		if(!hasPreviews(host,request)){
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - end of no previews!");
			return succGo(request, null, mapping, null);
		}
		cost.endStep();
		try {
			//为了标记完成而作的调用 先去掉，应该是Go的时候会自动Update
			succGo(request, null, mapping, null);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("succGo failed! host:"+host.getId(),GuideLogger.ERROR);
		}
		
//		request.setAttribute("highschoolList", hsList);
//		request.setAttribute("mixList", hsList);
		
		try {
			request.setAttribute("next_url", nextUrl(request) + "?rt=o");
			request.setAttribute("this_url", thisUrl());
			request.setAttribute("skip_url", thisUrl() + "?skip=1");
			
			GuideLogger.writeLine(this,"initPage()");
			A090729Util.setInitAttributes(request);
			
		} catch (Exception e) {
			GuideLogger.printLog("setInitAttributes failed! host:"+host.getId(),GuideLogger.ERROR);
		}
		
		cost.endFinally();
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - end");
		return mapping.findForward(SUCC_PAGE);
	}

	@Override
	protected String nextUrl() {
		return null;
	}

	@Override
	protected String nextUrl(HttpServletRequest request) {
		if (BirthdayLogic.getInstance().less17age(request)){
			GuideLogger.writeLine(this, "nextUrl() less17 to HEAD!");
			return Globals.urlGuide + "/hu-090729.do";
		}
		else{
			GuideLogger.writeLine(this, "nextUrl() >17  to import!");			
			return Globals.urlGuide + "/im-090729.do";
		}
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_PREVIEW;
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/pv-090729.do";
	}

	@SuppressWarnings("unused")
	private NewGuideView getHSGuideView(User host, int count) {
		return CommonPreviewLogic.getInstance().getHSGuideView(host, count);
	}
	
	
	private boolean hasPreviews(User host , HttpServletRequest request ){
//		MyTimeCost cost = MyTimeCost.begin();
		return true;
//		MyLogger.writeLine(this,request.getRequestURI()+" "+request.getRequestURL());
//		if(request.getParameter("h") == null)return false;
//		if("true".equals(request.getParameter("h"))){
//			return true;
//		}
//		cost.endFinally();
//		return false;
	}

}
