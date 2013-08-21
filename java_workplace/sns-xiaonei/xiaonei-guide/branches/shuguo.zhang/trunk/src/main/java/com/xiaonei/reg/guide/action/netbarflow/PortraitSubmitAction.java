package com.xiaonei.reg.guide.action.netbarflow;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;



public class PortraitSubmitAction extends ProtectAction{
	private final int thisStep = StepStatusLogic.STEP_UPLOAD_HEAD;
	private final static String INIT = "init";
	private final static String SUCC = "succ";
	
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		MyTimeCost cost = MyTimeCost.begin();
		String pa_skip = request.getParameter("skip");
		cost.endStep();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		GuideLogger.writeLine(this , "| host:"+ host.getId()+ " - start");
		cost.endStep();
		if("1".equals(pa_skip)){
			doSkip(host);
			cost.endStep();
			cost.endFinally();
			return mapping.findForward(SUCC);
		}
		
		GuideLogger.writeLine(this , "| host:"+ host.getId()+ " - end");
		cost.endStep();
		cost.endFinally();
		return mapping.findForward(INIT);
	}
	
	private void doSkip(User host) {
		MyTimeCost cost = MyTimeCost.begin();
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		cost.endStep();
		cost.endFinally();
	}

}
