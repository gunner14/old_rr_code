package com.xiaonei.reg.guide.action.netbarflow;

import java.sql.SQLException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.a090821.UserProfileUtil;
import com.xiaonei.reg.guide.util.GuideLogger;

public class PreviewInitAction extends ProtectAction{

	private final static String INIT = "init";
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		GuideLogger.writeLine(this , "| host:"+ host.getId()+ " - start");
		cost.endStep();
		try {
			List<Page> list = PageService.getInstance().getPages( PageChooser.getPageIds(host) );
			GuideLogger.printLog("| host:"+host.getId()+" gets pages count:"+list.size());
			request.setAttribute("pagelist", list);
			
		} catch (Exception e) {
			GuideLogger.printLog("| host:"+host.getId()+" error:"+e.toString());
			e.printStackTrace();
		}
		cost.endStep();
		try {
			RegionInfo info = UserProfileUtil.loadRegion(host);
			request.setAttribute("info", info);
			request.setAttribute("next_url", "/pvnbs.do?skip=1");
		} catch (Exception e) {
			GuideLogger.printLog("| host:"+host.getId()+" error:"+e.toString());
			e.printStackTrace();
		}
		cost.endStep();
		NetbarUtil.setInitAttribute(request);
		cost.endStep();
		GuideLogger.writeLine(this , "| host:"+ host.getId()+ " - end");
		cost.endStep();
		cost.endFinally();
		return mapping.findForward(INIT);
	}

}
