package com.xiaonei.reg.guide.action;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.portrait.PendingUserLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * 上传
 * 
 * @author wangtai
 * 
 */
public class NoStagePortraitAction extends GuideFlowBaseAction {

	/**
	 * 初始化页面
	 * 
	 * @param request
	 * @param mapping
	 * @return
	 */
	// @Override
	protected ActionForward initPage(HttpServletRequest request,ActionMapping mapping) {
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
		request.setAttribute("sex_flag", UserAdapter.get(host).getGender());
		// 跳转到head.upload域下
		String sex_flag = "1";
		if (User.genderFemale.equalsIgnoreCase(UserAdapter.get(host).getGender())) {
			sex_flag = "0";
		}
		try {
            return new ActionForward("http://head.upload." + Globals.domain + "/registerguide/Entry.do?rurl=" 
                    + URLEncoder.encode(Globals.urlGuide + "/head-upload-ns.do"	+ "?skip=1","UTF-8") + "&sex_flag=" + sex_flag, true);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return null;
	}

	@Override
	protected String nextUrl() {
		return Globals.urlGuide + "/guide.do";
	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		String skip = request.getParameter("skip");
		return !"2".equalsIgnoreCase(skip);
	}

	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		User host = BaseThreadUtil.currentHost(request);
		// 存入加星系统
		try {
			PendingUserLogic.save(host);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_UPLOAD_HEAD;
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/head-upload-ns.do";
	}

}
