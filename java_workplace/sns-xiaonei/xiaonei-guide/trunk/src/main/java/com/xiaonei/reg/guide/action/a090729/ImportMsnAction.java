package com.xiaonei.reg.guide.action.a090729;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

public class ImportMsnAction extends GuideFlowBaseAction {

	private static final String FORWARD_SUCC = "succ";

	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form, ActionMapping mapping, HttpServletResponse response) {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		// 标记完成
		StepStatusLogic.updateStepStatus(host.getId(),StepStatusLogic.STEP_IMPORT_MSN);
		cost.endFinally();
		return new ActionForward((Globals.urlGuide + "/hu-090729.do"), true);
	}

	@Override
	protected ActionForward initPage(HttpServletRequest request,ActionMapping mapping) {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.writeLine(this, "initPage() - hostid:" + host.getId() + " - start");
		cost.endStep();
		try {
			UserDesc ud = WUserDescAdapter.getInstance().get(host.getId());
			cost.endStep();
			if (null != ud) {
				request.setAttribute("msn", ud.getMsn());
			}
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.writeLine(this, "initPage() - hostid:" + host.getId() + " - error", 1);
			e.printStackTrace();
		}
		cost.endStep();
		request.setAttribute("user_email", StringFormat.isValidEmailAddr(host.getAccount()) ? StringUtils.trimToEmpty(host.getAccount()) : "");
		request.setAttribute("is_f", StringFormat.isValidEmailAddr(host.getAccount()) ? "true" : "false");
		try {
            request.setAttribute("rurl", URLEncoder.encode(Globals.urlGuide + "/im-090729.do" + "?skip=1","UTF-8"));
            request.setAttribute("ourl", URLEncoder.encode(Globals.urlGuide + "/im-090729.do","UTF-8"));
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }		
		cost.endStep();
		String rt = errNo(request);
		cost.endStep();
		if (null != rt) {
			BaseActionUtil.addErr(request, rt);
		}
		cost.endStep();
		GuideLogger.writeLine(this,"initPage()");
		A090729Util.setInitAttributes(request);
		cost.endStep();
		GuideLogger.writeLine(this, "initPage() - hostid:" + host.getId() + "end");
		return mapping.findForward(FORWARD_SUCC);
	}

	/**
	 * 远程获得错误代码
	 * 
	 * @param request
	 * @return
	 */
	private String errNo(HttpServletRequest request) {
		String errNo = request.getParameter("errno");
		if ("1".equals(errNo))
			return "密码或帐号错误, 要不要换个帐号试试？";
		if ("2".equals(errNo))
			return "密码或帐号错误, 要不要换个帐号试试？";
		return null;
	}

	/**
	 * 时候应该初始化
	 * 
	 * @param request
	 * @return
	 */
	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return !"ok".equalsIgnoreCase(request.getParameter("msn"));
	}

	@Override
	protected String nextUrl() {
		return (Globals.urlGuide + "/hu-090729.do");
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_IMPORT_MSN;
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/im-090729.do";
	}
}
