package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.msn.CommonImportMsnLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

public class DXRegImportMsnAction extends ProtectAction {

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
		String succ = "succ";
		UserTraceLogic.getInstance().insertRegHis(request, host,
				host.getAccount(), String.valueOf(RegOprDefine.IMPORT_MSN),
				UserAdapter.get(host).getStage(), null, null, null, null, 0);
		StepStatusLogic.updateStepStatus(host.getId(),
				StepStatusLogic.STEP_PREVIEW);
		String toUrl = StepStatusLogic.goWhere(host);
		if (!"post".equalsIgnoreCase(request.getMethod())) {
			return mapping.findForward(succ);
		}
		if (!Utils.getInstance().validateForString(toUrl)
				&& !toUrl.equalsIgnoreCase(Globals.urlGuide
						+ "/regimportmsndx.do")) {
			return new ActionForward(toUrl, true);
		}
		request.setAttribute("university", CommonImportMsnLogic.getInstance()
				.getUniversity(host));
		NewGuideView ngv = CommonImportMsnLogic.getInstance()
				.getUniversityData(host);
		request.setAttribute("univList", ngv);

		return mapping.findForward(succ);
	}

}
