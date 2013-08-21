package com.xiaonei.reg.guide.action;

import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.page.domain.spread.SpreadPageType;
import com.xiaonei.page.domain.view.PageSpreadView;
import com.xiaonei.page.service.spread.SpreadFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;

public class GuideAjaxFeedPageAction {
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {		
		User user = BaseThreadUtil.currentHost(request);		
		Map<Integer,SpreadPageType> pageTypes = SpreadFactory.getInstance().getHomeTabSpread().getPageTypes();
		request.setAttribute("pageTypes", pageTypes);	
		int pageSize=8;
		int ptid=1;		
		if(request.getParameter("ptid")!=null){
			ptid=Integer.parseInt(request.getParameter("ptid"));
		}		
		request.setAttribute("ptid", ptid);	
		List<PageSpreadView> pageSpreadView=SpreadFactory.getInstance().getHomeTabSpread().getUserSpread(user.getId(), ptid, pageSize);	
		request.setAttribute("pageSpreadView", pageSpreadView);
		return mapping.findForward("succ");
	}
}
