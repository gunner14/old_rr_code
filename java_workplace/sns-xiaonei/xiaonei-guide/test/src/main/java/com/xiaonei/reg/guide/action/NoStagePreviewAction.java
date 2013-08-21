/**
 * 
 */
package com.xiaonei.reg.guide.action;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.preview.CommonPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;
import com.xiaonei.search2.model.client.SearchView;

/**
 * @author wangtai
 * 
 */
public class NoStagePreviewAction extends GuideFlowBaseAction {

	private static final String SUCC_PAGE = "succ";

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return true;

	}

	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		return null;
	}

	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
		List<SearchView> hsList = null;
		NewGuideView n = getHSGuideView(host, 40);
		if (n != null) {
			hsList = n.getResult();
		}
		if (null == hsList || hsList.size() == 0) {
			return succGo(request, null, mapping, null);
		}
		succGo(request, null, mapping, null);
		request.setAttribute("highschoolList", hsList);
		request.setAttribute("mixList", hsList);
		request.setAttribute("next_url", nextUrl(request) + "?rt=o");
		request.setAttribute("this_url", thisUrl());
		request.setAttribute("skip_url", thisUrl() + "?skip=1");

		return mapping.findForward(SUCC_PAGE);
	}

	@Override
	protected String nextUrl() {
		return null;
	}

	@Override
	protected String nextUrl(HttpServletRequest request) {
		if (BirthdayLogic.getInstance().less17age(request))
			return Globals.urlGuide + "/head-upload-ns.do";
		else
			return Globals.urlGuide + "/import-msn-ns.do";
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_PREVIEW;
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/preview-ns.do";
	}

	private NewGuideView getHSGuideView(User host, int count) {
		return CommonPreviewLogic.getInstance().getHSGuideView(host, count);
	}

}
