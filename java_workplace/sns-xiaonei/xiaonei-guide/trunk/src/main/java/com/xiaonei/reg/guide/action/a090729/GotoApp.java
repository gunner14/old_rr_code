package com.xiaonei.reg.guide.action.a090729;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

/**
 * 上传
 * 
 * @author wangtai
 * 
 */
public class GotoApp extends GuideFlowBaseAction {


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
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - start");
		succGo(request, null, mapping, null);
		
		String toUrl = "http://app."+Globals.domain+"/apps/apps.do?selected=2&type=1&category=0&origin=2801";
		String fromSource = ""+RegFromSourceLogic.getFromSource(host);
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - fromSource is :"+fromSource);
		String valueUrl = A090729Util.getUrl(fromSource);
		String farmUrl = A090729Util.getUrl("page.farm");
		String cookUrl = A090729Util.getUrl("page.cook");
		if(StringUtils.isNotEmpty(valueUrl)){
			toUrl = valueUrl;
		}
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - fromSource is :"+fromSource);
		List<String> list = RegFromSourceLogic.getFromSoureList(fromSource);
		if(list.contains("S_FARMREG")){
			toUrl = farmUrl;
		}
		if(list.contains("S_COOKREG")){
			toUrl = cookUrl;
		}
		/*
		if("page.S_FARMREG".equals(fromSource.trim())){
			toUrl = farmUrl;
		}
		if("page.S_COOKREG".equals(fromSource.trim())){
			toUrl = cookUrl;
		}
		*/
		if(toUrl == null || toUrl.trim().equals("")){
			toUrl = "http://app."+Globals.domain+"/apps/apps.do?selected=2&type=1&category=0&origin=2801";
		}
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - to url is :"+toUrl);
		return new ActionForward(toUrl, true);
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
		return null;
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_APP;
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/gapp.do";
	}

}
